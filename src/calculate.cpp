#include <cctype>
#include <cmath>
#include <iostream>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <stack>
#include <string>
#include <fmt/format.h>

namespace views = ranges::views;

// 定义运算符的优先级，数字越大优先级越高
int priority(char op) {
  if (op == '(') return 0;
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/') return 2;
  return -1;  // 非法的运算符
}

// 计算两个操作数和一个运算符的结果
double calculate(double a, double b, char op) {
  if (op == '+') return a + b;
  if (op == '-') return a - b;
  if (op == '*') return a * b;
  if (op == '/') return a / b;
  return NAN;  // 非法的运算符
}

// 求中缀表达式的值，参数是一个字符串，返回一个double
double evaluate(std::string expression) {
  std::stack<double> operands;  // 存储操作数的栈
  std::stack<char> operators;   // 存储运算符的栈
  int i = 0;                    // 遍历字符串的索引

  while (i < expression.length()) {
    char c = expression[i];
    if (isspace(c)) {  // 跳过空白字符
      i++;
      continue;
    }
    if (isdigit(c) || c == '.') {  // 读取一个数字
      double num = 0;
      int dot = -1;  // 小数点的位置
      while (i < expression.length() && (isdigit(c) || c == '.')) {
        if (c == '.') {
          if (dot != -1) return NAN;  // 出现多个小数点，表达式不合法
          dot = i;
        } else {
          num = num * 10 + (c - '0');
        }
        i++;
        c = expression[i];
      }
      if (dot != -1) {  // 调整小数的位数
        num = num / pow(10, i - dot - 1);
      }
      operands.push(num);   // 将数字压入操作数栈
    } else if (c == '(') {  // 左括号直接压入运算符栈
      operators.push(c);
      i++;
    } else if (c == ')') {  // 右括号，弹出运算符和操作数，直到遇到左括号
      while (!operators.empty() && operators.top() != '(') {
        char op = operators.top();
        operators.pop();
        if (operands.size() < 2) return NAN;  // 操作数不足，表达式不合法
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        double res = calculate(a, b, op);
        if (std::isnan(res)) return NAN;  // 出现非法的运算符，表达式不合法
        operands.push(res);               // 将结果压入操作数栈
      }
      if (operators.empty()) return NAN;  // 没有匹配的左括号，表达式不合法
      operators.pop();                    // 弹出左括号
      i++;
    } else {  // 其他字符，视为运算符
      while (!operators.empty() && priority(operators.top()) >= priority(c)) {
        // 当栈顶运算符的优先级不低于当前运算符时，弹出运算符和操作数，计算结果
        char op = operators.top();
        operators.pop();
        if (operands.size() < 2) return NAN;  // 操作数不足，表达式不合法
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        double res = calculate(a, b, op);
        if (std::isnan(res)) return NAN;  // 出现非法的运算符，表达式不合法
        operands.push(res);               // 将结果压入操作数栈
      }
      operators.push(c);  // 将当前运算符压入运算符栈
      i++;
    }
  }
  // 遍历完字符串后，依次弹出运算符和操作数，计算结果
  while (!operators.empty()) {
    char op = operators.top();
    operators.pop();
    if (operands.size() < 2) return NAN;  // 操作数不足，表达式不合法
    double b = operands.top();
    operands.pop();
    double a = operands.top();
    operands.pop();
    double res = calculate(a, b, op);
    if (std::isnan(res)) return NAN;  // 出现非法的运算符，表达式不合法
    operands.push(res);               // 将结果压入操作数栈
  }
  if (operands.size() != 1) return NAN;  // 操作数多余，表达式不合法
  return operands.top();                 // 返回最终的结果
}

// ------ tests ------
int mytest1() {
  // 测试用例
  std::string test[] = {
      "(1 + (2 + 3.0) * 4.0) / 2.5", "1 + 2 * 3 - 4 / 5",
      "1 + 2 * (3 - 4 / 5)",         "1 + 2 * (3 - 4 / 5))",
      "1 + 2 * (3 - 4 / 5.0",        "1 + 2 * (3 - 4 / 5.0)",
      "1 + 2 * (3 - 4 / 5.0) + .",   "1 + 2 * (3 - 4 / 5.0) + .5"};
  for (auto [i, s] : test | views::reverse | views::enumerate) {
    std::cout << fmt::format("[{}] {} = {}\n", i, s, evaluate(s));
  }
  return 0;
}
