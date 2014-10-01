#!/bin/bash
sudo cgcreate -t afk:afk -g memory,cpu:memlimit &&
echo 1M | sudo tee /sys/fs/cgroup/memory/memlimit/memory.limit_in_bytes &&
cgexec -g memory,cpu:memlimit "$@"
sudo cgdelete memory,cpu:memlimit
