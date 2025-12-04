#include <iostream>
#include <string>

using namespace std;

void computeLPSArray(string pattern, int* lps) {
    int m = pattern.length();
    
    // 'len' tracks the length of the previous longest prefix suffix
    // It also acts as the index for the prefix we are currently comparing against
    int len = 0; 

    // The LPS of the first character is always 0 (a single char has no proper prefix)
    lps[0] = 0; 
    
    // Start calculating from the second character (index 1)
    int i = 1;

    while (i < m) {
        // CASE 1: Match Found
        // If the character at the current index 'i' matches the character
        // at the current prefix length 'len'
        if (pattern[i] == pattern[len]) {
            len++;         // We found a longer prefix-suffix, increment length
            lps[i] = len;  // Store this length at the current index
            i++;           // Move to the next character in the pattern
        } 
        // CASE 2: Mismatch
        else {
            // CASE 2A: We can fall back to a shorter prefix
            // If len is not 0, it means we were matching a prefix, but it just broke.
            // We check if there is a *shorter* prefix within the previous match 
            // that we can continue from.
            if (len != 0) {
                // CRITICAL STEP:
                // We do NOT increment 'i' here.
                // We look at the LPS value of the previous character (len - 1)
                // to find the next longest prefix to try.
                len = lps[len - 1];
            } 
            // CASE 2B: No prefix to fall back to
            // If len is 0, we can't go back any further.
            else {
                lps[i] = 0; // No prefix matches
                i++;        // Move to the next character
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
