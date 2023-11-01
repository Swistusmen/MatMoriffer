#ifndef MATMORIFFERCTL
#define MATMORIFFERCTL
#include <linux/ioctl.h>

#define MATMORIFFER_IOCTL_MAGIC 'A'
#define TEST_INCREMENTAL _IO(MATMORIFFER_IOCTL_MAGIC, 1)
#define MATMORIFFER_SNAP_DEVICE "matmoriffer_snap_device"
#define NETLINK_TESTFAMILY 25

#endif