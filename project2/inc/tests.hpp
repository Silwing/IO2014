#pragma once
#include "AbstractStorage.hpp"
#include "ExternalHeap.hpp"
#include "Exceptions.hpp"
#include <cstdio>
#include <vector>

int step;

template<int P, int m>
void printHeapToFile(ExternalHeap<int, P, m>* heap, AbstractStorage<int, P, m>* storage) {
    string path = "graphs/graph_" + to_string(step++) + ".dot";
    printf("Printing to %s\n", path.c_str());
    FILE* file = fopen(path.c_str(), "w+");
    printHeapToFile(file, heap, storage);
    fclose(file);
}

template<int P, int m>
void printHeap(ExternalHeap<int, P, m>* heap, AbstractStorage<int, P, m>* storage) {
    printHeapToFile(stderr, heap, storage);
}


template<int P, int m>
void printHeapToFile(FILE* file, ExternalHeap<int, P, m>* heap, AbstractStorage<int, P, m>* storage) {
    fprintf(file, "digraph H {\n");
    printNodeToFile(file, storage->readNode(0), storage, heap->getRootSize());
    fprintf(file, "}\n");
}

template<int P, int m>
void printNodeToFile(FILE* file, Node<int, m> node, AbstractStorage<int, P, m>* storage, int nodeSize) {
    int buf[P*m];
    storage->readBlock(node, buf);
    string buffer = to_string(buf[0]);
    for (int i = 1; i < nodeSize; i++) {
        buffer += ", " + to_string(buf[i]);
    }

    fprintf(file, "\"%d\" [\n  label = \"Node %d | %s \"\n  shape=\"record\"\n];\n", node.getId(), node.getId(), buffer.c_str());

    for (int i = 0; i < m; i++) {
        unsigned int s = node.getSizeOf(i);
        if (s > 0) {
            Node<int, m> child = storage->readNode(node.getChild(i));
            fprintf(file, "\"%d\" -> \"%d\"\n", node.getId(), child.getId());
            printNodeToFile(file, child, storage, s);
        }
    }
}

template<int P, int m>
void test(AbstractStorage<int, P, m>* storage, vector<int> input, vector<int> output, const char* msg) {
    ExternalHeap<int, P, m> heap(storage);
    for (int i = 0; i < input.size(); i++) {
        heap.insert(input[i]);
    }
    printHeap(&heap, storage);
    fprintf(stderr, "Starting test: %s\n", msg);

    for (int i = 0; i < output.size(); i++) {
        int j = heap.deleteMax();
        if (j == output[i]) continue;
        char* str = new char[100];
        sprintf(str, "Bad test. Got %d, expected %d", j, output[i]);
        throw new Exception(str, msg);
    }
    fprintf(stderr, "%s passed\n", msg);
}

template<int P, int m>
void testSiftUpToRoot(AbstractStorage<int, P, m>* storage) {
    vector<int> input;
    for (int i = 0; i < P * m * 2; i++)
        input.push_back(i);

    vector<int> output(input.begin(), input.end());
    reverse(output.begin(), output.end());
    test(storage, input, output, "testSiftUpToRoot and aligned last page");
}

template<int P, int m>
void testSiftUpToNode(AbstractStorage<int, P, m>* storage) {
    vector<int> input;
    for (int i = 0; i < P * m * (m + 2); i++)
        input.push_back(i);

    vector<int> output(input.begin(), input.end());
    reverse(output.begin(), output.end());

    test(storage, input, output, "testSiftUpToNode and aligned last page");
}

template<int P, int m>
void testRebalanceLeafCase2(AbstractStorage<int, P, m>* storage) {
    vector<int> input { 100, 99, 98, 97, 96, 95, 94, 93,
                         89, 87, 79, 77, 69, 68, 67, 66,
                         19, 18, 17, 16, 15, 14, 13, 12,
                         88, 86, 78, 76, 59, 58, 57, 56};
    vector<int> output {100, 99, 98, 97, 96, 95, 94, 93,
                         89, 88, 87, 86, 79, 78, 77, 76,
                         69, 68, 67, 66, 59, 58, 57, 56,
                         19, 18, 17, 16, 15, 14, 13, 12};

    test(storage, input, output, "testRebalanceLeafCase2");
}

template<int P, int m>
void testSorting(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    int size = 100;
    for (int i = 0; i < size; i++) {
        heap.insert(rand() % 1000);
    }
    int max = heap.deleteMax();
    for (int i = 1; i < size; i++) {
        int next = heap.deleteMax();
        if (next > max) {
            printf("max: %d | next: %d\n", max, next);
            printHeap(&heap, storage);
            throw new Exception("BadTestResult", "is not sorted");
        }
        max = next;
    }
    fprintf(stderr, "Sorting done.\n");
}

template<int P, int m>
void testForUnalignedLastPage(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    for (int i = 0; i < 1000000; i++) {
        int limit = rand() % 10 + 1;
        //printf("Inserting %d ints\n", limit);
        for (int j = 0; j < limit; j++) {
            heap.insert(rand() % 10000);
        }
        limit = rand() % limit;
        //printf("Deleting %d ints\n", limit);
        for (int j = 0; j < limit; j++) {
            heap.deleteMax();
        }
    }
}

void testUnalignedLastPage(AbstractStorage<int, 2, 4>* storage) {
    ExternalHeap<int, 2, 4> heap(storage);


    vector<int> input;
    for (int i = 0; i < 2 * 4 * 4; i++)
        input.push_back(100-i);

    input[2*4*2] = 92;
    input[2*4*3] = 92;

    vector<int> output(input.begin(), input.end());
    sort(output.begin(), output.end());
    reverse(output.begin(), output.end());


    test(storage, input, output, "Unaligned last page");
}
