#include "SingleItemStreams.h"
#include <iostream>
#include <cstdio>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName) {
    int zize = 100;
    out->create();
	for (int i = 0; i < zize; i++) {
        out->write(i);
    }
    out->close();

	in->open();
    for (int i = 0; i < zize; i++) {
        if (i != in->readNext()) {
            printf("%s failed at element %d\n", testName.c_str(), i);
            return;
        }
    }
    if (!in->endOfStream()) {
        printf("%s failed at endOfFile\n", testName.c_str());
    } else {
        printf("%s succeed\n", testName.c_str());
    }
}

int main(int argc, char** argv) {
	SingleItemOutputStream<int> out("data/foo");
	SingleItemInputStream<int> in("data/foo");

    test(&in, &out, "SingleItems");
}
