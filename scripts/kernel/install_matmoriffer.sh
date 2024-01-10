#!/bin/bash

sudo make -C ../../kernel/
if [ ! -d "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer" ]; then
    sudo mkdir "/lib/modules/$(uname -r)/kernel/drivers/matmoriffer"
fi 

module_in_destination=/lib/modules/$(uname -r)/kernel/drivers/matmoriffer/matmoriffer.ko
if [ ! -e $module_in_destination ]; then
    sudo cp ../../kernel/source/matmoriffer.ko $module_in_destination
else
    echo "Matmoriffer is present, updating"
    sudo rmmod matmoriffer
    sudo rm $module_in_destination
    sudo cp ../../kernel/source/matmoriffer.ko $module_in_destination
fi

if [ ! -d "/etc/modules-load.d/matmorrifer.conf" ]; then
    echo "matmoriffer" >> "/etc/modules-load.d/matmorrifer.conf"
fi 

sudo depmod -a
sudo insmod $module_in_destination