#include <iostream>
#include <string>

using namespace std;

// ALPHABET_SIZE: Number of characters in the input alphabet (256 for ASCII)
const int ALPHABET_SIZE = 256;
// PRIME_MODULUS: A prime number to reduce hash values (minimizes collisions)
const int PRIME_MODULUS = 101;

/*
 * text: The main string to search in
 * pattern: The string to look for
 * count: Reference variable to store number of matches found
 */
int* rabinKarpSearch(const string& text, const string& pattern, int& count) {
    int textLen = text.length();
    int patLen = pattern.length();
    
    // Array to store starting indices of matches (Max possible matches is textLen)
    int* indices = new int[textLen];
    count = 0;
    
    int i, j;
    int patternHash = 0; // Hash value for pattern
    int windowHash = 0;  // Hash value for current window of text
    int leadingDigitWeight = 1; // The value of h = pow(d, m-1) % q

    // 1. Calculate leadingDigitWeight = pow(ALPHABET_SIZE, patLen-1) % PRIME_MODULUS
    // We need this to remove the "leading" character from the rolling hash efficiently
    for (i = 0; i < patLen - 1; i++)
        leadingDigitWeight = (leadingDigitWeight * ALPHABET_SIZE) % PRIME_MODULUS;

    // 2. Calculate the hash value of pattern and first window of text
    for (i = 0; i < patLen; i++) {
        patternHash = (ALPHABET_SIZE * patternHash + pattern[i]) % PRIME_MODULUS;
        windowHash = (ALPHABET_SIZE * windowHash + text[i]) % PRIME_MODULUS;
    }

    // 3. Slide the pattern over text one by one
    for (i = 0; i <= textLen - patLen; i++) {

        // Check the hash values of current window of text and pattern.
        // If the hash values match, then only check for characters one by one.
        if (patternHash == windowHash) {
            bool matchFound = true;
            // Check for characters one by one (handle hash collisions)
            for (j = 0; j < patLen; j++) {
                if (text[i + j] != pattern[j]) {
                    matchFound = false;
                    break;
                }
            }
            
            // If hashes match and characters match, it is a confirmed match
            if (matchFound) {
                indices[count++] = i;
            }
        }

        // 4. Calculate hash value for NEXT window of text: Remove leading digit, add trailing digit
        if (i < textLen - patLen) {
            // Formula: newHash = (d * (oldHash - removeChar * h) + addChar) % q
            
            // Step A: Remove leading character (text[i])
            // We subtract (text[i] * leadingDigitWeight) because text[i] was multiplied by d^(m-1) in the previous hash
            windowHash = (ALPHABET_SIZE * (windowHash - text[i] * leadingDigitWeight) + text[i + patLen]) % PRIME_MODULUS;

            // Step B: Handle negative values resulting from modulo subtraction
            // In C++, -5 % 101 might return -5, but we want 96.
            if (windowHash < 0)
                windowHash = (windowHash + PRIME_MODULUS);
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
