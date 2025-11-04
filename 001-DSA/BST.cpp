#include <iostream>
#include <queue>
using namespace std;

// ===============================
// Node Structure
// ===============================
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) {
        data = val;
        left = right = NULL;
    }
};

// ===============================
// BST Insertion
// ===============================
Node* insertBST(Node* root, int key) {
    if (root == NULL) {
        return new Node(key);
    }
    if (key < root->data) {
        root->left = insertBST(root->left, key);
    } else if (key > root->data) {
        root->right = insertBST(root->right, key);
    }
    return root;
}

// ===============================
// Traversals
// ===============================
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void preorder(Node* root) {
    if (root == NULL) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

void levelOrder(Node* root) {
    if (root == NULL) return;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        cout << curr->data << " ";
        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}

// ===============================
// Search in BST
// ===============================
bool searchBST(Node* root, int key) {
    if (root == NULL) return false;
    if (root->data == key) return true;
    else if (key < root->data) return searchBST(root->left, key);
    else return searchBST(root->right, key);
}

// ===============================
// Find Minimum Node (for deletion)
// ===============================
Node* findMin(Node* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

// ===============================
// Delete Node in BST
// ===============================
Node* deleteBST(Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->data) {
        root->left = deleteBST(root->left, key);
    } else if (key > root->data) {
        root->right = deleteBST(root->right, key);
    } else {
        // Node found
        // Case 1: No child
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        }
        // Case 2: One child
        else if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        // Case 3: Two children
        Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data);
    }
    return root;
}

// ===============================
// Height of Tree
// ===============================
int height(Node* root) {
    if (root == NULL) return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    return max(lh, rh) + 1;
}

// ===============================
// Count Total Nodes
// ===============================
int countNodes(Node* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// ===============================
// Check Full Binary Tree
// ===============================
bool isFullBinaryTree(Node* root) {
    if (root == NULL) return true;

    // Leaf node
    if (root->left == NULL && root->right == NULL) return true;

    // Node with two children
    if (root->left && root->right)
        return isFullBinaryTree(root->left) && isFullBinaryTree(root->right);

    // One child only
    return false;
}

// ===============================
// Check Complete Binary Tree
// ===============================
bool isCompleteBinaryTree(Node* root) {
    if (root == NULL) return true;

    queue<Node*> q;
    q.push(root);
    bool nullFound = false;

    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();

        if (curr == NULL) {
            nullFound = true;
        } else {
            if (nullFound) return false; // if null seen before, not complete
            q.push(curr->left);
            q.push(curr->right);
        }
    }
    return true;
}

// ===============================
// MAIN FUNCTION
// ===============================
int main() {
    Node* root = NULL;

    // Insert elements in BST
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        root = insertBST(root, values[i]);
    }

    cout << "Inorder Traversal (sorted): ";
    inorder(root);
    cout << endl;

    cout << "Preorder Traversal: ";
    preorder(root);
    cout << endl;

    cout << "Postorder Traversal: ";
    postorder(root);
    cout << endl;

    cout << "Level Order Traversal: ";
    levelOrder(root);
    cout << endl;

    // Search
    int key = 60;
    cout << "Search " << key << ": ";
    cout << (searchBST(root, key) ? "Found" : "Not Found") << endl;

    // Delete Node
    root = deleteBST(root, 20);
    cout << "After deleting 20 (Inorder): ";
    inorder(root);
    cout << endl;

    // Height & Count
    cout << "Height of Tree: " << height(root) << endl;
    cout << "Total Nodes: " << countNodes(root) << endl;

    // Check Full and Complete
    cout << "Is Full Binary Tree? " << (isFullBinaryTree(root) ? "Yes" : "No") << endl;
    cout << "Is Complete Binary Tree? " << (isCompleteBinaryTree(root) ? "Yes" : "No") << endl;

    return 0;
}
