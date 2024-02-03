#include "libmatmoriffer.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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
