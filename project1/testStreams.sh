#!/bin/bash

rm -f $2
for B in 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304
do
	for i in {1..100}
	do
		./main -e streams -s $1 -k 1 -b $B -n 10000000 >> $2
	done
done
