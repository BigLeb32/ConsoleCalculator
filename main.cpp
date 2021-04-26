#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <algorithm>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;


// Map of operator's priority
static map<char, int> priority {
    { '+', 0 },
    { '-', 0 },
    { '*', 1 },
    { '/', 1 }
};

bool isOperand(char c)
{
    if (c >= '0' && c <= '9') {
        return true;
    }
    else if (c == '.' || c == ',') {
        return true;
    }
    return false;
}

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/') {
        return true;
    }
    return false;
}

string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

float toFloat(std::string s)
{
    std::replace(s.begin(), s.end(), ',', '.');
    return std::atof(s.c_str());
}

float calc(const string& expr)
{
    std::string expression = removeSpaces(expr);
    std::string pfstring;
    stack<char> charstack;

    // For minus before operand case
    // True when left parenthese appears
    // False when operand appears
    bool newpar = false;

    // Infix to postfix translation
    for (size_t i = 0; i < expression.size(); i++) {
        if (isOperand(expression[i])) {
            newpar = false;
            pfstring += expression[i];
        }
        else if (expression[i] == '(') {
            newpar = true;
            charstack.push(expression[i]);
        }
        else if (expression[i] == ')') {
            while (!charstack.empty() && charstack.top() != '(') {
                pfstring += ' ';
                pfstring += charstack.top(); charstack.pop();
            }
            if (charstack.top() == '(') {
                charstack.pop();
            }
        }
        else if (isOperator(expression[i])) {
            pfstring += ' ';
            if (charstack.empty() || charstack.top() == '(') {
                if (expression[i] == '-' && i == 0) {
                    charstack.push('_');
                }
                else if (expression[i] == '-' && newpar == true) {
                    charstack.push('_');
                }
                else {
                    charstack.push(expression[i]);
                }

            }
            else {
                while (!charstack.empty() && charstack.top() != '('
                    && priority[expression[i]] <= priority[charstack.top()]) {
                    pfstring += charstack.top();
                    charstack.pop();
                }
                charstack.push(expression[i]);
            }
        }
        else {
            throw expression[i];
        }
    }

    while (!charstack.empty()) {
        pfstring += ' ';
        pfstring += charstack.top();
        charstack.pop();
    }

    stack<float> flstack;
    for (size_t i = 0; i < pfstring.size(); i++) {

        static std::string strnumb = "";
        if (isOperand(pfstring[i]) && pfstring[i] != ' ') {
            strnumb +=  pfstring[i];
        }
        else if (pfstring[i] == ' ' && strnumb != "") {
            flstack.push(toFloat(strnumb));
            strnumb = "";
        }

        if (isOperator(pfstring[i]) || pfstring[i] == '_') {
            float x = flstack.top(); flstack.pop();
            float y;
            float res{};
            switch (pfstring[i]) {
                case '_':
                    res = x * -1;
                    break;
                case '+':
                    y = flstack.top(); flstack.pop();
                    res = y + x;
                    break;
                case '-':
                    y = flstack.top(); flstack.pop();
                    res = y - x;
                    break;
                case '*':
                    y = flstack.top(); flstack.pop();
                    res = y * x;
                    break;
                case '/':
                    y = flstack.top(); flstack.pop();
                    res = y / x;
                    break;
                default:
                    break;
            }
            flstack.push(res);
        }
    }

    return roundf(flstack.top() * 100) / 100.0;
}


TEST_CASE("Test of evaluating expressions")
{
    std::string input;
    SECTION("Expression 1") {
        input = "-1 + 5 - 3";
        REQUIRE(calc(input) == 1);
    }

    SECTION("Expression 2") {
        input = "-10 + (8 * 2.5) - (3 / 1,5)";
        REQUIRE(calc(input) == 8);
    }

    SECTION("Expression 3") {
        input = "1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)";
        REQUIRE(calc(input) == 11);
    }

    SECTION("Expression 4") {
        input = "1.1 + 2.1 + abc";
        REQUIRE_THROWS(calc(input));
    }
}

//int main()
//{
//    std::string input;
//    for (;;) {
//        std::getline(std::cin, input);
//        try {
//            cout << calc(input) << endl;
//        }
//        catch (char e) {
//            cout << "некорректный ввод, строка содержит недопустимый символ "
//                << e << endl;
//        }
//    }

//    return 0;
//}


