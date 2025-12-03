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
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Ideally, use a prime number (e.g., 23 instead of 20)
const int TABLE_SIZE = 23; 

struct Node {
    string key;
    string data;
    Node* next;

    Node(string k, string d) : key(k), data(d), next(nullptr) {}
};

class HashTable {
private:
    Node* table[TABLE_SIZE];

    // Improved Hash Function
    int hashFunction(const string& key) const {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        // IMPORTANT: Use Modulo (%), not Division (/)
        return sum % TABLE_SIZE; 
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    // Destructor to prevent memory leaks
    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }

    void insert(string key, string data) {
        int index = hashFunction(key);
        Node* current = table[index];

        // 1. Check if key exists and update
        while (current != nullptr) {
            if (current->key == key) {
                current->data = data;
                return;
            }
            current = current->next;
        }

        // 2. If not found, insert at HEAD (Collision: Chaining)
        Node* newNode = new Node(key, data);
        newNode->next = table[index];
        table[index] = newNode;
    }

    // New Delete Function
    void remove(string key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                // Case 1: Node is head of the list
                if (prev == nullptr) {
                    table[index] = current->next;
                } 
                // Case 2: Node is in middle or end
                else {
                    prev->next = current->next;
                }
                delete current; // Free memory
                cout << "Deleted key: " << key << endl;
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Key not found for deletion: " << key << endl;
    }

    // Return string data directly, or empty string if not found
    string search(string key) const {
        int index = hashFunction(key);
        Node* current = table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return current->data;
            }
            current = current->next;
        }
        return "Not Found";
    }

    void displayTable() {
        for(int i = 0; i < TABLE_SIZE; i++) {
            if(table[i] != nullptr) {
                cout << "Index " << i << ": ";
                Node* temp = table[i];
                while(temp) {
                    cout << "[" << temp->key << ":" << temp->data << "] -> ";
                    temp = temp->next;
                }
                cout << "NULL" << endl;
            }
        }
    }
};

int main() {
    HashTable ht;

    // Insertions
    ht.insert("A", "Apple");
    ht.insert("B", "Banana");
    ht.insert("C", "Cherry");
    
    // Collision demonstration (assuming simple ascii sum creates collision)
    ht.insert("D", "Date"); 

    // Update existing key
    ht.insert("A", "Apricot");

    cout << "--- Current Table ---" << endl;
    ht.displayTable();

    cout << "\n--- Search ---" << endl;
    cout << "Value for B: " << ht.search("B") << endl;

    cout << "\n--- Deletion ---" << endl;
    ht.remove("B"); // Remove middle/head
    ht.remove("Z"); // Remove non-existent

    cout << "\n--- Final Table ---" << endl;
    ht.displayTable();

    return 0;
}
