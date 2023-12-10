#include <fmt/format.h>

#include <calc/calc_matrix.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

Element MatrixDisplayer(const Eigen::MatrixXd& matrix) {
  Elements list;
  for (int i = 0; i < matrix.rows(); ++i) {
    Elements row;
    for (int j = 0; j < matrix.cols(); ++j) {
      row.push_back(text(std::format("{:.2f}  ", matrix(i, j))));
    }
    list.push_back(hbox(std::move(row)));
  }
  return vbox(std::move(list));
}

int main() {
  std::string matrix1;
  std::string matrix2;
  std::string result;
  std::vector<std::string> operations = {"add", "subtract", "multiply", "transpose"};
  int selected_operation = 0;

  auto matrix1_input = Input(&matrix1, "Enter matrix 1");
  auto matrix2_input = Input(&matrix2, "Enter matrix 2");
  auto operation_selector = Container::Vertical({
      Radiobox(&operations, &selected_operation),
  });

  auto container = Container::Horizontal({
      matrix1_input,
      matrix2_input,
      operation_selector,
  });

  auto renderer = Renderer(container, [&] {
    return window(
        text(" Matrix Calculator ") | bold,
        vbox({
            hbox({
              matrix1_input->Render(),
              separator(),
              matrix2_input->Render(),
              separator(),
              operation_selector->Render(),
            }),
            separator(),
            vbox(MatrixDisplayer(performMatrixOperationProxy(
                matrix1, matrix2, parseIntToMatrixOp(selected_operation)))),
        }));
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);
}
