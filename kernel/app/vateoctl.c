#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "matmorifferctl.h"

#define MAX_PAYLOAD 1024

//simple application for testing purposes

void print_help();
void switch_protocol(const int protocol);
void netlink_socket(int argc,char** argv);

int main(int argc, char** argv){
    if(getuid()!=0){
        printf("Error: This program demands root priviliges\n");
        return 0;
    }

    if(argc==1){
        print_help();
        return 0;
    }

    if(!strcmp(argv[1],"help")) print_help();
    else if(!strcmp(argv[1],"tcp")) switch_protocol(1);
    else if(!strcmp(argv[1],"udp")) switch_protocol(2);
    else if(!strcmp(argv[1],"netlink-socket")) netlink_socket(argc-1,argv+1);
    else print_help();

    return 0;
}

void switch_protocol(const int protocol){
        char snap_device_path[100];
        strcpy(snap_device_path, "/dev/");
        strcat(snap_device_path,MATMORIFFER_SNAP_DEVICE);

        int fd=open(snap_device_path,O_RDWR);
        if (fd<0) goto error;

        int ret;
        if(protocol==1){
            ret=ioctl(fd,TURN_ON_TCP);
        }else if(protocol==2){
            ret=ioctl(fd,TURN_ON_UDP);
        }
        if(ret){
            printf("Error when calling %s : %d\n",snap_device_path,ret);
        }
        close(fd);
        return;

        error:
        printf("could not open %s \n",snap_device_path);
}

void print_help()
{
    printf("help --for help\n");
    printf("tcp- turns on/off listening on tcp\n");
    printf("udp- turns on/off listening on tcp\n");
    printf("netlink-socket for testing netlink socket\n");
}

void netlink_socket(int argc,char** argv)
{
    int fd=socket(AF_NETLINK, SOCK_RAW, NETLINK_TESTFAMILY);
    if(fd<0){
        printf("could not create socket, returning \n");
        return;
    }
    char message[100];
    while(1){
    printf("/n> ");
    fgets(message, sizeof(message), stdin); 
    message[strcspn(message, "\n")] = '\0';

    if (strcmp(message, "exit") == 0) {
        break;
    }
    //sending a message
    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family=AF_NETLINK;
    addr.nl_pid = 0; 
    addr.nl_groups = 0;

    struct nlmsghdr *nlh = (struct nlmsghdr *) malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strcpy((char *) NLMSG_DATA(nlh), message);

    struct iovec iov; memset(&iov, 0, sizeof(iov));
    iov.iov_base = (void *) nlh;
    iov.iov_len = nlh->nlmsg_len;

    struct msghdr msg; memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *) &addr;
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    sendmsg(fd, &msg, 0);
    //receive a message
    free(nlh);
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    struct iovec iov_res;
    struct msghdr response;
        
    iov_res.iov_base = (void *)nlh;
    iov_res.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
        
    response.msg_name = (void *)&addr;
    response.msg_namelen = sizeof(addr);
    response.msg_iov = &iov_res;
    response.msg_iovlen = 1;
        
    ssize_t recv_len = recvmsg(fd, &response, 0);
        
    if (recv_len < 0) {
        perror("recvmsg error");
        break;
    }
        
    //show message which I got
    char *received_message = (char *)NLMSG_DATA(nlh);
    printf("Received message: %s\n", received_message);
    }
    close(fd);
}