#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

struct Beam {
  int row;
  int col;
};

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
Beam FindStart(const std::vector<std::string>& grid) {
  for (size_t row = 0; row < grid.size(); ++row) {
    for (size_t col = 0; col < grid[row].size(); ++col) {
      if (grid[row][col] == 'S') {
        return {static_cast<int>(row), static_cast<int>(col)};
      }
    }
  }
  return {-1, -1};
}

int SimulateBeams(const std::vector<std::string>& grid) {
  Beam start = FindStart(grid);

  std::queue<Beam> beams;
  beams.push(start);

  std::set<std::pair<int, int>> activated_splitters;

  int rows = grid.size();
  int cols = grid[0].size();

  while (!beams.empty()) {
    Beam beam = beams.front();
    beams.pop();

    // Mover el rayo hacia abajo hasta encontrar un splitter o salir del grid
    for (int row = beam.row + 1; row < rows; ++row) {
      char cell = grid[row][beam.col];

      if (cell == '^') {
        // Encontró un splitter
        std::pair<int, int> splitter_pos = {row, beam.col};

        // Solo activar si no ha sido activado antes
        if (activated_splitters.find(splitter_pos) ==
            activated_splitters.end()) {
          activated_splitters.insert(splitter_pos);

          // Crear dos nuevos rayos: izquierda y derecha
          int left_col = beam.col - 1;
          int right_col = beam.col + 1;

          if (left_col >= 0) {
            beams.push({row, left_col});
          }
          if (right_col < cols) {
            beams.push({row, right_col});
          }
        }

        // El rayo actual se detiene
        break;
      }
      // Si es '.', continuar hacia abajo
    }
  }

  return activated_splitters.size();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }

  std::vector<std::string> grid = ReadGrid(argv[1]);
  int splits = SimulateBeams(grid);

  std::cout << splits << std::endl;

  return 0;
}