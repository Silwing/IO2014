#pragma once
#include "AbstractStorage.hpp"
#include "Exceptions.hpp"
#include <vector>

using namespace std;

template<typename E, int P, int m>
class InMemoryStorage : public AbstractStorage<E, P, m> {
    
private:
    vector<Node<E, m> > nodes;
    vector<vector<E> > blocks;
    
public:
    void writeNode(Node<E, m>& node) {
        if (nodes.size() < node.getId()) throw new IllegalStateException("Cannot add node n before node n + 1");
        if (nodes.size() == node.getId()) {
            nodes.push_back(node);
            E page[P*m];
            blocks.push_back(vector<E>(begin(page), end(page)));
        }
        else nodes[node.getId()] = node;
    }
    
    Node<E, m> readNode(unsigned int nodeId) {
        if (nodes.size() <= nodeId) throw new IllegalStateException("Node not created yet");
        return nodes[nodeId];
    }
    
    void readBlock(unsigned int nodeId, E* dest) {
        if (nodes.size() <= nodeId) throw new IllegalStateException("Node not created yet");
        for (int i = 0; i < P * m; i++)
            dest[i] = blocks[nodeId][i];
    }
    
    void writeBlock(unsigned int nodeId, E* src) {
        while (blocks.size() <= nodeId){
            E page[P*m];
            blocks.push_back(vector<E>(begin(page), end(page)));
        }
        for (int i = 0; i < P * m; i++)
            blocks[nodeId][i] = src[i];
    }
    
    void readPage(unsigned int nodeId, unsigned int offset, E* dest) {
        if (nodes.size() <= nodeId) throw new IllegalStateException("Node not created yet");
        for (int i = 0; i < P; i++) {
            dest[i] = blocks[nodeId][offset*P + i];
        }
    }
    
    void writePage(unsigned int nodeId, unsigned int offset, E* src) {
        while (blocks.size() <= nodeId) {
            E page[P*m];
            blocks.push_back(vector<E>(begin(page), end(page)));
        }
        for (int i = 0; i < P; i++) {
            blocks[nodeId][offset*P + i] = src[i];
        }
    }
};