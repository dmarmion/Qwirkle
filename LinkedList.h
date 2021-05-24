#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

    LinkedList();
    LinkedList(LinkedList& other);
    ~LinkedList();

    // No-argument overload maintained for backwards compatibility, assumes
    // colour is not desired
    std::string toString();
    std::string toString(bool useColour);

    // we won't need addFront or insert, but do them for practise
    void addFront(Tile* tile);
    void addBack(Tile* tile);

    void removeFront();
    void removeBack();
    void remove(int index);
    void clear();
    bool contains(Tile* t);

    Tile* getFront();
    Tile* getBack();
    Tile* get(int index);

    int findTileIndex(Tile* tile);
    int getSize();

    bool findAndRemove(Tile* tile);

private:
    Node* head;
    Node* tail;
    int size;
};

#endif // ASSIGN2_LINKEDLIST_H
