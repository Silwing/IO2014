#!/bin/bash
sudo cgcreate -t afk:afk -g memory,cpu:memlimit &&
echo 4M | sudo tee /sys/fs/cgroup/memory/memlimit/memory.limit_in_bytes &&
cgexec -g memory,cpu:memlimit bash
sudo cgdelete memory,cpu:memlimit
