#include <cstdio>
#include <string>
#include "SimpleStorage.hpp"
#include "Exceptions.hpp"
#include "Node.hpp"
#include "ExternalHeap.hpp"
#include "tests.hpp"


#define PAGESIZE 2
#define MEMORY 4
#define FANOUT MEMORY/PAGESIZE




int main(int argc, char** argv) {
	try {
        SimpleStorage<int, 2, 4> storage("data");
        testSiftUpToRoot(&storage);
        testSiftUpToNode(&storage);
        SimpleStorage<int, 2, 4> storage2("data2");
        testRebalanceLeafCase2(&storage2);
        testSorting(&storage);
	} catch (Exception e) {
		printf("Exception thrown of type %s\n", e.getType());
		printf("  %s\n", e.getMsg());
	} catch (Exception* e) {
		printf("Exception thrown of type %s\n", e->getType());
		printf("  %s\n", e->getMsg());
		delete e;
	}
}
