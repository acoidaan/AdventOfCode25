#include <fstream>
#include <iostream>

void readFromFile(std::string input);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " <input_file>\n";
  } else {
    readFromFile(argv[1]);
  }
  return 0;
}

void readFromFile(std::string input) {
  std::ifstream file(input);
  std::string line;
  int suma_total = 0;

  while (std::getline(file, line)) {
    int max_joltage = 0;

    for (int i = 0; i < line.length(); ++i) {
      for (int j = i + 1; j < line.length(); ++j) {
        int num = (line[i] - '0') * 10 + (line[j] - '0');

        if (num > max_joltage) {
          max_joltage = num;
        }
      }
    }
    suma_total += max_joltage;
  }
  std::cout << suma_total << std::endl;
}