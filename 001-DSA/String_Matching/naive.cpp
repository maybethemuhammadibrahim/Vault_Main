#include <iostream>
#include <string>

using namespace std;

int* bruteForceSearch(string text, string pattern, int& count) {
    int n = text.length();
    int m = pattern.length();

    int* indexes = new int[n];
    count = 0;

    for (int i = 0; i <=n-m; i++) {
        int j = 0;
        while (j < m && text[i+j] == pattern[j]) {
            j++;
        }
        if (j == m) {
            indexes[count++] = i;
        }
    }
    return indexes;
}

int main() {
    string text = "the quick brown fox jumps over the lazy dog";
    string pattern = "the";
    int count = 0;

    int* result = bruteForceSearch(text, pattern, count);

    cout << "[";
    for (int i = 0; i<count; i++) {
        cout << result[i] << (i < count - 1 ? ", " : "");
    }
    cout << "]" << endl;

    delete[] result;
    return 0;
}
