#include <fstream>
#include <iostream>

#define INIT_POS 50

int calculateNewPos(int actual_pos, char direction, int distance) {
  int new_pos = actual_pos;
  if (direction == 'L') {
    new_pos -= distance;
  } else {
    new_pos += distance;
  }
  new_pos = ((new_pos) % 100 + 100) % 100;
  return new_pos;
}

void readFromFile(std::string input) {
  std::ifstream file(input);

  int pos = INIT_POS;
  int count = 0;
  std::string line;

  while (std::getline(file, line)) {
    pos = calculateNewPos(pos, line[0], std::stoi(line.substr(1)));
    if (pos == 0) count++;
  }
  std::cout << count << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    readFromFile(argv[1]);
  } else {
    std::cerr << argv[0] << " <input_file>\n";
  }
  return 0;
}