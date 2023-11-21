#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <linux/blkdev.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include "../lib/matmorifferctl.h"
#include "network_operations.h"

#define MODULE_NAME "matmoriffer"
#define BUFFSIZE 100

static struct proc_dir_entry* entry;
static struct mutex ioctl_mutex;

static void *proc_start(struct seq_file *m, loff_t *pos)
{
    if (*pos < 1)
		return pos;
	return NULL;
}

static void *proc_next(struct seq_file *m, void *v, loff_t *pos)
{
	(*pos)++;
    if (*pos >= 1)
		return NULL;
	return pos;
}

static void proc_stop(struct seq_file *m, void *v){
}

static int proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Spying on protocols: ");
    if(is_protocol_turned_on(TCP)){
        printk(KERN_INFO "TCP\n");
        seq_printf(m, "TCP ");
    }
    if(is_protocol_turned_on(UDP)){
        printk(KERN_INFO "UDP\n");
        seq_printf(m, "UDP ");
    }
    seq_printf(m,"\n");
    return 0;
}

static const struct seq_operations my_seq_ops={
    .start = proc_start,
	.next = proc_next,
	.stop = proc_stop,
	.show = proc_show,
};

static int open_proc_file(struct inode * inode, struct file * filp)
{
    mutex_lock(&ioctl_mutex);
    return seq_open(filp, &my_seq_ops);
}

static int release_proc_file(struct inode *inode, struct file * filp)
{
    seq_release(inode, filp);
	mutex_unlock(&ioctl_mutex);
	return 0;
}

static const struct proc_ops my_proc_ops={
    .proc_open = open_proc_file,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = release_proc_file,
};

long int matmoriffer_ioctl(struct file * flip, unsigned int cmd,  long unsigned int) 
{
    printk(KERN_INFO "matmoriffer_ioctl has been called\n");
    switch(cmd){
        case TURN_ON_TCP:{
            swich_protocol_value(TCP);
            break;
        }
        case TURN_ON_UDP:{
            swich_protocol_value(UDP);
            break;
        }
        default:{

        }
    }
    return 0;
}

static int matmoriffer_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int matmoriffer_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t matmoriffer_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

static ssize_t matmoriffer_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

static const struct file_operations control_fops =
{
        .owner          = THIS_MODULE,
	    .unlocked_ioctl = matmoriffer_ioctl,
        .open = matmoriffer_open,
        .release = matmoriffer_release,
        .read = matmoriffer_read,
        .write = matmoriffer_write,
};

static struct miscdevice control_device={
    .minor=MISC_DYNAMIC_MINOR,
    .name=MATMORIFFER_SNAP_DEVICE,
    .fops=&control_fops,
};

static int __init driver_initialization(void){
    printk(KERN_INFO "Initializing matmoriffer \n");
    listening_protocols=0;
    initialize_message_buffer(&messages);
    entry=proc_create("matmoriffer",0660, NULL,&my_proc_ops);
    int ret;

    ret=misc_register(&control_device);
    if(ret){
        printk(KERN_INFO "registering matmoriffer_snap_device ioctl status: %d", ret);
        return -1;
    }

    socket=netlink_kernel_create(&init_net,NETLINK_TESTFAMILY, &netlink_socket_config );
    if(socket==NULL){
        printk(KERN_INFO "could not create netlink socket\n");
        return -1;
    }

    ret=nf_register_net_hook(&init_net, &nfho);
    if(ret){
        printk(KERN_INFO "Could not register netfilter hook operations\n");
    }

    return 0;
}

static void driver_deinitialization(void){
    free_message_buffer(&messages);
    proc_remove(entry);
    misc_deregister(&control_device);
    if(socket){
        netlink_kernel_release(socket);
    }
    printk(KERN_INFO "Matmoriffer has been deinitialized\n");
}

module_init(driver_initialization);
module_exit(driver_deinitialization);

MODULE_INFO(name,MODULE_NAME);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michal Switala Swistusmen");
MODULE_DESCRIPTION("Module for controling network privacy on your PC");
MODULE_VERSION("0.1"); 