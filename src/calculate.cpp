#include <ranges>
#include <string>
#include <stack>
#include <cmath>
using namespace std;

// 定义一个优先级函数，返回运算符的优先级
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 定义一个计算函数，根据运算符对两个操作数进行计算
double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    return nan("");
}

// 定义一个求表达式值的函数，接受一个std::string作为输入，返回值为double
double evaluate(string expression) {
    // 定义两个栈，一个用于存储操作数，一个用于存储运算符
    stack<double> operands;
    stack<char> operators;

    // 遍历表达式的每个字符
    // for (int i = 0; i < expression.size(); i++) {
    for (auto [i, c] : ranges::zip_view(views::iota(0, static_cast<int>(expression.size())), expression | views::all)) {

        // 如果是空格，跳过
        if (c == ' ') continue;

        // 如果是数字，解析出完整的数字，并压入操作数栈
        if (isdigit(c) || c == '.') {
            double num = 0;
            int dot = 0; // 记录小数点的位置
            while (i < expression.size() && (isdigit(c) || c == '.')) {
                if (c == '.') {
                    dot = i;
                } else {
                    num = num * 10 + (c - '0');
                }
                i++;
            }
            i--; // 回退一位，因为循环结束时多加了一位
            if (dot > 0) { // 如果有小数点，需要除以相应的10的幂
                num = num / pow(10, i - dot);
            }
            operands.push(num);
        }

        // 如果是左括号，压入运算符栈
        else if (c == '(') {
            operators.push(c);
        }

        // 如果是右括号，弹出运算符栈，直到遇到左括号，对应的操作数进行计算，并压入操作数栈
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = operands.top(); operands.pop();
                double a = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                operands.push(calculate(a, b, op));
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop(); // 弹出左括号
            } else {
                return nan(""); // 如果没有匹配的左括号，表达式不合法，返回nan
            }
        }

        // 如果是运算符，比较其与运算符栈顶的优先级，如果高于或等于栈顶，压入运算符栈
        // 如果低于栈顶，弹出运算符栈，对应的操作数进行计算，并压入操作数栈，重复此过程，直到栈顶优先级低于当前运算符，或栈为空，或遇到左括号
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!operators.empty() && operators.top() != '(' && priority(c) <= priority(operators.top())) {
                double b = operands.top(); operands.pop();
                double a = operands.top(); operands.pop();
                char op = operators.top(); operators.pop();
                operands.push(calculate(a, b, op));
            }
            operators.push(c); // 压入当前运算符
        }

        // 如果是其他字符，表达式不合法，返回nan
        else {
            return nan("");
        }
    }

    // 遍历完表达式后，如果运算符栈不为空，依次弹出运算符栈，对应的操作数进行计算，并压入操作数栈，直到运算符栈为空
    while (!operators.empty()) {
        double b = operands.top(); operands.pop();
        double a = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();
        operands.push(calculate(a, b, op));
    }

    // 如果操作数栈只有一个元素，那么就是表达式的值，返回它
    if (operands.size() == 1) {
        return operands.top();
    }

    // 否则，表达式不合法，返回nan
    return nan("");
}

// 定义一个测试函数，用于测试evaluate函数是否正确
// void test(string expression) {
//     cout << "Expression: " << expression << endl;
//     cout << "Value: " << evaluate(expression) << endl;
//     cout << "------------------------" << endl;
// }
