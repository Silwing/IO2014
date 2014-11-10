#include <cstdio>
#include <string>
#include "MMappedStorage.hpp"
#include "Exceptions.hpp"
#include "Node.hpp"
#include "ExternalHeap.hpp"
#include "tests.hpp"


#define PAGESIZE 2
#define MEMORY 4
#define FANOUT MEMORY/PAGESIZE


int main(int argc, char** argv) {
	try {
        MMappedStorage<int, 1024, 255> storage("data");
        storage.writePage(0, 0, &src);
        //testSiftUpToRoot(&storage);
        //testSiftUpToNode(&storage);
        //testUnalignedLastPage(&storage);
        //testRebalanceLeafCase2(&storage);
        //testSorting(&storage);
	} catch (Exception e) {
		printf("Exception thrown of type %s\n", e.getType());
		printf("  %s\n", e.getMsg());
	} catch (Exception* e) {
		printf("Exception thrown of type %s\n", e->getType());
		printf("  %s\n", e->getMsg());
		delete e;
	}
}
