#include "libmatmoriffer.h"
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

int matmoriffer_turn_tcp()
{
 /*   char snap_device_path[100];
    strcpy(snap_device_path, "/dev/");
    strcat(snap_device_path,MATMORIFFER_SNAP_DEVICE);*/


    char snap_device_path[]="/dev/matmoriffer_snap_device";

    int fd=open(snap_device_path,O_RDWR);
    if (fd<0)
        return 0;

    int ret;

    ret=ioctl(fd,TURN_ON_TCP);

    if(ret){
        printf("Error when calling %s : %d\n",snap_device_path,ret);
        close(fd);
        return 0;
    }
    close(fd);
    return 1;
}

int matmoriffer_turn_udp()
{
    char snap_device_path[100];
    strcpy(snap_device_path, "/dev/");
    strcat(snap_device_path,MATMORIFFER_SNAP_DEVICE);

    int fd=open(snap_device_path,O_RDWR);
    //tutaj jest blad
    if (fd<0)
        return 0;

    int ret;

    ret=ioctl(fd,TURN_ON_UDP);

    if(ret){
        printf("Error when calling %s : %d\n",snap_device_path,ret);
        close(fd);
        return 0;
    }
    close(fd);
    return 1;
}

void netlink_socket(struct NetlinkSocketArguments *args)
{

    int fd=socket(AF_NETLINK, SOCK_RAW, NETLINK_TESTFAMILY);
    if(fd<0){
        printf("could not create socket, returning \n");
        return;
    }
    char message[100];
    int continue_to_listen;
    int exit_communication;
    struct sockaddr_nl addr;
    struct nlmsghdr *nlh;

    while(1){
        //sending a message
        memset(&addr, 0, sizeof(addr));
        addr.nl_family=AF_NETLINK;
        addr.nl_pid = 0; 
        addr.nl_groups = 0;

        nlh = (struct nlmsghdr *) malloc(NLMSG_SPACE(MAX_PAYLOAD));
        memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
        nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
        nlh->nlmsg_pid = getpid();
        nlh->nlmsg_flags = 0;
        strcpy((char *) NLMSG_DATA(nlh), message);

        struct iovec iov;
        memset(&iov, 0, sizeof(iov));
        iov.iov_base = (void *) nlh;
        iov.iov_len = nlh->nlmsg_len;

        struct msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_name = (void *) &addr;
        msg.msg_namelen = sizeof(addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        int ret=sendmsg(fd, &msg, 0);

        //receive a message
        while(1){
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

            struct pollfd fds[1];
            fds[0].fd = fd;
            fds[0].events = POLLIN;

            int ret = poll(fds, 1, TIME_TO_WAIT);

            if (ret > 0 && (fds[0].revents & POLLIN)){
                ssize_t recv_len = recvmsg(fd, &response, 0);
                    
                if (recv_len < 0) {
                    perror("recvmsg error");
                    break;
                }

                //to powoduje crash
                args->buffer[*args->writingIndex] = (char *)NLMSG_DATA(nlh);
                (*args->writingIndex)++;
            }else{
                break;
            }
        }
    }
exit:
    close(fd);
}

void dupa1(char** d){
    strcpy(*d, "dupa");
}
