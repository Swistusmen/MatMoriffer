#ifndef MATMORIFFERCTL
#define MATMORIFFERCTL
#include <linux/ioctl.h>

#define MATMORIFFER_IOCTL_MAGIC 'A'
#define TURN_ON_TCP _IO(MATMORIFFER_IOCTL_MAGIC, 1)
#define TURN_ON_UDP _IO(MATMORIFFER_IOCTL_MAGIC, 2)
#define MATMORIFFER_SNAP_DEVICE "matmoriffer_snap_device"
#define NETLINK_TESTFAMILY 25

//userspac-kernel space communication
#define CONTINUE_COMMUNICATION "continue"
#define BREAK_COMMUNICATION "exit"
#define TIME_TO_WAIT 100
//
#endif