#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Parser {
    string expr;
    size_t pos = 0;

public:
    Parser(const string &expression) : expr(expression) {}

    // Entry point
    double parse() {
        pos = 0;
        return parseExpression();
    }

private:
    // Parses addition and subtraction
    double parseExpression() {
        double value = parseTerm();
        while (pos < expr.length()) {
            if (expr[pos] == '+') {
                pos++;
                value += parseTerm();
            } else if (expr[pos] == '-') {
                pos++;
                value -= parseTerm();
            } else {
                break;
            }
        }
        return value;
    }

    // Parses multiplication and division
    double parseTerm() {
        double value = parseFactor();
        while (pos < expr.length()) {
            if (expr[pos] == '*') {
                pos++;
                value *= parseFactor();
            } else if (expr[pos] == '/') {
                pos++;
                double divisor = parseFactor();
                if (divisor == 0) {
                    throw runtime_error("Division by zero!");
                }
                value /= divisor;
            } else {
                break;
            }
        }
        return value;
    }

    // Parses numbers or parenthesized expressions
    double parseFactor() {
        skipWhitespace();

        if (expr[pos] == '(') {
            pos++;
            double value = parseExpression();
            if (expr[pos] == ')') {
                pos++;
                return value;
            } else {
                throw runtime_error("Expected closing parenthesis");
            }
        } else {
            return parseNumber();
        }
    }

    // Parses a number
    double parseNumber() {
        skipWhitespace();
        double number = 0;
        size_t start = pos;

        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.')) {
            pos++;
        }

        if (start == pos)
            throw runtime_error("Expected number");

        return stod(expr.substr(start, pos - start));
    }

    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) pos++;
    }
};

int main() {
    string input;

    cout << "=== Arithmetic Expression Evaluator ===\n";
    cout << "Enter expressions like: 3 + 4 * (2 - 1)\n";
    cout << "Type 'exit' to quit.\n\n";

    while (true) {
        cout << ">> ";
        getline(cin, input);

        if (input == "exit")
            break;

        try {
            Parser parser(input);
            double result = parser.parse();
            cout << "Result: " << result << "\n";
        } catch (const exception &e) {
            cerr << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
