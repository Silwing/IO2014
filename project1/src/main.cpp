#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "DWayMergeSorter.hpp"
#include <iostream>
#include <cstdio>
#include <vector>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

using namespace std;

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName) {
    int zize = 2048;
    out->create();
	for (int i = 0; i < zize; i++) {
        out->write(i);
    }
    out->close();
    
	in->open();
    for (int i = 0; i < zize; i++) {
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
	vector<AbstractInputStream<int>* > ins;
	string files[5] =  {"data/file_1", "data/file_2", "data/file_3", "data/file_4", "data/file_5"};
	for (int i = 0; i < 5; i++) {
		SingleItemOutputStream<int> out(files[i]);
		out.create();
		for (int j = 0; j < 5; j++) {
			out.write(i + j);
		}
		out.close();
		
		SingleItemInputStream<int>* in = new SingleItemInputStream<int>(files[i]);
		in->open();
		ins.push_back(in);
	}
	
	SingleItemOutputStream<int> out("data/result");
	DWayMergeSorter<int> sorter(ins, &out);
	out.create();
	sorter.merge();
	out.close();
	
	SingleItemInputStream<int> in("data/result");
	in.open();
	while(!in.endOfStream()) {
		printf("%i\n", in.readNext());
	};
	
	
	/*
    // SingleStreams
	test(&in, &out, "SingleItems");
    
    // FStreams
    FInputStream<int> fin("data/foo");
    FOutputStream<int> fout("data/foo");
    test(&fin, &fout, "FStreams");

    // BufferedStreams
    BufferedInputStream<int> bin("data/foo", 512);
    BufferedOutputStream<int> bout("data/foo", 1024);
    test(&bin, &bout, "BStreams");

	// MMappedStreams
	MMappedOutputStream<int> mout("data/foo", 1024);
	MMappedInputStream<int> min("data/foo", 1024);
	test(&min, &mout, "MMappedStreams");

    // Combos
    test(&in, &fout, "Streams -> FStream");
    test(&fin, &out, "FStreams -> Streams");
    test(&bin, &fout, "FStreams -> BufferedStreams");
    test(&fin, &bout, "BufferedStreams -> FStream");
    test(&bin, &mout, "BufferedStreams -> MMappedStreams");
    test(&min, &bout, "MMappedStreams -> BufferedOut");
    
	*/
}
