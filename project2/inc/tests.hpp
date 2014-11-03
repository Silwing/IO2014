#include "AbstractStorage.hpp"
#include "ExternalHeap.hpp"
#include "Exceptions.hpp"
#include <vector>

template<int P, int m>
void printNode(Node<int, m> node, AbstractStorage<int, P, m>* storage) {
    int buf[P*m];
    storage->readBlock(node, buf);
    string buffer = to_string(buf[0]);
    for (int i = 1; i < P*m; i++) {
        buffer += ", " + to_string(buf[i]);
    }
    
    printf("\"%d\" [\n  label = \"Node %d | %s \"\n  shape=\"record\"\n];\n", node.getId(), node.getId(), buffer.c_str());
    
    for (int i = 0; i < m; i++) {
        if (node.getSizeOf(i) != 0) {
            Node<int, m> child = storage->readNode(node.getChild(i));
            printf("\"%d\" -> \"%d\"\n", node.getId(), child.getId());
            printNode(child, storage);
        }
    }
}

template<int P, int m>
void test(AbstractStorage<int, P, m>* storage, vector<int> input, vector<int> output, const char* msg) {
    ExternalHeap<int, P, m> heap(storage);
    for (int i = 0; i < input.size(); i++) {
        heap.insert(input[i]);
    }
    
    printNode(storage->readNode(0), storage);
    
    for (int i = 0; i < output.size(); i++) {
        int j = heap.deleteMax();
        if (j == output[i]) continue;
        printNode(storage->readNode(0), storage);
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
    
    test(storage, input, output, "testSiftUpToRoot");
}

template<int P, int m>
void testSiftUpToNode(AbstractStorage<int, P, m>* storage) {
    ExternalHeap<int, P, m> heap(storage);
    
    vector<int> input;
    for (int i = 0; i < P * m * (m + 2); i++)
        input.push_back(i);
        
    vector<int> output(input.begin(), input.end());
    reverse(output.begin(), output.end());
        
    test(storage, input, output, "testSiftUpToNode");
    
}