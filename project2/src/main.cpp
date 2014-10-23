#include <cstdio>
#include <string>
#include "SimpleStorage.hpp"
#include "Exceptions.hpp"
#include "Node.hpp"
#include "ExternalHeap.hpp"


#define PAGESIZE 2
#define MEMORY 8
#define FANOUT MEMORY/PAGESIZE


void printNode(Node<int, FANOUT> node, AbstractStorage<int, PAGESIZE, FANOUT>* storage) {
	int buf[FANOUT*PAGESIZE];
	storage->readBlock(node, &buf[0]);
	string buffer = to_string(buf[0]);
	for (int i = 1; i < FANOUT*PAGESIZE; i++) {
		buffer += ", " + to_string(buf[i]);
	}
	
	printf("\"%d\" [\n  label = \"Node %d | %s \"\n  shape=\"record\"\n];\n", node.getId(), node.getId(), buffer.c_str());
	
	for (int i = 0; i < FANOUT; i++) {
		if (node.getSizeOf(i) != 0) {
			Node<int, FANOUT> child = storage->readNode(node.getChild(i));
			printf("\"%d\" -> \"%d\"\n", node.getId(), child.getId());
			printNode(child, storage);
		}
	}
}

int main(int argc, char** argv) {
	try {
		SimpleStorage<int, PAGESIZE, FANOUT> storage("data");
		ExternalHeap<int, PAGESIZE, FANOUT> heap(&storage);
		
		for (int i = 0; i < 50 * PAGESIZE * FANOUT; i++) {
			heap.insert(rand() % 100);
		}
		
		printf("digraph g {\n");
		printNode(storage.readNode(0), &storage);
		printf("}\n");
	} catch (Exception e) {
		printf("Exception thrown of type %s\n", e.getType());
		printf("  %s\n", e.getMsg());
	} catch (Exception* e) {
		printf("Exception thrown of type %s\n", e->getType());
		printf("  %s\n", e->getMsg());
		delete e;
	}
}
