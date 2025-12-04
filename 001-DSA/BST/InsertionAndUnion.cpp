#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

// --- HELPER FUNCTIONS ---

// 1. Flatten: In-order traversal to store nodes in a sorted vector
void inOrderTraversal(Node* root, vector<int>& arr) {
    if (root == nullptr) return;
    inOrderTraversal(root->left, arr);
    arr.push_back(root->data);
    inOrderTraversal(root->right, arr);
}

// 2. Build: Construct a balanced AVL tree from a sorted vector
// This is O(N) and creates a perfectly balanced tree automatically
Node* sortedArrayToAVL(const vector<int>& arr, int start, int end) {
    if (start > end) return nullptr;

    // Get the middle element to make it the root
    int mid = (start + end) / 2;
    Node* root = new Node(arr[mid]);

    // Recursively construct left and right subtrees
    root->left = sortedArrayToAVL(arr, start, mid - 1);
    root->right = sortedArrayToAVL(arr, mid + 1, end);

    return root;
}

// --- CORE LOGIC: INTERSECTION ---

Node* findIntersection(Node* root1, Node* root2) {
    // Step 1: Flatten both trees into sorted vectors
    vector<int> arr1, arr2;
    inOrderTraversal(root1, arr1);
    inOrderTraversal(root2, arr2);

    // Step 2: Merge the two sorted vectors (Intersection Logic)
    vector<int> resultArr;
    int i = 0, j = 0;
    int n1 = arr1.size();
    int n2 = arr2.size();

    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            // For Intersection: Skip smaller element in arr1
            i++; 
            
            // --- CHANGE FOR UNION ---
            // resultArr.push_back(arr1[i]); // Add distinct element
            // i++;
            // ------------------------
        }
        else if (arr2[j] < arr1[i]) {
            // For Intersection: Skip smaller element in arr2
            j++;

            // --- CHANGE FOR UNION ---
            // resultArr.push_back(arr2[j]); // Add distinct element
            // j++;
            // ------------------------
        }
        else {
            // Equal elements found! Add to result.
            resultArr.push_back(arr1[i]);
            i++;
            j++;
        }
    }

    // --- CHANGE FOR UNION ---
    // For Union, you must also add remaining elements from both arrays
    // while (i < n1) { resultArr.push_back(arr1[i++]); }
    // while (j < n2) { resultArr.push_back(arr2[j++]); }
    // ------------------------

    // Step 3: Convert the merged sorted array back into an AVL Tree
    return sortedArrayToAVL(resultArr, 0, resultArr.size() - 1);
}

// --- UTILITY TO PRINT TREE (Level Order / InOrder) ---
void printInOrder(Node* root) {
    if (!root) return;
    printInOrder(root->left);
    cout << root->data << " ";
    printInOrder(root->right);
}

// Simple helper to create dummy nodes for testing
Node* newNode(int data) {
    return new Node(data);
}

int main() {
    // Manually creating two simple BSTs/AVLs for demonstration
    // Tree 1: 10, 20, 30
    Node* root1 = newNode(20);
    root1->left = newNode(10);
    root1->right = newNode(30);

    // Tree 2: 20, 30, 40
    Node* root2 = newNode(30);
    root2->left = newNode(20);
    root2->right = newNode(40);

    cout << "Tree 1 Elements: ";
    printInOrder(root1);
    cout << "\nTree 2 Elements: ";
    printInOrder(root2);

    // Perform Intersection
    Node* intersectionRoot = findIntersection(root1, root2);

    cout << "\n\nIntersection Tree Elements: ";
    printInOrder(intersectionRoot); 
    // Expected Output: 20 30

    return 0;
}
