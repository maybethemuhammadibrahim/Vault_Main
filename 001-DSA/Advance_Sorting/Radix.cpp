#include <iostream>
using namespace std;

// Node structure for linked list bucket
struct Node {
    int data;
    Node* next;
    Node(int val) {
        data = val;
        next = NULL;
    }
};

// Insert element at end of bucket (linked list)
void insertBucket(Node*& head, int val) {
    Node* newNode = new Node(val);
    if (head == NULL) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newNode;
}

// Clear bucket after use
void clearBucket(Node*& head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// Get maximum number in array (for digit count)
int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// Radix Sort using linked list buckets
void radixSort(int arr[], int n) {
    int maxNum = getMax(arr, n);
    int exp = 1; // digit place (1 = ones, 10 = tens, etc.)

    // Buckets for digits 0-9 (as linked lists)
    Node* buckets[10];

    while (maxNum / exp > 0) {
        // Initialize buckets
        for (int i = 0; i < 10; i++)
            buckets[i] = NULL;

        // Place elements into buckets based on current digit
        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10;
            insertBucket(buckets[digit], arr[i]);
        }

        // Collect numbers back into array
        int idx = 0;
        for (int i = 0; i < 10; i++) {
            Node* temp = buckets[i];
            while (temp != NULL) {
                arr[idx++] = temp->data;
                temp = temp->next;
            }
            // Clear memory
            clearBucket(buckets[i]);
        }

        exp *= 10; // Move to next digit place
    }
}

// Print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array: ";
    printArray(arr, n);

    radixSort(arr, n);

    cout << "Sorted array: ";
    printArray(arr, n);

    return 0;
}
