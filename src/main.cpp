#include <iostream>
#include <string>

int main() {
  using namespace std;
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::cout << "$ ";

  string command;

  getline(cin, command);

  cout<< command<< ": Command not found"<< endl;
}
