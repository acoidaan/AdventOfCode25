#include <fstream>
#include <iostream>
#include <string>

void readFromFile(std::string input);
std::string getMaxJoltage(std::string bank, int k);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " <input_file>\n";
  } else {
    readFromFile(argv[1]);
  }
  return 0;
}

std::string getMaxJoltage(std::string bank, int k) {
  std::string result = "";
  int n = bank.length();
  int start = 0;

  for (int i = 0; i < k; i++) {
    int end = n - (k - i);

    char max_digit = '0';
    int max_pos = start;

    for (int j = start; j <= end; j++) {
      if (bank[j] > max_digit) {
        max_digit = bank[j];
        max_pos = j;
      }
    }

    result += max_digit;
    start = max_pos + 1;
  }

  return result;
}

void readFromFile(std::string input) {
  std::ifstream file(input);
  std::string line;
  unsigned long long suma_total = 0;

  while (std::getline(file, line)) {
    std::string max_joltage_str = getMaxJoltage(line, 12);

    unsigned long long max_joltage = std::stoull(max_joltage_str);

    suma_total += max_joltage;
  }
  std::cout << suma_total << std::endl;
}