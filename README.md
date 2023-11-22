# MatMoriffer

## Target:
Create an application which combines kernel level network tracking with Qt charts in order to let you see to whom you are sending packets, from who do you get them. Potentially I would like you to be able to block sending/ receiving packages

## Demands
Kernel module has been tested on Ubuntu 22.04 kernel version 6.2

## Compilation
```
cd kernel
sudo make
cd src
sudo insmod matmoriffer
```

## Usage
vateoctl is an application which enables you to test kernel module (currently I am rewriting it to receive messages over netlink sockets-so at the time when I'm writing that you won't be able to see any messages- to achieve that w8 or put printk in proper places)
