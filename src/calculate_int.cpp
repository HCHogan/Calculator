#include <cstdint>
#include <iostream>
#include <format>
#include <limits>
#include <optional>
#include <ranges>
#include <regex>
#include <stack>
#include <string>
#include <vector>
#include <tuple>

namespace views = std::views;
namespace ranges = std::ranges;

using std::nullopt;
using std::optional;
using std::string;
using std::string_view;
using std::stack;
using std::tuple;

static const string SYMBOLS = "\\+|-|\\*|/|\\(|\\)";
static const string LEFT = "(";
static const string RIGHT = ")";
static const string ADD = "+";
static const string MINUS = "-";
static const string TIMES = "*";
static const string DIVISION = "/";

static const int LEVEL_01 = 1;
static const int LEVEL_02 = 2;
static const int LEVEL_HIGH = std::numeric_limits<int>::max();

auto priority_int(char op) -> int {
  if (op == '+' || op == '-') return LEVEL_01;
  if (op == '*' || op == '/') return LEVEL_02;
  if (op == '(' || op == ')') return LEVEL_HIGH;
  return LEVEL_HIGH;
}

auto compute_int(double num1, double num2, char op) -> double {
  if (op == '+') return num1 + num2;
  if (op == '-') return num1 - num2;
  if (op == '*') return num1 * num2;
  if (op == '/') return num1 / num2;
  return nan("");
}

auto scan_int(string_view expr) -> tuple<stack<double>, stack<char>> {
  stack<double> operands;
  stack<char> operators;

  for (auto [i, c] : ranges::zip_view(views::iota(0, static_cast<int>(expr.size())), expr | views::all)) {
    if (c == ' ') continue;
    
    if (isdigit(c)) {
      double num = 0;
      int dot = 0;
      while (i < expr.size() && (isdigit(c) || c == '.')) {
        if (c == '.') {
          dot = i;
        } else {
          num = num * 10 + (c - '0');
        }
        i++;
      }
      i--;
      if (dot > 0) {
        num = num / pow(10, i - dot);
      }
      operands.push(num);
    } else if (c == '(') {
      operators.push(c);
    } else if (c == ')') {
      while (operators.top() != '(') {
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        char op = operators.top();
        operators.pop();
        operands.push(compute_int(a, b, op));
      }
      operators.pop();
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      while (!operators.empty() && priority_int(c) <= priority_int(operators.top())) {
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();
        char op = operators.top();
        operators.pop();
        operands.push(compute_int(a, b, op));
      }
      operators.push(c);
    }
  }

  return {operands, operators};
}

auto evaluate_int(string expr) -> optional<int> { 
  auto [operands, operators] = scan_int(expr);
  std::cout << std::format("operands: {}\n", operands);
  std::cout << std::format("operators: {}\n", operators);
  return nullopt; 
}

