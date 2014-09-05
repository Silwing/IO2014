#include "SingleItemStreams.h"
#include "FStreams.h"
#include "BufferedStreams.h"
#include <iostream>
#include <cstdio>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName) {
    int zize = 1000000;
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
        if (i != in->readNext()) {
            printf("%s failed at element %d\n", testName.c_str(), i);
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
    /* SingleStreams */
	SingleItemOutputStream<int> out("data/foo");
	SingleItemInputStream<int> in("data/foo");
    test(&in, &out, "SingleItems");
    
    /* FStreams */
    FInputStream<int> fin("data/foo");
    FOutputStream<int> fout("data/foo");
    test(&fin, &fout, "FStreams");

    /* BufferedStreams */
    BufferedInputStream<int> bin("data/foo", 512);
    BufferedOutputStream<int> bout("data/foo", 1024);
    test(&bin, &bout, "BStreams");


    /* Combos */
    test(&in, &fout, "Streams -> FStream");
    test(&fin, &out, "FStreams -> Streams");
    test(&bin, &fout, "FStreams -> BufferedStreams");
    test(&fin, &bout, "BufferedStreams -> FStream");
}
