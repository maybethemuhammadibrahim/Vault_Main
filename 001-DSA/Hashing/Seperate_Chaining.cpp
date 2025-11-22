#include <iostream>

// ============================================================
// THEORY: COMMON HASH FUNCTIONS
// ============================================================
/*
    1. Division Method (Used in this code)
       Formula: h(k) = k % m
       - k is the key, m is the table size.
       - Use: Most common for integers.
       - Best Practice: 'm' should be a prime number not close to a power of 2
         to distribute keys evenly.

    2. Multiplication Method
       Formula: h(k) = floor( m * (k * A % 1) )
       - 'A' is a constant between 0 and 1 (e.g., Knuth suggests (sqrt(5)-1)/2).
       - (k * A % 1) extracts the fractional part of k*A.
       - Use: Good when table size is not a prime number (e.g., power of 2).

    3. Mid-Square Method
       Formula: Square the key (k^2), then extract the middle r digits.
       - Use: Good when keys often share the same leading or trailing digits.
       - Randomizes the bits well.

    4. Folding Method
       Formula: Split key into parts (e.g., 123456 -> 12 + 34 + 56), add them up,
       then take Modulo m.
       - Use: Useful for keys with a large number of digits (like SSNs or phone numbers)
       - Can be "Shift Folding" (just adding) or "Boundary Folding" (reversing every other part).

    5. Polynomial Rolling Hash (String Hashing)
       Formula: (s[0]*p^0 + s[1]*p^1 + ... + s[n-1]*p^n-1) % m
       - Use: The standard way to hash Strings.
       - 'p' is usually a prime (31 or 53).
*/

// ============================================================
// IMPLEMENTATION (NO STL)
// ============================================================

// A simple Node for the Linked List (Chaining)
struct Node {
    int key;
    int value;
    Node* next;

    Node(int k, int v) {
        key = k;
        value = v;
        next = nullptr;
    }
};

class HashTable {
private:
    Node** table;     // Array of pointers to Nodes (the buckets)
    int capacity;     // Size of the array (number of buckets)

    // MAIN HASH FUNCTION: Division Method
    int hashFunction(int key) {
        // Ensure the result is positive even for negative keys
        return abs(key) % capacity;
    }

    // Helper to compute absolute value without <cmath>
    int abs(int val) {
        return (val < 0) ? -val : val;
    }

public:
    // Constructor
    HashTable(int cap) {
        capacity = cap;
        table = new Node*[capacity];
        
        // Initialize all buckets to nullptr
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }

    // Destructor: Clean up memory
    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] table;
    }

    // INSERTION ADT
    // Adds a key-value pair. If key exists, updates the value.
    void insert(int key, int value) {
        int index = hashFunction(key);
        
        Node* current = table[index];

        // 1. Check if key already exists (Update)
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // Update value
                return;
            }
            current = current->next;
        }

        // 2. Key not found, insert at beginning (Collision Resolution: Chaining)
        Node* newNode = new Node(key, value);
        newNode->next = table[index]; // Point new node to current head
        table[index] = newNode;       // Update head to new node
        
        std::cout << "Inserted key: " << key << " at index: " << index << std::endl;
    }

    // SEARCH ADT
    // Returns the value associated with key, or -1 if not found
    int search(int key) {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return -1; // Not found indicator
    }

    // DELETION ADT
    // Removes a key from the table
    void remove(int key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* prev = nullptr;

        // Traverse the list at this bucket
        while (current != nullptr) {
            if (current->key == key) {
                // Case 1: Node to delete is head
                if (prev == nullptr) {
                    table[index] = current->next;
                } 
                // Case 2: Node to delete is in middle or end
                else {
                    prev->next = current->next;
                }
                
                delete current;
                std::cout << "Removed key: " << key << std::endl;
                return;
            }
            prev = current;
            current = current->next;
        }
        
        std::cout << "Key " << key << " not found for deletion." << std::endl;
    }

    // Utility: Print the entire table structure
    void printTable() {
        std::cout << "\n--- Hash Table Status ---" << std::endl;
        for (int i = 0; i < capacity; i++) {
            std::cout << "Bucket " << i << ": ";
            Node* current = table[i];
            while (current != nullptr) {
                std::cout << "[" << current->key << ":" << current->value << "] -> ";
                current = current->next;
            }
            std::cout << "NULL" << std::endl;
        }
        std::cout << "-------------------------\n" << std::endl;
    }
};

// Main Driver
int main() {
    // Create a Hash Table with 7 buckets (Prime number is simpler for Division method)
    HashTable ht(7);

    // 1. Test Insertion
    ht.insert(10, 100); // 10 % 7 = 3
    ht.insert(20, 200); // 20 % 7 = 6
    ht.insert(15, 150); // 15 % 7 = 1
    ht.insert(7, 700);  // 7 % 7 = 0

    // 2. Test Collision (Chaining)
    // 17 % 7 = 3 (Collides with 10)
    std::cout << "\nInserting collision (17)..." << std::endl;
    ht.insert(17, 170); 

    ht.printTable();

    // 3. Test Search
    std::cout << "Searching for 10: " << ht.search(10) << std::endl;
    std::cout << "Searching for 17: " << ht.search(17) << std::endl;
    std::cout << "Searching for 99: " << ht.search(99) << " (Should be -1)" << std::endl;

    // 4. Test Deletion
    std::cout << "\nDeleting key 10..." << std::endl;
    ht.remove(10); // Should remove 10 but keep 17 in the chain

    ht.printTable();

    return 0;
}
