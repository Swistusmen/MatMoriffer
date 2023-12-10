#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/delay.h>
#include <linux/string.h>
#include "message_buffer.h"
#include "../lib/matmorifferctl.h"
// general
static message_buffer messages;
static struct sock *socket;

enum PROTOCOL
{
    TCP,
    UDP,
};

static char listening_protocols;

static void swich_protocol_value(const enum PROTOCOL protocol)
{
    listening_protocols ^= (1 << protocol);
}

static int is_protocol_turned_on(const enum PROTOCOL protocol)
{
    return (listening_protocols & (1 << protocol)) != 0;
}

// general
// handling network
static unsigned int netfilter_hooking_fun(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    if (listening_protocols != 0)
    {
        struct iphdr *ip_header;
        ip_header = ip_hdr(skb);
        message *msg;
        msg = kmalloc(sizeof(struct message), GFP_KERNEL);
        char content[100];

        if (is_protocol_turned_on(TCP) && ip_header->protocol == IPPROTO_TCP)
        {
            struct tcphdr *tcp_header;
            tcp_header = (struct tcphdr *)(ip_header + 1);
            snprintf(content, sizeof(content), "TCP from src IP: %pI4, src port: %d, dst IP: %pI4, dst port: %d\n",
                     &ip_header->saddr, ntohs(tcp_header->source),
                     &ip_header->daddr, ntohs(tcp_header->dest));
            initialize_message(msg, content);
            push_message(&messages, msg);
        }
        else if (is_protocol_turned_on(UDP) && ip_header->protocol == IPPROTO_UDP)
        {
            struct udphdr *udp_header;
            udp_header = (struct udphdr *)(ip_header + 1);
            snprintf(content, sizeof(content), "UDP from src IP: %pI4, src port: %d, dst IP: %pI4, dst port: %d\n",
                     &ip_header->saddr, ntohs(udp_header->source),
                     &ip_header->daddr, ntohs(udp_header->dest));
            initialize_message(msg, content);
            push_message(&messages, msg);
        }

        return NF_ACCEPT;
    }
    return NF_DROP;
}

static struct nf_hook_ops nfho = {
    .hook = netfilter_hooking_fun,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST};
// handling network

// handling netlink sockets
static int send_netlink_message(message *msg, struct nlmsghdr *nlh, pid_t pid)
{
    size_t message_size = strlen(msg->content) + 1;
    struct sk_buff *response = nlmsg_new(message_size, GFP_KERNEL);
    if (!response)
    {
        printk(KERN_INFO "Failed to allocate struc kernel buffer\n");
        kfree(msg);
        return 0;
    }

    nlh = nlmsg_put(response, 0, 0, NLMSG_DONE, message_size, 0);
    NETLINK_CB(response).dst_group = 0;
    strncpy(nlmsg_data(nlh), msg->content, message_size);
    nlmsg_unicast(socket, response, pid);
    return 1;
}

static void receive_netlink_message(struct sk_buff *skb)
{
    while (true)
    {
        struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
        char message_from_userspace[100];
        //tutaj jest blad
        strcpy(message_from_userspace, (char *)nlmsg_data(nlh));
        int are_different = strcmp(message_from_userspace, BREAK_COMMUNICATION);
        if (are_different == 0)
        {
            printk(KERN_INFO, "received break communicate");
            goto exit;
        }
        printk(KERN_INFO "DEBUG: received message %s", message_from_userspace);
        pid_t pid = nlh->nlmsg_pid;
        message *msg;

        pop_message(&messages, &msg);
        int messages_counter = 1;
        while (msg)
        {
            if (!send_netlink_message(msg, nlh, pid))
            {
                return;
            }

            if (messages_counter == TIME_TO_WAIT)
            {
                break;
            }
            kfree(msg);
            pop_message(&messages, &msg);
            messages_counter++;
        }
        msg = kmalloc(sizeof(struct message), GFP_KERNEL);
        strcpy(msg->content, CONTINUE_COMMUNICATION);
        if (!send_netlink_message(msg, nlh, pid))
        {
            return;
        }
        kfree(msg);
        mdelay(TIME_TO_WAIT);
    }
exit:
    return;
}

static struct netlink_kernel_cfg netlink_socket_config = {
    .input = receive_netlink_message,
};
// handling netlink sockets

// TODO: receive_netlink_message-
// add 2nd loop to wait some time for loading message from TCP
//(also waiting for some response from userspace-that still listening)
// TODO: receive_netlink_message- add some counter f.e. 100-> after that break from inner loop (to free buffer)