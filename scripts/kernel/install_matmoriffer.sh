#!/bin/bash

sudo make -C ../../kernel/
if [[ ! -d "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer" ]] then
    sudo mkdir "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer"
fi 

sudo cp ../../kernel/source/matmoriffer.ko /lib/modules/$(uname -r)/kernel/drivers/matmoriffer/matmoriffer.ko

if [[ ! -d "/etc/modules-load.d/matmorrifer.conf" ]] then
    echo "matmoriffer" >> "/etc/modules-load.d/matmorrifer.conf"
fi 

sudo depmod -a
sudo insmod ../../kernel/source/matmoriffer.ko