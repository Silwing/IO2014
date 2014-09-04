#include "SingleItemStreams.h"
#include <iostream>
#include <cstdio>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

int main(int argc, char** argv) {

	SingleItemOutputStream<int> out("data/foo");
	out.create();
	for (int i = 0; i < 10; i++) {
		out.write(i);
	}
	out.close();

	SingleItemInputStream<int> in("data/foo");
	in.open();
	while(!in.endOfStream()) {
		printf("%d\n", in.readNext());
	}
}
