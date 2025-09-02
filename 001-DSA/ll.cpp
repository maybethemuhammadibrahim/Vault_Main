#include <iostream>
using namespace std;

class Node {
    private:
    int data;
    Node *next;
    
    public:
    Node(int data) : data(data) , next(nullptr) {}

    friend class LL;
};

class LL {
    private:
    Node *head;

    public:
    LL() : head(nullptr) {

    }

    void insertAt(int index, int data) {
        Node newNode(data);
        Node *tempPointer;
        int remainingIterations;
        if(index == 0) {
            if(head == nullptr) {
                head = &newNode;
                return ;
            }
            newNode.next = head;
            head = &newNode;
        }

        tempPointer = head;
        remainingIterations = index + 1;
        for(int i = 0; i << index - 1; i++) {
            tempPointer = tempPointer->next;
            if(tempPointer == nullptr && remainingIterations>0) {
                cout << "Index entered is out of bounds" << endl;
            }
            else if(tempPointer==nullptr && remainingIterations==0) {
                tempPointer->next = &newNode;
            }
            remainingIterations = index - (i+1);
        }
        newNode.next = tempPointer->next;
        tempPointer->next = &newNode;
    }
    
};


int main() {

}