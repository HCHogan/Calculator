#include <Eigen/Dense>
#include <format>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <string>

#include "calc/calculate.hpp"
#include "calc_int/calculate_int.hpp"

using namespace ftxui;
using Eigen::MatrixXd;

void calculator() {
  std::vector<string> tab_values{
      "normal calculator",
      "matrix calculator",
  };

  int tab_selected = 0;
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  std::string input;
  std::string output;
  Component input_component = Input(&input, "Expression");

  auto component = Container::Vertical({
      input_component,
  });

  auto tab_container = Container::Tab(
      {
          Container::Vertical({
              input_component,
          }),
      },
      &tab_selected);

  auto container = Container::Vertical({
      tab_toggle,
      tab_container,
  });

  auto renderer = Renderer(container, [&] {
    return window(text(" Matrix Calculator ") | bold,
                  vbox({
                      // hbox(text("Answer : ") )
                      tab_toggle->Render(),
                      separator(),
                      hbox(text("Answer : "),
                           text(std::format("{:.6f}", evaluate(input)))),
                      hbox(text("Expression : "), tab_container->Render()),
                  }));
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}

int main() {
  calculator();

  return EXIT_SUCCESS;
}
