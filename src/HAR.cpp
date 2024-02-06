#include <exception>
#include <iostream>

#include "app.hpp"

int main() {
  try {
    Application app;
    app.run();
  } catch (std::exception &e) {
    std::cout << e.what() << '\n';
    return 1;
  }
  return 0;
}
