#include "SimpleStorage.hpp"
#include "Node.hpp"
#include "ExternalHeap.hpp"
#include <cstdio>

#define PAGESIZE 2
#define MEMORY 6
#define FANOUT MEMORY/PAGESIZE


void printNode(Node<int, FANOUT> node, AbstractStorage<int, PAGESIZE, FANOUT>* storage) {
	//printf("\"node%d\" [\n label = \"Node %d | %s \"\n];\n", node.getId(), node.getId(), "here goes buffer");
}

int main(int argc, char** argv) {
	SimpleStorage<int, PAGESIZE, FANOUT> storage("data");
	ExternalHeap<int, PAGESIZE, FANOUT> heap(&storage);
	
	for (int i = 0; i < 6 * PAGESIZE * FANOUT; i++) {
		heap.insert(rand() % 10);
		fprintf(stderr, "yeah %d/%d\n", i+1, 6*PAGESIZE * FANOUT);
	}
	
	//printf("digraph g {\n");
	//printNode(storage.readNode(0), &storage);
	//printf("}\n");
}
