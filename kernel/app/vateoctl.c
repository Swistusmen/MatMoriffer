#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "matmorifferctl.h"

int main(int argc, char** argv){
    if(getuid()!=0){
        printf("Error: This program demands root priviliges\n");
        return 0;
    }

    char snap_device_path[100];
    strcpy(snap_device_path, "/dev/");
    strcat(snap_device_path,MATMORIFFER_SNAP_DEVICE);


    int fd=open(snap_device_path,O_RDWR);
    if (fd<0) goto error;

    int ret=ioctl(fd,TEST_INCREMENTAL);
    if(ret){
        printf("Error when calling %s : %d\n",snap_device_path,ret);
    }
    close(fd);
    return 0;

    error:
    printf("could not open %s \n",snap_device_path);
    return 0;
}