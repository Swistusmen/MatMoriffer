#!/bin/bash

sudo rm -rf /usr/local/netshield
if [ -d "$HOME/Desktop" ]; then
    sudo rm -rf $HOME/.local/share/applications/netshield
    sudo rm -rf $HOME/Desktop/NetShield.desktop
fi
export PATH=$(echo $PATH | sed -e 's/:\/usr\/local\/netshield//')

source ~/.bashrc

echo "succesfully uninstalled netshield"