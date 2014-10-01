#! /bin/bash

for n in 16 32 64 128 256 512 1024
do
	n=$(($n * 1024))
	time ./main -s bs -e quick -i "data/very_big_unsorted" -o "data/very_big_sorted" -I -n $n >> $1
done

for n in 16 32 64 128 256 512 1024
do
	n=$(($n * 1024))
	time ./main -s bs -e heap -i "data/very_big_unsorted" -o "data/very_big_sorted" -I -n $n >> $1
done
