#include <iostream>
using namespace std;

#define SIZE 5  // maximum size of the circular queue

class CircularQueue {
private:
    int items[SIZE];
    int front, rear;

public:
    CircularQueue() {
        front = -1;
        rear = -1;
    }

    // Check if the queue is full
 bool isFull() {
    if ((rear + 1) % SIZE == front)
        return true;
    return false;
}

    // Check if the queue is empty
    bool isEmpty() {
        return (front == -1);
    }

    // Insert element in the queue
    void enqueue(int element) {
        if (isFull()) {
            cout << "Queue is FULL!" << endl;
            return;
        }

        if (front == -1) front = 0; // first element
        rear = (rear + 1) % SIZE;
        items[rear] = element;
        cout << "Inserted: " << element << endl;
    }

    // Delete element from the queue
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is EMPTY!" << endl;
            return;
        }

        cout << "Deleted: " << items[front] << endl;

        if (front == rear) {
            // Queue has only one element
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % SIZE;
        }
    }

    // Display elements of queue
    void display() {
        if (isEmpty()) {
            cout << "Queue is EMPTY!" << endl;
            return;
        }

        cout << "Queue elements are: ";
        int i = front;
        while (true) {
            cout << items[i] << " ";
            if (i == rear)
                break;
            i = (i + 1) % SIZE;
        }
        cout << endl;
    }
};

int main() {
    CircularQueue q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.enqueue(50);

    q.display();

    q.dequeue();
    q.dequeue();

    q.display();

    q.enqueue(60);
    q.enqueue(70);

    q.display();

    return 0;
}


