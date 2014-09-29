#!/bin/bash

rm -f $2
for k in 1 2 4 8 16 32 64 128 256 512
do
	for i in {1..100}
	do
		./main -e streams -s $1 -k $k -b 1024 -n 10000000 >> $2
	done
done
