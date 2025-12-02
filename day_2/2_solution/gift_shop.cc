#include <fstream>
#include <iostream>
#include <sstream>

void readFromFile(std::string input);
bool isInvalid(long long num);

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
  std::getline(file, line);  // está en la misma línea

  std::stringstream ss(line);
  std::string rango;
  long long suma = 0;

  while (std::getline(ss, rango, ',')) {
    long long inicio, fin;
    char dash;

    std::stringstream range_ss(rango);
    range_ss >> inicio >> dash >> fin;

    for (long long num = inicio; num <= fin; num++) {
      if (isInvalid(num)) {
        suma += num;
      }
    }
  }

  std::cout << suma << std::endl;
}

bool isInvalid(long long num) {
  std::string str = std::to_string(num);

  for (int patron_len = 1; patron_len <= str.length() / 2; patron_len++) {
    if (str.length() % patron_len == 0) {
      int repeticiones = str.length() / patron_len;

      if (repeticiones >= 2) {
        std::string patron = str.substr(0, patron_len);

        std::string repetido = "";
        for (int i = 0; i < repeticiones; i++) {
          repetido += patron;
        }

        if (repetido == str) {
          return true;
        }
      }
    }
  }

  return false;
}