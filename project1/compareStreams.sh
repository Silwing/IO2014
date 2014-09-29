rm -f *_$1
for i in 100 1000 10000 100000 1000000 10000000 100000000
do
	for j in {1..100}
	do
#		./main -e streams -s fs -k 4 -n $i >> "fs_$1"
#		./main -e streams -s bs -k 4 -b 1024 -n $i >> "bs_$1"
		./main -e streams -s mm -k 4 -b 8192 -n $i >> "mm_$1"
	done
done
