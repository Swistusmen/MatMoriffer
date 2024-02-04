#!/bin/bash

destination="/usr/local/netshield"
if [ -d $destination ]; then
    echo "NetShield is already installed- updating"
    sudo rm $destination/NetShield
    sudo cp $(pwd)/../NetShield $destination/NetShield
else
    sudo mkdir $destination
    sudo cp $(pwd)/../NetShield $destination/NetShield
    export PATH=$PATH:/usr/local/netshield
    sudo source ~/.bashrc
fi

if [ ! -d "/etc/netshield" ]; then
    echo "NetShield is already installed- updating"
    sudo mkdir "/etc/netshield"
    sudo cp $(pwd)/../configuration/config.json "/etc/netshield/config.json"
else
    sudo rm "/etc/netshield/config.json"
    sudo cp $(pwd)/../configuration/config.json "/etc/netshield/config.json"
fi

application_shortcut=$HOME/Desktop
if [ -d "$HOME/Desktop" ]; then
    sudo mkdir $HOME/.local/share/applications/netshield
    echo "[Desktop Entry]" > $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Name=NetShield" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Exec=/usr/local/netshield/NetShield" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Type=Application" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    sudo update-desktop-database
    if [ ! -e "$HOME/Desktop/NetShield.desktop "]; then
        sudo cp $HOME/.local/share/applications/netshield/NetShield.desktop $HOME/Desktop/NetShield.desktop
        gio set $HOME/Desktop/NetShield.desktop "metadata::trusted" true
    fi
fi

echo "Installing NetShield has finished with success!"