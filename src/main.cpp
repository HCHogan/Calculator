#include "calculate.hpp"
#include "calculate_int.hpp"
#include <Eigen/Dense>
#include <format>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <ranges>
#include <string>

using namespace ftxui;
using Eigen::MatrixXd;
// using std::cout;
// using std::format;
namespace views = std::views;
namespace ranges = std::ranges;

class MatrixComponent : public Component {
public:
};

void tests() {

  MatrixXd m(2, 2);
  m(0, 0) = 3;
  m(1, 0) = 2.5;
  m(0, 1) = -1;
  m(1, 1) = m(1, 0) + m(0, 1);
  std::cout << m << std::endl;

  Element document = hbox({
      text("left") | border,
      text("middle") | border | flex,
      text("right") | border,
  });

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));

  Render(screen, document);
  screen.Print();

  // auto button = Button("Click me", [] { std::cout << "Button clicked\n"; });
  // // auto mytext = text("Hello world");
  //
  // auto container = Container::Vertical({button});

  auto screen2 = Screen::Create(Dimension::Fixed(32), Dimension::Fixed(10));

  auto &pixel = screen2.PixelAt(9, 9);
  pixel.character = U'A';
  pixel.bold = true;
  pixel.foreground_color = Color::Blue;

  // std::cout << screen2.ToString();

  Element document2 =
      hbox({window(text("The title"),
                   vbox({hbox({text("The left inner text"), separator(),
                               text("The right inner text")}) |
                             flex,
                         gauge(0.5) | border})) |
            flex});
  auto empty_screen = Screen::Create(Dimension::Full(), Dimension::Fixed(16));

  // document2 |= border;
  Render(empty_screen, document2);
  empty_screen.Print();
}

void test1() {
  std::string first_name;
  std::string last_name;
  std::string password;

  Component input_first_name = Input(&first_name, "First name");
  Component input_last_name = Input(&last_name, "Last name");
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "Password", password_option);

  std::vector<std::string> entries = {"Hello ", "Fuck you ", "Hi ", "Goodbye "};

  int selected_1 = 0;
  int dropdown_select = 0;
  int selected_3 = 0;
  int selected_4 = 0;

  Component dropdown = Dropdown(&entries, &dropdown_select);

  auto component = Container::Vertical({
      dropdown,
      input_first_name,
      input_last_name,
      input_password,
  });

  auto renderer = Renderer(component, [&] {
    return vbox({
               hbox({
                   text(entries[dropdown_select] + first_name + " " +
                        last_name),
                   dropdown->Render(),
               }),
               separator(),
               hbox(text(" First name : "), input_first_name->Render()),
               hbox(text(" Last name  : "), input_last_name->Render()),
               hbox(text(" Password   : "), input_password->Render()),
           }) |
           border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}

void test2() {
  std::vector<std::string> entries = {
      "tribute",     "clearance", "ally",        "bend",        "electronics",
      "module",      "era",       "cultural",    "sniff",       "nationalism",
      "negotiation", "deliver",   "figure",      "east",        "tribute",
      "clearance",   "ally",      "bend",        "electronics", "module",
      "era",         "cultural",  "sniff",       "nationalism", "negotiation",
      "deliver",     "figure",    "east",        "tribute",     "clearance",
      "ally",        "bend",      "electronics", "module",      "era",
      "cultural",    "sniff",     "nationalism", "negotiation", "deliver",
      "figure",      "east",
  };

  int selected_1 = 0;
  int selected_2 = 0;
  int selected_3 = 0;
  int selected_4 = 0;

  auto layout = Container::Vertical({
      Container::Horizontal({
          Dropdown(&entries, &selected_1),
          Dropdown(&entries, &selected_2),
      }),
      Container::Horizontal({
          Dropdown(&entries, &selected_3),
          Dropdown(&entries, &selected_4),
      }),
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(layout);
}

void calculator() {
  std::string input;
  std::string output;
  Component input_component = Input(&input, "Expression");

  auto component = Container::Vertical({
      input_component,
  });

  auto renderer = Renderer(component, [&] {
    return window(text("calculator"),
                  vbox({
                      // hbox(text("Answer : ") )
                      hbox(text("Answer : "),
                           text(std::format("{:.6f}", evaluate(input)))),
                      hbox(text("Expression : "), input_component->Render()),
                  }));
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}

auto calculate_test() -> int {
  std::vector<int> v = {1, 2, 3, 4, 5};

  for (auto [i, c] : ranges::zip_view(views::iota(0, static_cast<int>(v.size())), v | views::all)) {
    std::cout << std::format("{} and {}\n", i, c);
  }

  std::cout << std::format("{:.6f}\n", evaluate("1 + 2 + 3"));
  std::cout << std::format("{:.6f}\n", evaluate("1 +"));

  return 0;
}

auto calculate_int_test() -> int {
  evaluate_int("1 + 2 + 3");
  // evaluate_int("(1 + (2 * 3)) / 2.5");
  return 0;
}

int main() {
  // tests();
  // test1();
  // test2();
  // calculator();
  // calculate_test();
  calculate_int_test();

  return EXIT_SUCCESS;
}
