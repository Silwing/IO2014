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
        InMemoryStorage<int, 2, 4> storage;
        //testSiftUpToRoot(&storage);
        //testSiftUpToNode(&storage);
        //testUnalignedLastPage(&storage);
        //testRebalanceLeafCase2(&storage);
        int size = 300;
        for(int i = 2; i < size; i++) {
            try {
                if(i == 114) {

                }
                testSorting(&storage, i);
            } catch(Exception* e) {
                printf("Sorting %d numbers failed with exception of type %s: %s\n", i, e->getType(), e->getMsg());
            }
        }
        printf("Sorted up to %d numbers.", size);

	/*} catch (Exception e) {
		printf("Exception thrown of type %s\n", e.getType());
		printf("  %s\n", e.getMsg());
	} catch (Exception* e) {
		printf("Exception thrown of type %s\n", e->getType());
		printf("  %s\n", e->getMsg());
		delete e;
	}*/
}
