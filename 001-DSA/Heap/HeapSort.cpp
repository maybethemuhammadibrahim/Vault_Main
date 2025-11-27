#include <iostream>
using namespace std;

// Return index of left child
int left(int i) { 
    return 2 * i + 1; 
}

// Return index of right child
int right(int i) { 
    return 2 * i + 2; 
}

// Heapify a subtree rooted at index 'i'.
// 'n' is the current heap size.
void heapifyDown(int arr[], int n, int i) {
    int largest = i;
    int l = left(i);
    int r = right(i);

    // If left child exists and is greater than current largest
    if(l < n && arr[l] > arr[largest])
        largest = l;

    // If right child exists and is greater than current largest
    if(r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not the root node, swap and continue heapifying
    if(largest != i) {
        swap(arr[i], arr[largest]);
        heapifyDown(arr, n, largest);
    }
}

// Build a max heap from an array
void buildMaxHeap(int arr[], int n) {
    // Start heapifying from last internal node down to root
    for(int i = n/2 - 1; i >= 0; i--) {
    // for(int i = 0; i < n-1; i++) {
        heapifyDown(arr, n, i);
    }
}

// Perform heapsort on an array (ascending order)
void heapSort(int arr[], int n) {
    // Step 1: Build max heap
    buildMaxHeap(arr, n);

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << endl;
    }
    cout << endl;
    

    // Step 2: Repeatedly extract max element and fix heap
    for(int i = n - 1; i > 0; i--) {
        // Move current max to end
        swap(arr[0], arr[i]);

        // Reduce heap size and restore max-heap property
        heapifyDown(arr, i, 0);
    }
}

int main() {
    int arr[] = {10, 5, 70, 15, 12, 35, 50};
    int n = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, n);

    for(int i = 0; i < n; i++)
        cout << arr[i] << " ";
}
