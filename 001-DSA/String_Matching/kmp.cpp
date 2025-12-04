#include <iostream>
#include <string>

using namespace std;

void computeLPSArray(string pattern, int* lps) {
    int m = pattern.length();
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < m){
        if (pattern[i] == pattern[len]){
            len++;
            lps[i] = len;
            i++;
        } 
        else{
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int* kmpSearch(string text, string pattern, int& count){
    int n = text.length();
    int m = pattern.length();
    
    int* lps = new int[m];
    computeLPSArray(pattern, lps);

    int* indices = new int[n];
    count = 0;
    
    int i = 0; 
    int j = 0; 

    while (i < n) {
        if (pattern[j] == text[i]){
            j++;
            i++;
        }

        if (j==m) {
            indices[count++] = i-j;
            j = lps[j - 1];
        } 
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0)
                j = lps[j-1];
            else
                i++;
        }
    }
    
    delete[] lps;
    return indices;
}

int main() {
    string text = "ABABABC";
    string pattern = "ABABC";
    int count = 0;

    int* result = kmpSearch(text, pattern, count);

    cout << "Pattern found at idx";
    for (int i = 0; i < count; i++) {
        cout << result[i] << (i < count - 1 ? ", " : "");
    }
    cout << endl;

    delete[] result;
    return 0;
}
