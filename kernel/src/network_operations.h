#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include "message_buffer.h"
//general
static message_buffer messages;
static struct sock* socket;

enum PROTOCOL{
    TCP,
    UDP,
};

static char listening_protocols;

static void swich_protocol_value(const enum PROTOCOL protocol){
    listening_protocols ^=(1<<protocol);
}

static int is_protocol_turned_on(const enum PROTOCOL protocol){
    return (listening_protocols & (1 << protocol)) != 0;
}

//general
//handling network
static unsigned int netfilter_hooking_fun(void * priv, struct sk_buff* skb,const struct nf_hook_state* state){
    if(listening_protocols!=0){
        struct iphdr *ip_header;
        ip_header = ip_hdr(skb);
        message* msg;
        msg = kmalloc(sizeof(struct message), GFP_KERNEL);
        char content[100];

    if (is_protocol_turned_on(TCP) && ip_header->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp_header;
        tcp_header = (struct tcphdr *)(ip_header + 1);
        snprintf(content, sizeof(content), "TCP from src IP: %pI4, src port: %d, dst IP: %pI4, dst port: %d\n",
             &ip_header->saddr, ntohs(tcp_header->source),
             &ip_header->daddr, ntohs(tcp_header->dest));
        initialize_message(msg, content);
        push_message(&messages,msg);
    }
    else if(is_protocol_turned_on(UDP) && ip_header->protocol == IPPROTO_UDP){
        struct udphdr *udp_header;
        udp_header= (struct udphdr*) (ip_header +1);
        snprintf(content, sizeof(content),"UDP from src IP: %pI4, src port: %d, dst IP: %pI4, dst port: %d\n",
            &ip_header->saddr, ntohs(udp_header->source),
            &ip_header->daddr, ntohs(udp_header->dest) );
        initialize_message(msg, content);
        push_message(&messages,msg);
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
//handling network

//handling netlink sockets
static void receive_netlink_message(struct sk_buff* skb){
    //waiting for message in socket 
    struct nlmsghdr * nlh=(struct nlmsghdr*)skb->data;
    printk(KERN_INFO "Received message %s\n",(char*)nlmsg_data(nlh));
    pid_t pid= nlh->nlmsg_pid;

    //sending a message through socket
    char* message= "Response";
    size_t message_size=strlen(message)+1;
    struct sk_buff* response=nlmsg_new(message_size, GFP_KERNEL);
    if(!response){
        printk(KERN_INFO "Failed to allocate struc kernel buffer\n");
        return;
    }

    nlh=nlmsg_put(response,0,0,NLMSG_DONE, message_size,0);
    NETLINK_CB(response).dst_group=0;
    strncpy(nlmsg_data(nlh),message,message_size);
    nlmsg_unicast(socket, response, pid);
}

static struct netlink_kernel_cfg netlink_socket_config={
    .input= receive_netlink_message,
};
//handling netlink sockets
