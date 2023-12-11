# MatMoriffer

## About

Kernel module which enables to track sent and received packets- it is your local sniffer.

## Demands

It was tested and developed on kernel 6.2 and Ubuntu 22

## Building

```
sudo make
sudo insmod src/matmoriffer.ko
```

In order to interact with module special application has been made- you can find it in app directory

### TODO
- enrich packets data and controll over them through ioctl
- enable to block
- enrich proc information
