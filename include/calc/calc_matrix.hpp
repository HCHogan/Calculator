#include <eigen3/Eigen/Dense>

enum class MatrixOp {
    ADD,
    SUBTRACT,
    MULTIPLY,
    TRANSPOSE,
};

Eigen::MatrixXd performMatrixOperation(const std::string& matrix1, const std::string& matrix2, MatrixOp op);
Eigen::MatrixXd performMatrixOperationProxy(const std::string& matrix1, const std::string& matrix2, MatrixOp op);
MatrixOp parseIntToMatrixOp(int op);
std::string toString(const Eigen::MatrixXd &mat);
