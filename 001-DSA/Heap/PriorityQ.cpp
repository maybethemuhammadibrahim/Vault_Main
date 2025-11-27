#include <iostream>
using namespace std;

struct Node
{
    int value;    
    int priority; 
    Node(int val = -1, int pri = -1) : value(val), priority(pri) {}
};

class PriorityQueue
{
private:
    Node *heap; 
    int capacity;
    int size;

    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyUp(int i) 
    {
        while (i != 0 && heap[i].priority > heap[parent(i)].priority) 
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) 
    {
        int largest = i;
        int l = left(i);
        int r = right(i);

        if (l < size && heap[l].priority > heap[largest].priority)
            largest = l;

        if (r < size && heap[r].priority > heap[largest].priority)
            largest = r;

        if (largest != i) 
        {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    PriorityQueue(int c) : capacity(c), size(0) 
    {
        heap = new Node[capacity];
    }

    ~PriorityQueue() 
    {
        delete[] heap;
    }

    void insert(int val, int p) 
    {
        if (size >= capacity) 
        {
            cout << "Overflow\n";
            return;
        }

        heap[size] = Node(val, p);
        heapifyUp(size);
        size++;
    }

    Node deleteR() 
    {
        if (size == 0) 
        {
            cout << "Underflow\n";
            return Node();   // return an empty node safely
        }

        Node r = heap[0];
        heap[0] = heap[size - 1];  

        size--;
        heapifyDown(0);
        return r;
    }

    int peek() 
    {
        if (size == 0) {
            cout << "Empty\n";
            return -1;
        }
        return heap[0].value;
    }

    void display() 
    {
        for (int i = 0; i < size; i++)
            cout << "(" << heap[i].value << ", p=" << heap[i].priority << ") ";
        cout << endl;
    }
};

int main() 
{
    PriorityQueue pq(20);

    // Insert some values with priorities
    pq.insert(50, 1);
    pq.insert(20, 5);
    pq.insert(15, 2);
    pq.insert(30, 8);
    pq.insert(10, 3);

    cout << "Heap contents: ";
    pq.display();

    cout << "Top element (highest priority): " << pq.peek() << endl;

    cout << "Deleting top...\n";
    Node removed = pq.deleteR();
    cout << "Removed: " << removed.value << " (priority " << removed.priority << ")\n";

    cout << "Heap after deletion: ";
    pq.display();
}
