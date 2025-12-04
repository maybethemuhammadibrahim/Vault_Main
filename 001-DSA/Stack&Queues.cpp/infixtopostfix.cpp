#include <iostream>
#include <stack>
#include <string>

using namespace std;

// Function to return precedence of operators
int prec(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return -1;
}

string infixToPostfix(string s) {
    stack<char> st;
    string result;

    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        // 1. If operand, add to result
        if (isalnum(c)) {
            result += c;
        }
        // 2. If '(', push to stack
        else if (c == '(') {
            st.push('(');
        }
        // 3. If ')', pop until '(' is found
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            st.pop(); // Pop '('
        }
        // 4. If operator
        else {
            // Pop operators with higher or equal precedence
            // (Note: For '^' which is Right Associative, we usually use < instead of <=)
            while (!st.empty() && prec(st.top()) >= prec(c)) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all remaining operators
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }

    return result;
}

int main() {
    string exp = "a+b*(c^d-e)";
    cout << "Infix:   " << exp << endl;
    cout << "Postfix: " << infixToPostfix(exp) << endl;
    // Expected: abcd^e-*+
    return 0;
}
