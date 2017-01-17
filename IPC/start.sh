#!/bin/bash

make clean
make

gnome-terminal -e ./ECRTIR
gnome-terminal -e ./AFF
gnome-terminal -e ./AGE
gnome-terminal -e ./CLT

./CLT
