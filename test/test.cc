#include <iostream>

void foo() { std::cout << "foo function" << std::endl; }

void baz() { std::cout << "baz function" << std::endl; }

int main() {
  std::cout << "main function begin" << std::endl;
  foo();
  baz();
  std::cout << "main function end" << std::endl;
}