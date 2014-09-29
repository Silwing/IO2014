#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "DWayMergeSorter.hpp"
#include "streamtests.hpp"
#include "tests/sortingtest.hpp"
#include "Timer.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include "QuickSort.hpp"
#include "ExternalMergeSort.hpp"
#include "HeapSort.hpp"
#include <fstream>
#include <sys/resource.h>
#include <unistd.h>
#include <cstring>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

using namespace std;

struct Options {
	vector<const char*> test_funcs;
	vector<const char*> expe_funcs;
	int n = 1024 * 1024; 	// file size
	int k = 128; 			// number of files open at once also known as d
	int b = 1024;			// block size / buffer size
	int m = 1024;			// memory size
	const char* stream = "bs"; // Which streams to use: si (SingleItemStreams), fi (FStreams), bs (BufferedStreams), mm (MMappedStreams)
	const char* input = "input.txt";
	const char* output = "output.txt";
};

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName, int size) {
    out->create();
	for (int i = 0; i < size; i++) {
        out->write(i);
    }
    out->close();
    
	in->open();
    for (int i = 0; i < size; i++) {
        if (in->endOfStream()) {
            fprintf(stderr, "%s failed with endOfStream at %d\n", testName.c_str(), i);
            return;
        }
		int j = in->readNext();
        if (i != j) {
            fprintf(stderr, "%s failed at element %d. Got %d\n", testName.c_str(), i, j);
            return;
        }
    }
    if (!in->endOfStream()) {
        fprintf(stderr, "%s failed at endOfStream\n", testName.c_str());
    } else {
        fprintf(stderr, "%s succeed\n", testName.c_str());
    }
}

void testStreams(int n, int b) {
	SingleItemInputStream<int> sin("data/si");
	SingleItemOutputStream<int> sout("data/si");
	test(&sin, &sout, "SingleItemStreams", n);
	
	FInputStream<int> fin("data/fs");
	FOutputStream<int> fout("data/fs");
	test(&fin, &fout, "FStreams", n);
	
	BufferedInputStream<int> bin("data/bs", b);
	BufferedOutputStream<int> bout("data/bs", b);
	test(&bin, &bout, "BufferedInputStreams", n);
	
	if (b % 1024 == 0) {
		MMappedInputStream<int> min("data/mm", b);
		MMappedOutputStream<int> mout("data/mm", b);
		test(&min, &mout, "MMappedStreams", n);		
	} else {
		printf("Buffersize %d not suitable for MMappedStreams\n", b);
	}
	
	test(&sin, &fout, "SStream -> FStream", n);
	test(&fin, &bout, "FStream -> BStream", n);
	test(&bin, &sout, "BStream -> SStream", n);
}

void testSorting(int n, int b, int m, int d) {
	BufferedOutputStream<int> out("data/unsorted", b);
	out.create();
	for (int i = 0; i < n; i++) {
		out.write(rand() % n);
	}
	out.close();
	
	ExternalMergeSort sorter("data/unsorted", new BufferedStreamFactory(1024), n, m, d);
	sorter.sort("data/sorted");
	BufferedInputStream<int> in("data/sorted", b);
	in.open();
	int i = in.readNext();
	while(!in.endOfStream()) {
		int j = in.readNext();
		if (i > j) {
			printf("Bad sorting!!\n");
			return;
		}
		i = j;
	}
	printf("Good sorting\n");
}

int main(int argc, char** argv) {
	char c;
	struct Options opt;
	bool info = false;
	bool verbose = false;
	while((c = getopt(argc, argv, "t:e:n:b:k:m:d:Ivs:o:i:")) != -1) {
		switch (c) {
		case 't': //tests
			opt.test_funcs.push_back(optarg);
			break;
		case 'e': //experiments
			opt.expe_funcs.push_back(optarg);
			break;
		//Params
		case 'n':
			opt.n = atoi(optarg);
			break;
		case 'k':
		case 'd':
			opt.k = atoi(optarg);
			break;
		case 'b':
			opt.b = atoi(optarg);
			break;
		case 'm':
			opt.m = atoi(optarg);
			break;
		case 's':
			opt.stream = optarg;
			break;
		case 'o':
			opt.output = optarg;
			break;
		case 'i':
			opt.input = optarg;
			break;
		//Debugging and stuff
		case 'I':
			info = true;
			break;
		case 'v':
			verbose = true;
			break;
		case '?':
		case ':':
			break;
		}
	}
	
	if (info) {
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "  n    : %d\n", opt.n);
		fprintf(stderr, "  m    : %d\n", opt.m);
		fprintf(stderr, "  k (d): %d\n", opt.k);
		fprintf(stderr, "  b    : %d\n", opt.b);
		fprintf(stderr, "  in   : %s\n", opt.input);
		fprintf(stderr, "  out  : %s\n", opt.output);
	}

	StreamFactory* fac;
	if (strcmp(opt.stream, "si") == 0) fac = new SingleItemStreamFactory();
	else if (strcmp(opt.stream, "fs") == 0) fac = new FStreamFactory();
	else if (strcmp(opt.stream, "bs") == 0) fac = new BufferedStreamFactory(opt.b);
	else if (strcmp(opt.stream, "mm") == 0) fac = new MMappedStreamFactory(opt.b);
	else {
		fprintf(stderr, "Unknown stream chosen %s\n", opt.stream);
		return -1;
	}
	
	if (!verbose) {
		freopen("/dev/null", "w", stderr);
	}
	
	for (vector<const char*>::iterator it = opt.test_funcs.begin(); it != opt.test_funcs.end(); it++) {
		const char* arg = *it;
		if (strcmp(arg, "streams") == 0) {
			testStreams(opt.n, opt.b);
		} else if (strcmp(arg, "sort") == 0) {
			testSorting(opt.n, opt.b, opt.m, opt.k);
		} else {
			fprintf(stderr, "Unknown test function: %s\n", arg);
		}
	}
	
	for (vector<const char*>::iterator it = opt.expe_funcs.begin(); it != opt.expe_funcs.end(); it++) {
		const char* arg = *it;
		if (strcmp(arg, "streams") == 0) {
			experiments::start(opt.k, opt.n, fac);
		} else if (strcmp(arg, "external") == 0) {
			experiments::sort(fac, opt.input, opt.output, opt.m, opt.k);
		} else if (strcmp(arg, "data") == 0) {
			AbstractOutputStream<int>* out = fac->getOutputStream(opt.output);
			out->create();
			for (int i = 0; i < opt.n; i++) {
				out->write(rand());
			}
			out->close();
			delete out;
		} else if (strcmp(arg, "check") == 0) {
			AbstractInputStream<int>* in = fac->getInputStream(opt.input);
			in->open();
			int i = in->readNext();
			while(!in->endOfStream()) {
				int j = in->readNext();
				if (i > j) {
					printf("Not sorted\n");
					return -1;
				}
				i = j;
			}
			printf("Sorted\n");
		} else {
			fprintf(stderr, "Unknown experiment function: %s\n", arg);
		}
	}
}
