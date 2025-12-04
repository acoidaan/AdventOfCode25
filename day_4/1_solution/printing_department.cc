#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
  std::vector<std::string> grid;

  while (std::getline(file, line)) {
    grid.push_back(line);
  }

  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  int count = 0;

  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[i].length(); j++) {
      if (grid[i][j] == '@') {
        int vecinos = 0;

        for (int k = 0; k < 8; k++) {
          int ni = i + dx[k];
          int nj = j + dy[k];

          if (ni >= 0 && ni < grid.size() && nj >= 0 &&
              nj < grid[ni].length() && grid[ni][nj] == '@') {
            vecinos++;
          }
        }

        if (vecinos < 4) {
          count++;
        }
      }
    }
  }

  std::cout << count << std::endl;
}