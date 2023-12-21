#!/bin/bash

sudo cmake configure $(pwd)/../../NetShield 
sudo cmake --build $(pwd)/../../NetShield --target all


