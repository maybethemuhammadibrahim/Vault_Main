#include <iostream>
#include <string>

using namespace std;


const int d = 256;
const int q = 101;

int* rabinKarpSearch(string text, string pattern, int& count) {
    int n = text.length();
    int m = pattern.length();
    int* indices = new int[n];
    count = 0;
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < m-1; i++)
        h = (h * d) % q;

    for (i = 0; i<m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (i = 0; i <= n-m; i++) {
        if (p == t) {
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j])
                    break;
            }
            if (j == m)
                indices[count++] = i;
        }

        if (i < n - m) {
            t = (d * (t-text[i]*h)+text[i + m]) % q;
            if (t < 0)
                t = (t + q);
        }
    }
    return indices;
}

int main() {
    string text = "Data structures and algorithms are fun. Algorithms make tasks easier.";
    string pattern = "Algorithms";
    int count = 0;

    int* result = rabinKarpSearch(text, pattern, count);

    cout << "[";
    for (int i = 0; i < count; i++) {
        cout << result[i] << (i < count - 1 ? ", " : "");
    }
    cout << "]" << endl;

    delete[] result;
    return 0;
}
