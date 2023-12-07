#include <fmt/format.h>

#include <calc/calc_matrix.hpp>
#include <eigen3/Eigen/Dense>
#include <string_view>

int evaluate_matrix(std::string_view expression) {
  std::istringstream iss(expression.data());
  std::vector<double> values;
  int rows = 0;
  int cols = 0;

  for (std::string line; std::getline(iss, line);) {
    std::istringstream iss_line(line);
    cols = 0;
    for (double value; iss_line >> value;) {
      values.push_back(value);
      cols++;
    }
    rows++;
  }

  Eigen::MatrixXd matrix(rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix(i, j) = values[i * cols + j];
    }
  }
  return 0;
}

Eigen::MatrixXd parseStringToMatrix(const std::string &input) {
  std::istringstream iss(input);
  std::vector<double> values;
  std::string line;
  int cols = 0;
  int rows = 0;

  while (std::getline(iss, line)) {
    std::istringstream lineStream(line);
    double value;
    int lineCount = 0;
    while (lineStream >> value) {
      values.push_back(value);
      lineCount++;
    }
    if (rows == 0) {
      cols = lineCount;
    } else if (lineCount != cols) {
      throw std::runtime_error("Invalid matrix format");
    }
    rows++;
  }

  if (values.size() != rows * cols) {
    throw std::runtime_error("Invalid matrix format");
  }

  Eigen::MatrixXd result(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result(i, j) = values[i * cols + j];
    }
  }

  return result;
}

Eigen::MatrixXd performMatrixOperation(const std::string &matrix1,
                                       const std::string &matrix2,
                                       MatrixOp op) {
  Eigen::MatrixXd m1 = parseStringToMatrix(matrix1);
  Eigen::MatrixXd m2 = parseStringToMatrix(matrix2);

  if ((m1.rows() != m2.rows() || m1.cols() != m2.cols()) &&
      (op == MatrixOp::ADD || op == MatrixOp::SUBTRACT)) {
    throw std::runtime_error("Matrices are not the same size");
  }
  if (op == MatrixOp::MULTIPLY && m1.cols() != m2.cols()) {
    throw std::runtime_error("Matrices are not the same size");
  }

  switch (op) {
    case MatrixOp::ADD:
      return m1 + m2;
    case MatrixOp::SUBTRACT:
      return m1 - m2;
    case MatrixOp::MULTIPLY:
      return m1.cwiseProduct(m2);
    case MatrixOp::TRANSPOSE:
      return m1.transpose();
    default:
      throw std::runtime_error("Invalid matrix operation");
  }
}

Eigen::MatrixXd performMatrixOperationProxy(const std::string &matrix1,
                                            const std::string &matrix2,
                                            MatrixOp op) {
  try {
    return performMatrixOperation(matrix1, matrix2, op);
  } catch (const std::exception &e) {
    return Eigen::MatrixXd::Zero(1, 1);
  }
}

MatrixOp parseIntToMatrixOp(int op) {
  switch (op) {
    case 0:
      return MatrixOp::ADD;
    case 1:
      return MatrixOp::SUBTRACT;
    case 2:
      return MatrixOp::MULTIPLY;
    case 3:
      return MatrixOp::TRANSPOSE;
    default:
      throw std::runtime_error("Invalid matrix operation");
  }
}

std::string toString(const Eigen::MatrixXd &matrix) {
  std::stringstream ss;
  for (int i = 0; i < matrix.rows(); ++i) {
    for (int j = 0; j < matrix.cols(); ++j) {
      ss << fmt::format("{:.2f} ", matrix(i, j));
    }
    ss << "\n";
  }
  return ss.str();
}
