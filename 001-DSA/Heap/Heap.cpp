#include <iostream>
using namespace std;

class maxHeap {
    int *heap;
    int capacity;
    int size;

    int parent(int i) {return (i-1)/2;}
    int left(int i) {return (i*2)+1;}
    int right(int i) {return (i*2)+2;}

    void heapifyUp(int i) {
        // CHANGE (MinHeap): Change > to < (swap if child is smaller than parent)
        while(i!=0 && heap[i] > heap[parent(i)]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        // CHANGE (MinHeap): Rename 'largest' to 'smallest' for clarity
        int largest = i; 
        int l = left(i);
        int r = right(i);

        // CHANGE (MinHeap): Change > to < (check if left child is smaller)
        if(l < size && heap[l] > heap[largest]) 
            largest = l;

        // CHANGE (MinHeap): Change > to < (check if right child is smaller)
        if(r < size && heap[r] > heap[largest]) 
            largest = r;

        if(i != largest) {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    public:
    maxHeap(int c) : capacity(c), size(0) {
        heap = new int[capacity];
    }
    ~maxHeap() {
        delete [] heap;
    }

    void insert(int data) {
        if(size == capacity) {
            cout << "Heap is Full!" << endl;
            return;
        }
        heap[size] = data;
        heapifyUp(size);
        size++;
    }

    void deleteR() {
        if(size == 0) {
            cout << "Heap is Empty!" << endl;
            return;
        }

        cout << heap[0] << endl;
        heap[0] = heap[size-1];
        size--;
        heapifyDown(0);
    }

    void insert(int arr[], int size) {
        for(int i = 0; i < size; i++) {
            insert(arr[i]);
        }
    }

    void insertArr(int arr[],int sizeA) {
        for(int i = 0; i < sizeA; i++) {
            if(size >= capacity) { 
                cout << "Heap Overflow!" << endl;
                return;
            }
            // Better to just call: insert(arr[i]); 
            heap[size] = arr[i];
            cout << "inserted " << arr[i] << endl; 
            heapifyUp(size);
            size++; 
        }
    }

    void displayHeap() {
        for(int i = 0; i < size; i++) {
            cout << heap[i] << endl;
        }
    }

    void heapSort() {
    int originalSize = size;   // keep a copy

    for(int i = size - 1; i > 0; i--) {
        swap(heap[0], heap[i]);   // move max to end
        size--;                   // shrink the heap
        heapifyDown(0);           // restore heap property
    }

    size = originalSize;  // restore so heap is usable after sort
}
};

int main() {
    int arr[] = {10, 5, 70, 15, 12, 35, 50};
    maxHeap heap(20);
    heap.insertArr(arr, 7);
    heap.displayHeap();
    heap.heapSort();
    heap.displayHeap();
}