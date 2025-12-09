#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> ReadGrid(const std::string& filename) {
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }

  std::vector<std::string> grid;
  std::string line;
  while (std::getline(input_file, line)) {
    grid.push_back(line);
  }
  input_file.close();
  return grid;
}

// Encuentra la posición de 'S'
std::pair<int, int> FindStart(const std::vector<std::string>& grid) {
  for (size_t row = 0; row < grid.size(); ++row) {
    for (size_t col = 0; col < grid[row].size(); ++col) {
      if (grid[row][col] == 'S') {
        return {static_cast<int>(row), static_cast<int>(col)};
      }
    }
  }
  return {-1, -1};
}

// Cuenta el número de timelines desde una posición dada
long long CountTimelines(const std::vector<std::string>& grid, int row, int col,
                         std::map<std::pair<int, int>, long long>& memo) {
  int rows = grid.size();
  int cols = grid[0].size();

  // Validar límites horizontales
  if (col < 0 || col >= cols) {
    return 0;  // Camino inválido
  }

  // Verificar si ya calculamos desde esta posición
  std::pair<int, int> state = {row, col};
  if (memo.find(state) != memo.end()) {
    return memo[state];
  }

  // Mover hacia abajo hasta encontrar un splitter o salir del grid
  for (int r = row + 1; r < rows; ++r) {
    if (grid[r][col] == '^') {
      // Encontró un splitter - la partícula toma AMBOS caminos
      long long left_timelines = CountTimelines(grid, r, col - 1, memo);
      long long right_timelines = CountTimelines(grid, r, col + 1, memo);

      long long result = left_timelines + right_timelines;
      memo[state] = result;
      return result;
    }
  }

  // Salió del grid sin encontrar más splitters - 1 timeline completa
  memo[state] = 1;
  return 1;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }

  std::vector<std::string> grid = ReadGrid(argv[1]);
  auto [start_row, start_col] = FindStart(grid);

  std::map<std::pair<int, int>, long long> memo;
  long long timelines = CountTimelines(grid, start_row, start_col, memo);

  std::cout << timelines << std::endl;

  return 0;
}