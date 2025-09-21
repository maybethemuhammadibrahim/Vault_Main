#include <iostream>
using namespace std;

/*
  Linear Search:
  Scan the array from left to right.
  Return the index of the first occurrence of target, or -1 if not found.
*/
int linearSearch(int arr[], int size, int target) {
    for (int index = 0; index < size; index++) {          // Go through every element
        if (arr[index] == target) {                       // Check if this is the value we want
            return index;                                 // Return its index
        }
    }
    return -1;                                            // Not found
}

/*
  Binary Search (Iterative):
  Works only on a sorted array (ascending).
  Repeatedly cut the search interval in half.
*/
int binarySearch(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {                               // While there is a valid search range
        int mid = left + (right - left) / 2;              // Middle index
        if (arr[mid] == target)
            return mid;                                   // Found
        else if (arr[mid] < target)
            left = mid + 1;                               // Search in right half
        else
            right = mid - 1;                              // Search in left half
    }
    return -1;                                            // Not found
}

/*
  Interpolation Search:
  Improved over binary search for uniformly distributed sorted data.
  Estimates the position based on value distribution.
*/
int interpolationSearch(int arr[], int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        // Prevent division by zero
        if (arr[high] == arr[low]) {
            if (arr[low] == target) return low;
            else return -1;
        }

        // Estimate the position (probe)
        int pos = low + (int)((double)(high - low) * (target - arr[low]) / (arr[high] - arr[low]));
        
        if (arr[pos] == target)
            return pos;                                   // Found
        else if (arr[pos] < target)
            low = pos + 1;                                // Move to upper part
        else
            high = pos - 1;                               // Move to lower part
    }
    return -1;                                            // Not found
}

/*
  "Binary Sort" (Assumption: You meant a basic Bubble Sort variant)
  As requested: start each loop from i = 0 and j = 0 (unoptimized form).
  Repeatedly compare adjacent elements and swap if out of order.
  NOTE: This version always loops full range (no early exit / optimization).
*/
void binarySort(int arr[], int size) {
    for (int i = 0; i < size; i++) {                      // Outer pass (starts at 0 each time)
        for (int j = 0; j < size - 1; j++) {              // Inner loop always from 0
            if (arr[j] > arr[j + 1]) {                    // If adjacent elements out of order
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;                        // Swap them
            }
        }
    }
}

/*
  Selection Sort:
  Repeatedly find the smallest element in the unsorted part
  and move it to the front.
*/
void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {                  // Position to fill with smallest
        int minIndex = i;                                 // Assume current is smallest
        for (int j = i + 1; j < size; j++) {              // Find a smaller element
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {                              // Swap if a smaller element found
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

/*
  Insertion Sort:
  Build a sorted portion at the left of the array.
  Insert each new element into its correct position in the sorted part.
*/
void insertionSort(int arr[], int size) {
    for (int i = 1; i < size; i++) {                      // First element (i=0) is "sorted"
        int currentValue = arr[i];                        // Value to insert
        int j = i - 1;

        // Shift larger elements to the right
        while (j >= 0 && arr[j] > currentValue) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = currentValue;                        // Insert value into correct spot
    }
}

/*
  Comb Sort:
  Improvement over bubble sort using a gap that shrinks each pass.
  Eventually becomes a bubble sort when gap reaches 1.
*/
void combSort(int arr[], int size) {
    int gap = size;                                       // Initial gap = size
    bool swapped = true;                                  // Track if a swap occurred
    const double shrinkFactor = 1.3;                      // Common shrink factor

    while (gap > 1 || swapped) {                          // Continue while progress possible
        gap = (int)(gap / shrinkFactor);                  // Shrink gap
        if (gap < 1) gap = 1;                             // Minimum gap is 1
        swapped = false;

        for (int i = 0; i + gap < size; i++) {            // Compare elements gap apart
            if (arr[i] > arr[i + gap]) {
                int temp = arr[i];
                arr[i] = arr[i + gap];
                arr[i + gap] = temp;
                swapped = true;                           // A swap means we keep going
            }
        }
    }
}

/* Helper to print array */
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << " ";
    }
    cout << "\n";
}

int main() {
    int data1[] = {34, 7, 23, 32, 5, 62};
    int n1 = sizeof(data1)/sizeof(data1[0]);

    cout << "Original array:\n";
    printArray(data1, n1);

    // Demonstrate binarySort (bubble-like)
    int bubbleExample[] = {9, 4, 6, 2, 8, 1};
    int nb = sizeof(bubbleExample)/sizeof(bubbleExample[0]);
    binarySort(bubbleExample, nb);
    cout << "After binarySort (bubble style):\n";
    printArray(bubbleExample, nb);

    // Selection Sort
    int selExample[] = {29, 10, 14, 37, 14};
    int ns = sizeof(selExample)/sizeof(selExample[0]);
    selectionSort(selExample, ns);
    cout << "After selectionSort:\n";
    printArray(selExample, ns);

    // Insertion Sort
    int insExample[] = {5, 2, 9, 1, 5, 6};
    int ni = sizeof(insExample)/sizeof(insExample[0]);
    insertionSort(insExample, ni);
    cout << "After insertionSort:\n";
    printArray(insExample, ni);

    // Comb Sort
    int combExample[] = {20, -4, 7, 3, 15, 0, 2};
    int nc = sizeof(combExample)/sizeof(combExample[0]);
    combSort(combExample, nc);
    cout << "After combSort:\n";
    printArray(combExample, nc);

    // Prepare a sorted array for searches
    int sortedArr[] = {3, 5, 8, 12, 14, 18, 21, 27, 30};
    int nsrt = sizeof(sortedArr)/sizeof(sortedArr[0]);

    cout << "\nSearching in sorted array:\n";
    printArray(sortedArr, nsrt);

    int target = 18;
    cout << "linearSearch(" << target << ") -> " << linearSearch(sortedArr, nsrt, target) << "\n";
    cout << "binarySearch(" << target << ") -> " << binarySearch(sortedArr, nsrt, target) << "\n";
    cout << "interpolationSearch(" << target << ") -> " << interpolationSearch(sortedArr, nsrt, target) << "\n";

    target = 19;
    cout << "\nSearching for missing value " << target << ":\n";
    cout << "linearSearch -> " << linearSearch(sortedArr, nsrt, target) << "\n";
    cout << "binarySearch -> " << binarySearch(sortedArr, nsrt, target) << "\n";
    cout << "interpolationSearch -> " << interpolationSearch(sortedArr, nsrt, target) << "\n";

    return 0;
}