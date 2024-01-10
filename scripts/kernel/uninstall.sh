#!/bin/bash

sudo rmmod matmoriffer
sudo rm -rf "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer"
sudo rm -rf "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer/matmoriffer.ko"
sudo rm -rf "/etc/modules-load.d/matmorrifer.conf"
sudo depmod -a