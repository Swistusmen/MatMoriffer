#!/bin/bash

destination="/usr/local/netshield"
if [[ -d $destination ]] then
    return
else
    sudo mkdir $destination
    sudo cp $(pwd)/../NetShield $destination/NetShield
    export PATH=$PATH:/usr/local/netshield
    source ~/.bashrc
fi

application_shortcut=$HOME/Desktop
if [[ -d "$HOME/Desktop" ]] then
    sudo mkdir $HOME/.local/share/applications/netshield
    echo "[Desktop Entry]" > $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Name=NetShield" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Exec=/usr/local/netshield/NetShield" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    echo "Type=Application" >> $HOME/.local/share/applications/netshield/NetShield.desktop
    sudo update-desktop-database
    if [[ ! -e "$HOME/Desktop/NetShield.desktop "]] then
        sudo cp $HOME/.local/share/applications/netshield/NetShield.desktop $HOME/Desktop/NetShield.desktop
        gio set $HOME/Desktop/NetShield.desktop "metadata::trusted" true
    fi
fi

echo "Installing NetShield has finished with success!"