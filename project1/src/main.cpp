#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "DWayMergeSorter.hpp"
#include "streamtests.hpp"
#include <iostream>
#include <cstdio>
#include <vector>


#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

using namespace std;

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName) {
    int size = 1024 * 1024 * 256;
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

int main(int argc, char** argv) {
    bool t = false;
    /*
	vector<AbstractInputStream<int>* > ins;
	string files[5] =  {"data/file_1", "data/file_2", "data/file_3", "data/file_4", "data/file_5"};
	for (int i = 0; i < 5; i++) {
		MMappedOutputStream<int> out(files[i], 1024);
		out.create();
		for (int j = 0; j < 5; j++) {
			out.write(i + j);
		}
		out.close();
		
		BufferedInputStream<int>* in = new BufferedInputStream<int>(files[i], 256);
		in->open();
		ins.push_back(in);
	}
	
	BufferedOutputStream<int> out("data/result", 64);
	DWayMergeSorter<int> sorter(ins, &out);
	out.create();
	sorter.merge();
	out.close();
	
	FInputStream<int> in("data/result");
	in.open();
	while(!in.endOfStream()) {
		printf("%i\n", in.readNext());
	};
	*/
	
	/*
    // SingleStreams 
	SingleItemOutputStream<int> out("data/foo");
	SingleItemInputStream<int> in("data/foo");
	test(&in, &out, "SingleItems");
    */
    
    // FStreams
    FInputStream<int> fin("data/foo");
    FOutputStream<int> fout("data/foo");
    //test(&fin, &fout, "FStreams");

    // BufferedStreams
    BufferedInputStream<int> bin("data/foo", 1024);
    BufferedOutputStream<int> bout("data/foo", 1024);
    //test(&bin, &bout, "BStreams");

	// MMappedStreams
	MMappedOutputStream<int> mout("data/foo", 1024);
	MMappedInputStream<int> min("data/foo", 1024);
	test(&min, &mout, "MMappedStreams");
    // Combos
    //test(&in, &fout, "Streams -> FStream");
    //test(&fin, &out, "FStreams -> Streams");
    /*
    test(&bin, &fout, "FStreams -> BufferedStreams");
    test(&fin, &bout, "BufferedStreams -> FStream");
    test(&bin, &mout, "BufferedStreams -> MMappedStreams");
    test(&min, &bout, "MMappedStreams -> BufferedOut");
	*/
    if (t) {
        start(); 
    }
    
}
