#include <iostream>
#include <stack>
#include <string>
#include <cmath> // For pow()

using namespace std;

int evaluatePrefix(string exp) {
    stack<int> st;

    // Iterate backwards (Right to Left)
    for (int i = exp.length() - 1; i >= 0; i--) {
        char c = exp[i];

        // If operand, push to stack
        if (isdigit(c)) {
            st.push(c - '0');
        }
        // If operator, pop two elements
        else {
            int val1 = st.top(); st.pop();
            int val2 = st.top(); st.pop();

            switch (c) {
                case '+': st.push(val1 + val2); break;
                case '-': st.push(val1 - val2); break;
                case '*': st.push(val1 * val2); break;
                case '/': st.push(val1 / val2); break;
                case '^': st.push(pow(val1, val2)); break;
            }
        }
    }
    return st.top();
}

int main() {
    string exp = "-+2*319"; // Equivalent to: (2 + (3 * 1)) - 9
    cout << "Prefix Evaluation: " << evaluatePrefix(exp) << endl;
    // Output should be -4
    return 0;
}
