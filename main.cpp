#include <Eigen/Dense>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>

using namespace ftxui;
using Eigen::MatrixXd;

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
  int selected_2 = 0;
  int selected_3 = 0;
  int selected_4 = 0;

  auto layout = Container::Horizontal({
    Dropdown(&entries, &selected_1),
  });

  auto component = Container::Vertical({
      layout,
      input_first_name,
      input_last_name,
      input_password,
  });

  auto renderer = Renderer(component, [&] {
    return vbox({
               hbox({
                   text("Hello " + first_name + " " + last_name),
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

int main() {
  // tests();
  test1();

  return EXIT_SUCCESS;
}
