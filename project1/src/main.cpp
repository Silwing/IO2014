#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "streamtests.hpp"
#include <iostream>
#include <cstdio>


#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

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
    bool t = true; 
    /* SingleStreams */
	SingleItemOutputStream<int> out("data/foo");
	SingleItemInputStream<int> in("data/foo");
	//test(&in, &out, "SingleItems");
    
    /* FStreams */
    FInputStream<int> fin("data/foo");
    FOutputStream<int> fout("data/foo");
    //test(&fin, &fout, "FStreams");

    /* BufferedStreams */
    BufferedInputStream<int> bin("data/foo", 512);
    BufferedOutputStream<int> bout("data/foo", 1024);
    //test(&bin, &bout, "BStreams");

	/* MMappedStreams */
	MMappedInputStream<int> min("data/foo", 1024);

    /* Combos */
    test(&min, &out, "Stream -> MMappedStream");
    
    if (t) {
        start(); 
    }

    /**
     * test(&in, &fout, "Streams -> FStream");
     * test(&fin, &out, "FStreams -> Streams");
     * test(&bin, &fout, "FStreams -> BufferedStreams");
     * test(&fin, &bout, "BufferedStreams -> FStream");
     **/
}
