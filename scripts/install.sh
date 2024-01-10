#!/bin/bash

echo "Building and install matmoriffer"
sudo sh kernel/install_matmoriffer.sh
echo "Installing dependencies"
sudo sh netshield/install_dependencies.sh
echo "Building NetShield"
sudo sh netshield/netshield_building.sh
echo "Installing NetShield"
sudo sh netshield/netshield_install.sh
