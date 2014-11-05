#include "AbstractStorage.hpp"
#include "ExternalHeap.hpp"
#include "Exceptions.hpp"
#include <vector>

template<int P, int m>
void printNode(Node<int, m> node, AbstractStorage<int, P, m>* storage, unsigned int size) {
    int buf[P*m];
    storage->readBlock(node, buf);
    string buffer = to_string(buf[0]);
    for (int i = 1; i < size; i++) {
        buffer += ", " + to_string(buf[i]);
    }
    
    printf("\"%d\" [\n  label = \"Node %d | %s \"\n  shape=\"record\"\n];\n", node.getId(), node.getId(), buffer.c_str());
    
    for (int i = 0; i < m; i++) {
        unsigned int s = node.getSizeOf(i);
        if (s > 0) {
            Node<int, m> child = storage->readNode(node.getChild(i));
            printf("\"%d\" -> \"%d\"\n", node.getId(), child.getId());
            printNode(child, storage, s);
        }
    }
}

template<int P, int m>
void test(AbstractStorage<int, P, m>* storage, vector<int> input, vector<int> output, const char* msg) {
    ExternalHeap<int, P, m> heap(storage);
    for (int i = 0; i < input.size(); i++) {
        heap.insert(input[i]);
    }
    
    printNode(storage->readNode(0), storage, heap.getRootSize());
    
    for (int i = 0; i < output.size(); i++) {
        int j = heap.deleteMax();
        if (j == output[i]) continue;
        printNode(storage->readNode(0), storage, heap.getRootSize());
        char* str = new char[100];
        sprintf(str, "Bad test. Got %d, expected %d", j, output[i]);
        throw new Exception(str, msg);
    }
    printf("%s passed\n", msg);
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
    ExternalHeap<int, P, m> heap(storage);
    
    vector<int> input;
    for (int i = 0; i < P * m * (m + 2); i++)
        input.push_back(i);
        
    vector<int> output(input.begin(), input.end());
    reverse(output.begin(), output.end());
        
    test(storage, input, output, "testSiftUpToNode and aligned last page");
    
}

template<int P, int m>
void testSorting(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    int size = 50;
    for (int i = 0; i < size; i++) {
        heap.insert(rand() % 1000000);
    }
    int max = heap.deleteMax();
    for (int i = 1; i < size; i++) {
        int next = heap.deleteMax();
        if (next > max) throw new Exception("BadTestResult", "is not sorted");
        max = next;
    }
    printf("Sorting done. Throwing exception\n");
    heap.deleteMax();
}

template<int P, int m>
void testForUnalignedLastPage(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    for (int i = 0; i < 1000000; i++) {
        int limit = rand() % 10 + 1;
        printf("Inserting %d ints\n", limit);
        for (int j = 0; j < limit; j++) {
            heap.insert(rand() % 10000);
        }
        limit = rand() % limit;
        printf("Deleting %d ints\n", limit);
        for (int j = 0; j < limit; j++) {
            heap.deleteMax();
        }
    }
}

template<int P, int m>
void testUnalignedLastPage(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    
    
    vector<int> input;
    for (int i = 0; i < P * m * (m + 2); i++)
        input.push_back(100-i);
    
    input[P*m*3+4] = 90;
    
    vector<int> output(input.begin(), input.end());
    sort(output.begin(), output.end());
    reverse(output.begin(), output.end());
    
    
    test(storage, input, output, "Unaligned last page");
}