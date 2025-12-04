#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

void readFromFile(std::string input);
int countNeighbors(const std::vector<std::string>& grid, int i, int j);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << argv[0] << " <input_file>\n";
  } else {
    readFromFile(argv[1]);
  }
  return 0;
}

int countNeighbors(const std::vector<std::string>& grid, int i, int j) {
  // Las 8 direcciones (incluidas diagonales)
  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  int vecinos = 0;

  for (int k = 0; k < 8; k++) {
    int ni = i + dx[k];
    int nj = j + dy[k];

    // Verificar límites
    if (ni >= 0 && ni < grid.size() && nj >= 0 && nj < grid[ni].length() &&
        grid[ni][nj] == '@') {
      vecinos++;
    }
  }

  return vecinos;
}

void readFromFile(std::string input) {
  std::ifstream file(input);
  std::string line;
  std::vector<std::string> grid;

  // Leer toda la cuadrícula
  while (std::getline(file, line)) {
    grid.push_back(line);
  }

  int total_removed = 0;
  bool changed = true;

  // Repetir hasta que no haya más cambios
  while (changed) {
    changed = false;
    std::vector<std::pair<int, int>> to_remove;

    // Encontrar todos los rollos accesibles en esta iteración
    for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[i].length(); j++) {
        if (grid[i][j] == '@') {
          int vecinos = countNeighbors(grid, i, j);
          if (vecinos < 4) {
            to_remove.push_back({i, j});
          }
        }
      }
    }

    // Remover todos los rollos accesibles
    if (!to_remove.empty()) {
      changed = true;
      for (auto pos : to_remove) {
        grid[pos.first][pos.second] = '.';
        total_removed++;
      }
    }
  }

  std::cout << total_removed << std::endl;
}