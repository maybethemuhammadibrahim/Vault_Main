#include <iostream>
using namespace std;

void selection(int arr[], int size, int i) {
    if(i >= size)
        return;
    
    int minIndex = i;
    for(int j = i+1; j < size; j++) {
        if(arr[j] < arr[minIndex]) {
            minIndex = j;
        }
    }
    if(minIndex!=i) {
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
    selection(arr, size, i+1);
}

void insertion(int arr[], int size, int i) {
    if(i >= size) {
        return ;
    }
    int curr = arr[i];
    int j = i - 1;
    while(j>=0 && arr[j] > curr) {
        arr[j+1] = arr[j];
        j--;
    }
    arr[j+1] = curr;
    insertion(arr, size, i+1); 
}

void bubbleSortInner(int arr[], int size, int j) {
    if(j >= size) {
        return ;
    }
    if(arr[j] > arr[j+1]) {
        int temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
        // swapped = true;

    }
    bubbleSortInner(arr, size, j+1); 
    // return swapped;

}

void bubbleSortOuter(int arr[], int size, int i) {
    if(i >= size) {
        return ;
    }
    bubbleSortInner(arr, size, 0); //if swapping occurred then true

    
    bubbleSortOuter(arr, size, i+1);
    

}

int main() {
    int arr[6] = {4,3,2,1,9,0};
    bubbleSortOuter(arr, 6, 0);

    for(int i = 0; i < 6; i++) {
        cout << arr[i] << endl;
    }
}
