#include <iostream>
#include <string>

using namespace std;

// d is the number of characters in the input alphabet (256 for ASCII)
const int d = 256;
// q is a prime number to reduce hash values (minimizes collisions)
const int q = 101;

/*
 * text: The main string to search in
 * pattern: The string to look for
 * count: Reference variable to store number of matches found
 */
int* rabinKarpSearch(const string& text, const string& pattern, int& count) {
    int n = text.length();
    int m = pattern.length();
    
    // Array to store starting indices of matches (Max possible matches is n)
    int* indices = new int[n];
    count = 0;
    
    int i, j;
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for current window of text
    int h = 1; // The value of h = pow(d, m-1) % q

    // 1. Calculate h = pow(d, m-1) % q
    // We need this to remove the "leading" character from the rolling hash
    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    // 2. Calculate the hash value of pattern and first window of text
    for (i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // 3. Slide the pattern over text one by one
    for (i = 0; i <= n - m; i++) {

        // Check the hash values of current window of text and pattern.
        // If the hash values match, then only check for characters one by one.
        if (p == t) {
            bool matchFound = true;
            // Check for characters one by one (handle hash collisions)
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    matchFound = false;
                    break;
                }
            }
            
            // If p == t and characters match, it is a confirmed match
            if (matchFound) {
                indices[count++] = i;
            }
        }

        // 4. Calculate hash value for NEXT window of text: Remove leading digit, add trailing digit
        if (i < n - m) {
            // Formula: t_new = (d * (t_old - text[i] * h) + text[i+m]) % q
            
            // Step A: Remove leading character (text[i])
            // We subtract (text[i] * h) because text[i] was multiplied by d^(m-1) in the previous hash
            t = (d * (t - text[i] * h) + text[i + m]) % q;

            // Step B: Handle negative values resulting from modulo subtraction
            // In C++, -5 % 101 might return -5, but we want 96.
            if (t < 0)
                t = (t + q);
        }
    }
    return indices;
}

int main() {
    // Note: 'Algorithms' vs 'algorithms' (Case sensitive)
    string text = "Data structures and Algorithms are fun. Algorithms make tasks easier.";
    string pattern = "Algorithms";
    int count = 0;

    int* result = rabinKarpSearch(text, pattern, count);

    cout << "Pattern: \"" << pattern << "\" found at indices: [";
    for (int i = 0; i < count; i++) {
        cout << result[i] << (i < count - 1 ? ", " : "");
    }
    cout << "]" << endl;

    // Don't forget to free the memory!
    delete[] result;
    return 0;
}
