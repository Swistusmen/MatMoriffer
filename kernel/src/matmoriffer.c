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

#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/netlink.h>

#define MODULE_NAME "matmoriffer"
#define BUFFSIZE 100

int counter=0;

/*
Tested and developed for kernel 6.2 Ubuntu 22.04
playing with kernel 6.2
https://medium.com/powerof2/sequence-file-interface-in-kernel-8967d749f57d
*/

/*
1.Napisanie prostego netlinkowego socketu po stronie kernela i po stronie userspacu-maja miec mozliwosc komunikacji
2.Zarejestrowanie wlasnego urzadzenia sieciowego
3.Ogarniecie zeby to urzadzenie robilo dokladnie to samo co robi wireshark (opisane w kernel networkprogramming introduction)


Pozniej jesli bedzie czas:
1. Napisanie klienta w QT ktory to bedzie handlowal wszystko
*/

static struct proc_dir_entry* entry;
static struct mutex ioctl_mutex;
static struct sock* socket;

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
    seq_printf(m, "Hello in proc file %d\n", counter);
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
        case TEST_INCREMENTAL:{
            counter++;
            break;
        }default:{

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

static int __init driver_initialization(void){
    printk(KERN_INFO "Hello, World\n");
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

    return 0;
}

static void driver_deinitialization(void){
    proc_remove(entry);
    misc_deregister(&control_device);
    if(socket){
        netlink_kernel_release(socket);
    }
    printk(KERN_INFO "Goodbye world!\n");
}

module_init(driver_initialization);
module_exit(driver_deinitialization);

MODULE_INFO(name,MODULE_NAME);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michal Switala Swistusmen");
MODULE_DESCRIPTION("Just reactive module");
MODULE_VERSION("0.1"); 