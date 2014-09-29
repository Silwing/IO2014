#! /bin/bash
rm -f $1
for i in {1..10}
do
	for d in 2 8 32 128 512
	do
		for m in 1024 4096 16384 65536 262144 1048576 4194304 8388608
		do
			./main -s bs -e external -b 4096 -m $m -d $d -i data/unsorted -o data/sorted >> $1
		done
	done
done
