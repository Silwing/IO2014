#! /bin/bash
rm -f $1
M=$((1024 * 1024 * 1)) #1mb
#buffer: starts at page size (1024=4kb). As we only have 4mb, buffers of size 16384 (128kb) takes up most of memory
for b in 1024 4096 16384
do
	#streams: 128 streams of 128 kb buffers: 16mb
	for d in 2 8 32
	do
		# memory; since we have a 4mb memory, we start at 1mb (m = 128 consumption)
		# experiments shows that it is okay to overshoot the memory. 4mb = 1M elements -> 256M elements = 1gb
		for m in 16384 65536 262144 1048576 4194304
		do
			time ./main -s bs -e external -b $b -m $m -d $d -i data/unsorted -o data/sorted -I >> $1
			i=$(($i + 1))
			echo $i
		done
	done
done
