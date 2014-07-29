#!/bin/bash

insmod /home/dagor/ik220-driver/kdriver/ik220.ko
mknod /dev/ik220 c 61 0
