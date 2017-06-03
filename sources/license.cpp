#include "license.hpp"
#include <fstream>
#include <iostream>

static std::string filename = "license.txt";

void print_license() {
  std::ifstream ifs(filename);
  if (ifs.is_open()) {
    std::cout << ifs.rdbuf();
  }
}
