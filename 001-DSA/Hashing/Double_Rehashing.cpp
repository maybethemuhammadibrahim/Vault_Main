#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Helper function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Helper to find the next prime number >= n
int nextPrime(int n) {
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

// Helper to find the largest prime smaller than n (for the second hash function)
int prevPrime(int n) {
    n--;
    while (n >= 2) {
        if (isPrime(n)) return n;
        n--;
    }
    return 3; // Fallback
}

struct Entry {
    int key;
    string data;
    bool isActive; 

    Entry() : key(0), data(""), isActive(false) {}
};

class DoubleHash {
    Entry* table; // Changed from vector<Entry> to pointer
    int currentSize;
    int tableSize;
    int primeR; // The prime number used in Hash2

    int hash1(int key) {
        return key % tableSize;
    }

    int hash2(int key) {
        // Formula: R - (key % R)
        return primeR - (key % primeR);
    }

    void rehash() {
        cout << "\n--- REHASHING TRIGGERED (Load Factor > 0.6) ---" << endl;
        
        // 1. Save pointer to old data
        Entry* oldTable = table;
        int oldSize = tableSize;

        // 2. Calculate new properties
        // Double the size and find the next prime
        tableSize = nextPrime(oldSize * 2); 
        // Find a new Prime R smaller than the new table size
        primeR = prevPrime(tableSize);
        
        cout << "Old Size: " << oldSize << " -> New Size: " << tableSize << endl;
        cout << "Old R: " << (primeR == prevPrime(oldSize) ? "Unknown" : to_string(primeR)) 
             << " -> New R: " << primeR << endl;

        // 3. Allocate new table array
        table = new Entry[tableSize];
        currentSize = 0; // Reset count, insert() will increment it again

        // 4. Re-insert active entries
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i].isActive) {
                insert(oldTable[i].key, oldTable[i].data);
            }
        }

        // 5. Delete old array from memory
        delete[] oldTable;

        cout << "--- REHASHING COMPLETE ---\n" << endl;
    }

public:
    DoubleHash(int size = 7) {
        tableSize = nextPrime(size);
        primeR = prevPrime(tableSize);
        table = new Entry[tableSize]; // Allocate memory manually
        currentSize = 0;
        cout << "Initialized Table Size: " << tableSize << ", Prime R: " << primeR << endl;
    }

    // Destructor to free memory
    ~DoubleHash() {
        delete[] table;
    }

    void insert(int key, string data) {
        // Check Load Factor (Threshold 0.6)
        if (currentSize >= tableSize * 0.6) {
            rehash();
        }

        int index1 = hash1(key);
        int stepSize = hash2(key); 
        int i = 0;

        while (i < tableSize) {
            int probeIndex = (index1 + (i * stepSize)) % tableSize;

            // Found empty slot -> Insert
            if (table[probeIndex].isActive == false) {
                table[probeIndex].key = key;
                table[probeIndex].data = data;
                table[probeIndex].isActive = true;
                currentSize++;
                return;
            }

            // Found existing key -> Update
            if (table[probeIndex].key == key) {
                table[probeIndex].data = data;
                return;
            }

            i++;
        }
        cout << "Error: Table full or loop error (Should trigger rehash before this)." << endl;
    }

    string search(int key) {
        int index1 = hash1(key);
        int stepSize = hash2(key);
        int i = 0;

        while (i < tableSize) {
            int probeIndex = (index1 + (i * stepSize)) % tableSize;

            if (table[probeIndex].isActive == false) {
                return "Not Found";
            }

            if (table[probeIndex].key == key) {
                return table[probeIndex].data;
            }

            i++;
        }
        return "Not Found";
    }

    void display() {
        cout << "Table (Size " << tableSize << "):" << endl;
        for (int i = 0; i < tableSize; i++) {
            if (table[i].isActive) {
                cout << "[" << i << "] -> " << table[i].key << " (" << table[i].data << ")" << endl;
            }
        }
    }
};

int main() {
    // Start with a small size to force rehash quickly
    DoubleHash table(5); // Will init to prime size 5 or 7

    table.insert(10, "A");
    table.insert(20, "B");
    table.insert(30, "C"); // Should trigger rehash around here

    cout << "Current Table Status:" << endl;
    table.display();

    table.insert(40, "D");
    table.insert(50, "E");

    cout << "Final Table Status:" << endl;
    table.display();

    cout << "Search 10: " << table.search(10) << endl;

    return 0;
}
