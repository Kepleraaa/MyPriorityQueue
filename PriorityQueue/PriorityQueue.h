#pragma once

#include <iostream>
#include <sstream>
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

template <typename KeyType, typename ValueType>
class PriorityQueue // smallest key has highest priority. a min heap implementation
{
public:
    PriorityQueue();
    ~PriorityQueue();

    PriorityQueue(const PriorityQueue<KeyType, ValueType>& queue2);
    PriorityQueue& operator=(const PriorityQueue<KeyType, ValueType>& queue2);

    const ValueType& peek() const;
    ValueType dequeue();
    void enqueue(const KeyType& key, const ValueType& value);
    void adjustPriority(ValueType elem,KeyType newPriority);

    int size() const;
    bool isEmpty() const;
    void clear();

    bool equals(const PriorityQueue<KeyType, ValueType>& queue2) const;
    bool operator==(const PriorityQueue& queue2) const;
    bool operator!=(const PriorityQueue& queue2) const;

    string toString() const;

    template <typename K, typename V>
    friend ostream& operator<<(ostream& os, const PriorityQueue<K, V>& queue);

private:

    struct KVPair
    {
        KeyType key;
        ValueType value;
    };

    KVPair* elements;
    int capacity;
    int numElements;

    static const int INITIAL_CAPACITY = 8;

    void expand();
    void bubbleUp(int i);
    void bubbleDown(int i);
};

template <typename KeyType, typename ValueType>
const int PriorityQueue<KeyType, ValueType>::INITIAL_CAPACITY;

template <typename KeyType, typename ValueType>
PriorityQueue<KeyType, ValueType>::PriorityQueue() {
    elements = new KVPair[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    numElements = 0;
}

template <typename KeyType, typename ValueType>
PriorityQueue<KeyType, ValueType>::~PriorityQueue() {
    delete[] elements;
}

template <typename KeyType, typename ValueType>
PriorityQueue<KeyType, ValueType>::PriorityQueue(const PriorityQueue<KeyType, ValueType>& queue2)
{
    capacity = max(INITIAL_CAPACITY, queue2.numElements) * 2;
    elements = new KVPair[capacity];
    for (int i = 0; i < queue2.numElements; i++) {
        elements[i] = queue2.elements[i];
    }
    numElements = queue2.numElements;
}

template <typename KeyType, typename ValueType>
PriorityQueue<KeyType, ValueType>& PriorityQueue<KeyType, ValueType>::operator=(const PriorityQueue<KeyType, ValueType>& queue2)
{
    delete [] elements;
    capacity = max(INITIAL_CAPACITY, queue2.numElements) * 2;
    elements = new KVPair[capacity];
    for (int i = 0; i < queue2.numElements; i++) {
        elements[i] = queue2.elements[i];
    }
    numElements = queue2.numElements;
    return *this;
}

template <typename KeyType, typename ValueType>
const ValueType& PriorityQueue<KeyType, ValueType>::peek() const {
    if (isEmpty()) {
        error("Queue::peek: Attempting to peek at an empty queue");
    }
    return elements[0].value;
}

template <typename KeyType, typename ValueType>
ValueType PriorityQueue<KeyType, ValueType>::dequeue() {
    if (isEmpty()) {
        error("Queue::dequeue: Attempting to dequeue an empty queue");
    }
    ValueType value = elements[0].value;
    elements[0] = elements[--numElements];
    bubbleDown(0);
    return value;
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::enqueue(const KeyType& key, const ValueType& value) {
    if (numElements == capacity) {
        expand();
    }
    elements[numElements] = KVPair{key, value};
    bubbleUp(numElements);
    ++numElements;
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::adjustPriority(ValueType elem,KeyType newPriority){
    for (int i=0;i<numElements;i++){
        if (elem==elements[i].value){
            elements[i].key=newPriority;
            bubbleDown(i);
            bubbleUp(i);
        }
    }
}

template <typename KeyType, typename ValueType>
int PriorityQueue<KeyType, ValueType>::size() const {
    return numElements;
}

template <typename KeyType, typename ValueType>
bool PriorityQueue<KeyType, ValueType>::isEmpty() const {
    return numElements == 0;
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::clear() {
    numElements = 0;
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::expand() {
    capacity = numElements * 2;
    KVPair* newElements = new KVPair[capacity];
    for (int i = 0; i < numElements; i++) {
        newElements[i] = elements[i];
    }
    delete[] elements;
    elements = newElements;
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::bubbleDown(int i) {
    if (2*i+1 < numElements) {
        int j = (2*i+2 >= numElements || elements[2*i+1].key < elements[2*i+2].key) ? 2 * i + 1 : 2 * i + 2;
        if (elements[j].key < elements[i].key) {
            swap(elements[i], elements[j]);
            bubbleDown(j);
        }
    }
}

template <typename KeyType, typename ValueType>
void PriorityQueue<KeyType, ValueType>::bubbleUp(int i) {
    if (i > 0) {
        int j = (i - 1) / 2;
        if (elements[i].key < elements[j].key) {
            swap(elements[i], elements[j]);
            bubbleUp(j);
        }
    }
}

template <typename KeyType, typename ValueType>
string PriorityQueue<KeyType, ValueType>::toString() const {
    ostringstream os;
    os << *this;
    return os.str();
}

template <typename KeyType, typename ValueType>
ostream& operator<<(ostream& os, const PriorityQueue<KeyType, ValueType>& queue) {
    os << "{";
    for (int i = 0; i < queue.numElements - 1; ++i) {
        os << queue.elements[i].key << ":" << queue.elements[i].value << ", ";
    }
    os << queue.elements[queue.numElements-1].key << ":" << queue.elements[queue.numElements-1].value;
    os << "}";
    return os;
}

PROVIDED_TEST("constructor and destructor test")
{
    PriorityQueue<int, int> queue1;
    queue1.enqueue(1, 1);

    PriorityQueue<double, int> queue2;
    queue2.enqueue(0.01, 1);

    PriorityQueue<int, string> queue3;
    queue3.enqueue(1, "PriorityQueue");

    PriorityQueue<string, PriorityQueue<int, string>> queue4;
    queue4.enqueue("a", PriorityQueue<int, string>());
}

PROVIDED_TEST("empty and size test")
{
    PriorityQueue<int, string> queue;
    EXPECT_EQUAL(queue.isEmpty(), true);
    EXPECT_EQUAL(queue.size(), 0);
}

PROVIDED_TEST("Add 5 elements to queue.")
{
    PriorityQueue<int, string> queue;
    for (int i = 0; i < 5; i++) {
        queue.enqueue(i, string(i, i));
    }

    EXPECT_EQUAL(queue.size(), 5);
    EXPECT_EQUAL(queue.isEmpty(), false);
}

PROVIDED_TEST("Add 50 elements to queue. Tests expansion")
{
    PriorityQueue<double, string> queue;
    for (int i = 0; i < 50; i++) {
        queue.enqueue(i, string(i, i));
    }

    EXPECT_EQUAL(queue.size(), 50);
    for (int i = 0; i < 50; i++)
    {
        EXPECT_EQUAL(queue.dequeue(), string(i, i));
    }

    EXPECT_EQUAL(queue.isEmpty(), true);
}

PROVIDED_TEST("dequeue order test")
{
    PriorityQueue<int, string> queue;
    queue.enqueue(1, "a");
    queue.enqueue(3, "c");
    queue.enqueue(2, "b");
    queue.enqueue(5, "e");
    queue.enqueue(4, "d");

    EXPECT_EQUAL(queue.size(), 5);
    EXPECT_EQUAL(queue.dequeue(), "a");
    EXPECT_EQUAL(queue.dequeue(), "b");
    EXPECT_EQUAL(queue.dequeue(), "c");
    EXPECT_EQUAL(queue.dequeue(), "d");
    EXPECT_EQUAL(queue.dequeue(), "e");
    EXPECT_EQUAL(queue.isEmpty(), true);
    EXPECT_ERROR(queue.dequeue());
}
