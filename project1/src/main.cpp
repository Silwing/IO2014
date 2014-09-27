#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "DWayMergeSorter.hpp"
#include "streamtests.hpp"
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
            printf("%s failed with endOfStream at %d\n", testName.c_str(), i);
            return;
        }
		int j = in->readNext();
        if (i != j) {
            printf("%s failed at element %d. Got %d\n", testName.c_str(), i, j);
            return;
        }
    }
    if (!in->endOfStream()) {
        printf("%s failed at endOfStream\n", testName.c_str());
    } else {
        printf("%s succeed\n", testName.c_str());
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

class SimpleFactory : public StreamFactory {
	public:
		AbstractInputStream<int>* getInputStream(string path) {
			return new BufferedInputStream<int>(path, 1024);
		}
		
		AbstractOutputStream<int>* getOutputStream(string path) {
			return new BufferedOutputStream<int>(path, 1024);
		}
};

void testSorting(int n, int b, int m, int d) {
	BufferedOutputStream<int> out("data/unsorted", b);
	out.create();
	for (int i = 0; i < n; i++) {
		out.write(rand() % n);
	}
	out.close();
	
	ExternalMergeSort sorter("data/unsorted", new SimpleFactory(), n, m, d);
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
    
void experiment(int n, int k, int b){
    ofstream results; 
    results.open("results.dat", ios::trunc);
    results << "type\tn\tk\tb\tout\tstin" << endl;
    results.close(); 
    for (int i = 1; i<=k; i=i*2) {
        for (int j = 1024; j<=b; j=j*2) {
            start(i, n, j); 
        }
    }
}

int main(int argc, char** argv) {
	char c;
	struct Options opt;
	bool info = false;
	while((c = getopt(argc, argv, "t:e:n:b:k:m:d:i")) != -1) {
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
		case 'i':
			info = true;
			break;
		case '?':
		case ':':
			break;
		}
	}
	
	if (info) {
		printf("Options:\n");
		printf("  n    : %d\n", opt.n);
		printf("  m    : %d\n", opt.m);
		printf("  k (d): %d\n", opt.k);
		printf("  b    : %d\n", opt.b);
	}

	
	for (vector<const char*>::iterator it = opt.test_funcs.begin(); it != opt.test_funcs.end(); it++) {
		const char* arg = *it;
		if (strcmp(arg, "streams") == 0) {
			testStreams(opt.n, opt.b);
		} else if (strcmp(arg, "sort") == 0) {
			testSorting(opt.n, opt.b, opt.m, opt.k);
		} else {
			printf("Unknown test function: %s\n", arg);
		}
	}
	
	for (vector<const char*>::iterator it = opt.expe_funcs.begin(); it != opt.expe_funcs.end(); it++) {
		const char* arg = *it;
		if (strcmp(arg, "streams") == 0) {
			experiment(opt.n, opt.k, opt.b);
		} else {
			printf("Unknown experiment function: %s\n", arg);
		}
	}
}
