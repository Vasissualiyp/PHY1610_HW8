#include <cstdlib>
#include <iostream>

int main() {
  for (int i = 1; i <= 16; i++) {
    std::string command = "./runwithcores " + std::to_string(i);
    std::cout << "Number of cores is: " << std::to_string(i)<< std::endl;
    system(command.c_str());
  }

  return 0;
}

