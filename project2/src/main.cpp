#include <cstdio>
#include <string>
#include "InMemoryStorage.hpp"
#include "Exceptions.hpp"
#include "Node.hpp"
#include "ExternalHeap.hpp"
#include "tests.hpp"


#define PAGESIZE 2
#define MEMORY 4
#define FANOUT MEMORY/PAGESIZE


int main(int argc, char** argv) {
	//try {
        InMemoryStorage<int, 4, 2> storage;
        testSiftUpToRoot(&storage);
        testSiftUpToNode(&storage);
        
        InMemoryStorage<int, 2, 4> storage2;
        testRebalanceLeafCase2(&storage2);
        testSorting(&storage);
	/*} catch (Exception e) {
		printf("Exception thrown of type %s\n", e.getType());
		printf("  %s\n", e.getMsg());
	} catch (Exception* e) {
		printf("Exception thrown of type %s\n", e->getType());
		printf("  %s\n", e->getMsg());
		delete e;
	}*/
}
