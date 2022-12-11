#include <iostream>
#include "console.h"
#include "PriorityQueue.h"
#include "testing/SimpleTest.h"
using namespace std;

int main() {
    PriorityQueue<int,string> pq;
    cout<<"pq.enqueue(2,\"thunder\");"<<endl;
    cout<<"pq.enqueue(3,\"ironman\");"<<endl;
    cout<<"pq.enqueue(4,\"hulk\");"<<endl;
    cout<<"pq.enqueue(1,\"widow\");"<<endl;
    pq.enqueue(2,"thunder");
    pq.enqueue(3,"ironman");
    pq.enqueue(4,"hulk");
    pq.enqueue(1,"widow");
    cout<<"pq.peek()  "<<pq.peek()<<endl;
    cout<<"-------------------------------"<<endl;
    cout<<"pq.adjustPriority(\"widow\",6);"<<endl;
    pq.adjustPriority("widow",6);
    cout<<"pq.peek()  "<<pq.peek()<<endl;
//    if (runSimpleTests(ALL_TESTS)) {
//        return 0;
//    }
    return 0;
}
