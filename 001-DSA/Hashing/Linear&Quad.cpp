#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Prime size is crucial for Quadratic Probing to ensure we visit at least half the table
#define TABLE_SIZE 23 

struct Entry {
    int key;
    string data;
    bool isActive; // Marks if the slot is currently occupied

    Entry() : key(0), data(""), isActive(false) {}
};

class OpenAddressingHash {
    Entry table[TABLE_SIZE];
    int currentSize;

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

public:
    OpenAddressingHash() {
        currentSize = 0;
    }

    void insert(int key, string data) {
        if (currentSize >= TABLE_SIZE) {
            cout << "Hash Table is Full!" << endl;
            return;
        }

        int hashIndex = hashFunction(key);
        int i = 0;

        // Loop to find an empty spot or the existing key
        while (i < TABLE_SIZE) {
            
            // --- COLLISION RESOLUTION LOGIC ---
            
            // 1. LINEAR PROBING FORMULA: (Hash + i) % Size
            int probeIndex = (hashIndex + i) % TABLE_SIZE;

            // 2. QUADRATIC PROBING FORMULA (Uncomment to use):
            // int probeIndex = (hashIndex + (i * i)) % TABLE_SIZE;

            // ----------------------------------

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

            // Collision occurred -> increment i and try next probe
            i++;
        }
    }

    string search(int key) {
        int hashIndex = hashFunction(key);
        int i = 0;

        while (i < TABLE_SIZE) {
            
            // --- MUST MATCH INSERT LOGIC ---
            
            // LINEAR:
            int probeIndex = (hashIndex + i) % TABLE_SIZE;

            // QUADRATIC:
            // int probeIndex = (hashIndex + (i * i)) % TABLE_SIZE;

            // -------------------------------

            // If we hit an empty slot, the key definitely isn't here
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
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isActive) {
                cout << "Index " << i << ": [" << table[i].key << " -> " << table[i].data << "]" << endl;
            } else {
                cout << "Index " << i << ": -- Empty --" << endl;
            }
        }
    }
};

int main() {
    OpenAddressingHash table;

    // These keys might cause collisions depending on TABLE_SIZE
    table.insert(10, "Ten");   // 10 % 23 = 10
    table.insert(33, "Thirty-Three"); // 33 % 23 = 10 (Collision 1 -> Probes to 11)
    table.insert(56, "Fifty-Six");    // 56 % 23 = 10 (Collision 2 -> Probes to 12)

    cout << "--- Hash Table Content ---" << endl;
    table.display();

    cout << "\n--- Search Results ---" << endl;
    cout << "Search 33: " << table.search(33) << endl;
    cout << "Search 99: " << table.search(99) << endl;

    return 0;
}
