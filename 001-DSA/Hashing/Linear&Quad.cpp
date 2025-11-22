#include <iostream>

// ============================================================
// THEORY: OPEN ADDRESSING (LINEAR & QUADRATIC PROBING)
// ============================================================
/*
    In Open Addressing, all elements are stored within the hash table array itself.
    No linked lists are used. If a collision occurs, we probe for the next available slot.

    1. Linear Probing
       Formula: index = (hash(key) + i) % m
       - 'i' is the probe number (0, 1, 2...).
       -Linearly checks the next slot.
       - Issue: Primary Clustering (blocks of filled slots merge).

    2. Quadratic Probing
       Formula: index = (hash(key) + i * i) % m
       - Checks slots like hash, hash+1, hash+4, hash+9...
       - Reduces primary clustering but can cause Secondary Clustering.

    Deletion in Open Addressing:
    We cannot simply nullify a slot because it might break the probe chain for
    elements inserted later in the sequence.
    Solution: "Lazy Deletion". We mark the slot with a special dummy/tombstone node.
*/

// ============================================================
// IMPLEMENTATION (NO STL)
// ============================================================

struct HashNode {
    int key;
    int value;

    HashNode(int k, int v) {
        key = k;
        value = v;
    }
};

class HashTable {
private:
    HashNode** table; // Array of pointers to HashNodes
    int capacity;
    int size;
    HashNode* dummy;  // Special marker for deleted slots

    // Basic Hash Function
    int hashFunction(int key) {
        return abs(key) % capacity;
    }

    int abs(int val) {
        return (val < 0) ? -val : val;
    }

public:
    HashTable(int cap) {
        capacity = cap;
        size = 0;
        table = new HashNode*[capacity];
        
        // Initialize buckets to nullptr
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }

        // Create a dummy node (key = -1, val = -1) to represent deleted spots
        dummy = new HashNode(-1, -1);
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr && table[i] != dummy) {
                delete table[i];
            }
        }
        delete[] table;
        delete dummy;
    }

    // INSERTION ADT
    void insert(int key, int value) {
        if (size == capacity) {
            std::cout << "Hash Table is full! Cannot insert " << key << std::endl;
            return;
        }

        int hashIndex = hashFunction(key);
        int i = 0;
        
        // Loop to find an empty slot or update existing key
        while (i < capacity) {
            // --- PROBING FORMULAS ---
            // Linear Probing:
            int index = (hashIndex + i) % capacity;
            
            // Quadratic Probing (Commented out as requested):
            // int index = (hashIndex + i * i) % capacity;

            // 1. Found empty slot or deleted slot -> Insert here
            if (table[index] == nullptr || table[index] == dummy) {
                // Clean up if we are overwriting a dummy, strictly not needed as dummy is shared
                // but if we had unique dummies we would delete here.
                
                table[index] = new HashNode(key, value);
                size++;
                std::cout << "Inserted key: " << key << " at index: " << index 
                          << " (Probe step: " << i << ")" << std::endl;
                return;
            }
            // 2. Key already exists -> Update value
            else if (table[index]->key == key) {
                table[index]->value = value;
                std::cout << "Updated key: " << key << " at index: " << index << std::endl;
                return;
            }

            // 3. Collision -> Increment probe step
            i++;
        }
    }

    // SEARCH ADT
    int search(int key) {
        int hashIndex = hashFunction(key);
        int i = 0;

        while (i < capacity) {
            int index = (hashIndex + i) % capacity;
            // Quadratic: int index = (hashIndex + i * i) % capacity;

            // If we hit nullptr, the key definitely doesn't exist (probe chain broken)
            if (table[index] == nullptr) {
                return -1; 
            }

            // If we find the key (and it's not a dummy/deleted node)
            if (table[index] != dummy && table[index]->key == key) {
                return table[index]->value;
            }

            i++;
        }
        return -1; // Not found after checking all slots
    }

    // DELETION ADT
    void remove(int key) {
        int hashIndex = hashFunction(key);
        int i = 0;

        while (i < capacity) {
            int index = (hashIndex + i) % capacity;
            // Quadratic: int index = (hashIndex + i * i) % capacity;

            if (table[index] == nullptr) {
                std::cout << "Key " << key << " not found for deletion." << std::endl;
                return;
            }

            if (table[index] != dummy && table[index]->key == key) {
                delete table[index]; // Free the memory of the actual node
                table[index] = dummy; // Place the tombstone/dummy here
                size--;
                std::cout << "Removed key: " << key << " (Marked as DELETED)" << std::endl;
                return;
            }
            i++;
        }
        std::cout << "Key " << key << " not found." << std::endl;
    }

    void printTable() {
        std::cout << "\n--- Hash Table Status ---" << std::endl;
        for (int i = 0; i < capacity; i++) {
            std::cout << "Index " << i << ": ";
            if (table[i] == nullptr) {
                std::cout << "EMPTY";
            } else if (table[i] == dummy) {
                std::cout << "DELETED (Tombstone)";
            } else {
                std::cout << "[" << table[i]->key << ":" << table[i]->value << "]";
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------------\n" << std::endl;
    }
};

int main() {
    // Use size 7. 
    // Note for Quadratic Probing: Table size should ideally be a prime 
    // congruent to 3 mod 4 to guarantee coverage of at least half the table.
    HashTable ht(7);

    // 1. Fill some spots
    ht.insert(10, 100); // 10%7 = 3
    ht.insert(20, 200); // 20%7 = 6
    ht.insert(15, 150); // 15%7 = 1

    // 2. Test Collision & Linear Probing
    // 17 % 7 = 3. Slot 3 is taken by 10. 
    // Linear Probe 1: (3+1)%7 = 4. Slot 4 is Empty. Insert at 4.
    std::cout << "\nInserting collision (17)..." << std::endl;
    ht.insert(17, 170); 

    ht.printTable();

    // 3. Test Deletion (Lazy Deletion)
    std::cout << "Deleting key 10 (which is at index 3)..." << std::endl;
    ht.remove(10); 
    // Slot 3 becomes DELETED.

    ht.printTable();

    // 4. Test Search over a "Deleted" spot
    // Searching for 17 (at index 4).
    // Hash(17) = 3. Slot 3 is DELETED. Algorithm must continue to index 4.
    std::cout << "Searching for 17: " << ht.search(17) << " (Should find it despite hole at 3)" << std::endl;

    // 5. Test Insertion into a "Deleted" spot
    std::cout << "\nInserting 24 (24%7=3)..." << std::endl;
    ht.insert(24, 240); // Should reuse the DELETED slot at index 3
    
    ht.printTable();

    return 0;
}
