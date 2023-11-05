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
      operands.push(c - '0');
    } else if (std::regex_match(string(1, c), std::regex(SYMBOLS))) {
      if (priority_int(c) > priority_int(operators.top())) {
        operators.push(c);
      } else {
        while (!operators.empty() && priority_int(c) <= priority_int(operators.top())) {
          double num2 = operands.top();
          operands.pop();
          double num1 = operands.top();
          operands.pop();
          char op = operators.top();
          operators.pop();
          double res = compute_int(num1, num2, op);
          operands.push(res);
        }
      }
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

