#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <Eigen/Dense>
#include <iostream>
#include <sstream>
#include <vector>

using namespace ftxui;
using namespace Eigen;

// A component to display a matrix.
class MatrixComponent : public Component {
 public:
  MatrixComponent() {
    Add(&input_);
    input_.on_change = [this] { ParseInput(); };
    input_.on_enter = [this] { EmitEvent(Event::Custom); };
  }

  // Parse the input string and convert it to a matrix.
  void ParseInput() {
    std::istringstream iss(input_.content);
    std::vector<double> values;
    int rows = 0;
    int cols = 0;
    std::string line;
    while (std::getline(iss, line)) {
      rows++;
      std::istringstream iss2(line);
      double value;
      int count = 0;
      while (iss2 >> value) {
        values.push_back(value);
        count++;
      }
      if (rows == 1) {
        cols = count;
      } else if (cols != count) {
        // Invalid input, different number of columns.
        matrix_ = MatrixXd();
        return;
      }
    }
    if (values.empty()) {
      // Empty input, no matrix.
      matrix_ = MatrixXd();
      return;
    }
    // Create the matrix from the values.
    matrix_ = Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(values.data(), rows, cols);
  }

  // Convert the matrix to a string.
  std::string MatrixToString() {
    std::ostringstream oss;
    oss << matrix_;
    return oss.str();
  }

  // Return the matrix.
  MatrixXd GetMatrix() {
    return matrix_;
  }

  // Set the matrix and update the input.
  void SetMatrix(const MatrixXd& matrix) {
    matrix_ = matrix;
    input_.content = MatrixToString();
    input_.cursor_position = input_.content.size();
  }

  // Draw the component.
  Element Render() override {
    return window(text("Matrix") | hcenter | bold,
                  input_.Render() | hcenter | flex) |
           border;
  }

  // Handle keyboard events.
  bool OnEvent(Event event) override {
    if (event.is_mouse())
      return false;
    input_.OnEvent(event);
    return true;
  }

 private:
  Input input_;   // The input component.
  MatrixXd matrix_; // The matrix.
};

// A component to display the history of calculations.
class HistoryComponent : public Component {
 public:
  HistoryComponent() {
    Add(&container_);
    container_.Add(&text_);
    container_.Add(&clear_button_);
    clear_button_.on_click = [this] { Clear(); };
  }

  // Add a new entry to the history.
  void AddEntry(const std::string& entry) {
    history_.push_back(entry);
    UpdateText();
  }

  // Clear the history.
  void Clear() {
    history_.clear();
    UpdateText();
  }

  // Update the text component with the history.
  void UpdateText() {
    std::string content;
    for (const auto& entry : history_) {
      content += entry + "\n";
    }
    text_.SetText(content);
  }

  // Draw the component.
  Element Render() override {
    return window(text("History") | hcenter | bold,
                  hbox(text_ | yflex | scroll, clear_button_.Render() | size(WIDTH, EQUAL, 10))) |
           border;
  }

  // Handle keyboard events.
  bool OnEvent(Event event) override {
    if (event.is_mouse())
      return false;
    container_.OnEvent(event);
    return true;
  }

 private:
  Container container_;   // The container component.
  Text text_;             // The text component to display the history.
  Button clear_button_;   // The button component to clear the history.
  std::vector<std::string> history_; // The history of calculations.
};

// A component to perform matrix calculations.
class CalculatorComponent : public Component {
 public:
  CalculatorComponent() {
    Add(&container_);
    container_.Add(&matrix_a_);
    container_.Add(&matrix_b_);
    container_.Add(&operation_);
    container_.Add(&result_);
    container_.Add(&history_);
    operation_.Add(&add_button_);
    operation_.Add(&sub_button_);
    operation_.Add(&mul_button_);
    operation_.Add(&trans_button_);
    operation_.Add(&inv_button_);
    operation_.Add(&det_button_);
    operation_.Add(&rank_button_);
    operation_.Add(&trace_button_);
    operation_.Add(&eigen_button_);
    add_button_.on_click = [this] { Add(); };
    sub_button_.on_click = [this] { Sub(); };
    mul_button_.on_click = [this] { Mul(); };
    trans_button_.on_click = [this] { Trans(); };
    inv_button_.on_click = [this] { Inv(); };
    det_button_.on_click = [this] { Det(); };
    rank_button_.on_click = [this] { Rank(); };
    trace_button_.on_click = [this] { Trace(); };
    eigen_button_.on_click = [this] { Eigen(); };
    matrix_a_.on_enter = [this] { matrix_b_.TakeFocus(); };
    matrix_b_.on_enter = [this] { result_.SetMatrix(MatrixXd()); };
  }

  // Perform the addition operation.
  void Add() {
    MatrixXd a = matrix_a_.GetMatrix();
    MatrixXd b = matrix_b_.GetMatrix();
    if (a.rows() == 0 || b.rows() == 0) {
      // One of the matrices is empty, do nothing.
      return;
    }
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
      // The matrices have different dimensions, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot add matrices with different dimensions.");
      return;
    }
    // Perform the addition and show the result.
    MatrixXd c = a + b;
    result_.SetMatrix(c);
    history_.AddEntry("A + B =\n" + result_.MatrixToString());
  }

  // Perform the subtraction operation.
  void Sub() {
    MatrixXd a = matrix_a_.GetMatrix();
    MatrixXd b = matrix_b_.GetMatrix();
    if (a.rows() == 0 || b.rows() == 0) {
      // One of the matrices is empty, do nothing.
      return;
    }
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
      // The matrices have different dimensions, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot subtract matrices with different dimensions.");
      return;
    }
    // Perform the subtraction and show the result.
    MatrixXd c = a - b;
    result_.SetMatrix(c);
    history_.AddEntry("A - B =\n" + result_.MatrixToString());
  }

  // Perform the multiplication operation.
  void Mul() {
    MatrixXd a = matrix_a_.GetMatrix();
    MatrixXd b = matrix_b_.GetMatrix();
    if (a.rows() == 0 || b.rows() == 0) {
      // One of the matrices is empty, do nothing.
      return;
    }
    if (a.cols() != b.rows()) {
      // The matrices have incompatible dimensions, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot multiply matrices with incompatible dimensions.");
      return;
    }
    // Perform the multiplication and show the result.
    MatrixXd c = a * b;
    result_.SetMatrix(c);
    history_.AddEntry("A * B =\n" + result_.MatrixToString());
  }

  // Perform the transpose operation.
  void Trans() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    // Perform the transpose and show the result.
    MatrixXd b = a.transpose();
    result_.SetMatrix(b);
    history_.AddEntry("A^T =\n" + result_.MatrixToString());
  }

  // Perform the inverse operation.
  void Inv() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    if (a.rows() != a.cols()) {
      // The matrix is not square, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot invert a non-square matrix.");
      return;
    }
    if (a.determinant() == 0) {
      // The matrix is singular, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot invert a singular matrix.");
      return;
    }
    // Perform the inverse and show the result.
    MatrixXd b = a.inverse();
    result_.SetMatrix(b);
    history_.AddEntry("A^-1 =\n" + result_.MatrixToString());
  }

  // Perform the determinant operation.
  void Det() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    if (a.rows() != a.cols()) {
      // The matrix is not square, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot compute the determinant of a non-square matrix.");
      return;
    }
    // Perform the determinant and show the result.
    double b = a.determinant();
    std::ostringstream oss;
    oss << b;
    result_.SetMatrix(MatrixXd());
    history_.AddEntry("det(A) = " + oss.str());
  }

  // Perform the rank operation.
  void Rank() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    // Perform the rank and show the result.
    int b = a.rank();
    std::ostringstream oss;
    oss << b;
    result_.SetMatrix(MatrixXd());
    history_.AddEntry("rank(A) = " + oss.str());
  }

  // Perform the trace operation.
  void Trace() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    if (a.rows() != a.cols()) {
      // The matrix is not square, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot compute the trace of a non-square matrix.");
      return;
    }
    // Perform the trace and show the result.
    double b = a.trace();
    std::ostringstream oss;
    oss << b;
    result_.SetMatrix(MatrixXd());
    history_.AddEntry("tr(A) = " + oss.str());
  }

  // Perform the eigenvalues and eigenvectors operation.
  void Eigen() {
    MatrixXd a = matrix_a_.GetMatrix();
    if (a.rows() == 0) {
      // The matrix is empty, do nothing.
      return;
    }
    if (a.rows() != a.cols()) {
      // The matrix is not square, show an error message.
      result_.SetMatrix(MatrixXd());
      history_.AddEntry("Error: Cannot compute the eigenvalues and eigenvectors of a non-square matrix.");
      return;
    }
    // Perform the eigenvalues and eigenvectors and show the result.
    EigenSolver<MatrixXd> es(a);
    MatrixXd b = es.eigenvalues().real().asDiagonal();
    MatrixXd c = es.eigenvectors().real();
    result_.SetMatrix(b);
    history_.AddEntry("Eigenvalues of A =\n" + result_.MatrixToString());
    result_.SetMatrix(c);
    history_.AddEntry("Eigenvectors of A =\n" + result_.MatrixToString());
  }

  // Draw the component.
  Element Render() override {
    return window(text("Calculator") | hcenter | bold,
                  hbox(vbox(matrix_a_.Render() | size(WIDTH, EQUAL, 30),
                            matrix_b_.Render() | size(WIDTH, EQUAL, 30)),
                       vbox(operation_.Render() | size(WIDTH, EQUAL, 10),
                            result_.Render() | size(WIDTH, EQUAL, 30)))) |
           border;
  }

  // Handle keyboard events.
  bool OnEvent(Event event) override {
    if (event.is_mouse())
      return false;
    container_.OnEvent(event);
    return true;
  }

 private:
  Container container_;     // The container component.
  MatrixComponent matrix_a_; // The component for matrix A.
  MatrixComponent matrix_b_; // The component for matrix B.
  Container operation_;     // The container for the operation buttons.
  Button add_button_;       // The button for the addition operation.
  Button sub_button_;       // The button for the subtraction operation.
  Button mul_button_;       // The button for the multiplication operation.
  Button trans_button_;     // The button for the transpose operation.
  Button inv_button_;       // The button for the inverse operation.
  Button det_button_;       // The button for the determinant operation.
  Button rank_button_;      // The button for the rank operation.
  Button trace_button_;     // The button for the trace operation.
  Button eigen_button_;     // The button for the eigenvalues and eigenvectors operation.
  MatrixComponent result_;  // The component for the result matrix.
  HistoryComponent history_; // The component for the history of calculations.
};

// The main function.
int main() {
  // Create the calculator component.
  CalculatorComponent calculator;

  // Create the screen interactive and start the event loop.
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(&calculator);

  return 0;
}

