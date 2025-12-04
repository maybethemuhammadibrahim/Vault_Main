#include <iostream>
#include <stack>
#include <string>
#include <cmath> // For pow()

using namespace std;

int evaluatePostfix(string exp) {
    stack<int> st;

    for (char c : exp) {
        // If operand, push to stack
        if (isdigit(c)) {
            st.push(c - '0');
        }
        // If operator, pop two elements
        else {
            int val2 = st.top(); st.pop();
            int val1 = st.top(); st.pop();

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
    string exp = "231*+9-"; // Equivalent to: 2 + (3 * 1) - 9
    cout << "Postfix Evaluation: " << evaluatePostfix(exp) << endl; 
    // Output should be -4
    return 0;
}
