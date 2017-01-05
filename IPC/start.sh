#!/bin/bash

make clean
make

gnome-terminal -e ./IPC
gnome-terminal -e ./AFF
gnome-terminal -e ./AGE
gnome-terminal -e ./CLT
