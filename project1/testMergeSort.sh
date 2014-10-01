#! /bin/bash

# memory: 256k  (1mb)
b=1024  #(4kb)
d=32    # 32 streams when merging -> 132kb buffers (32 + 1) in total
m=65536 # (256kb)

for n in 16 32 64 128 256 512 1024
do
	n=$(($n * 1024))
	echo $n
	time ./main -s bs -e external -b $b -m $m -d $d -i "data/very_big_unsorted" -o "data/very_big_sorted" -I -n $n >> $1
done
