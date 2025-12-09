#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
  int x, y;
};

std::vector<Point> ReadPoints(const std::string& filename) {
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }

  std::vector<Point> points;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    Point p;
    char comma;
    iss >> p.x >> comma >> p.y;
    points.push_back(p);
  }
  input_file.close();
  return points;
}

long long CalculateArea(const Point& p1, const Point& p2) {
  long long width = std::abs(static_cast<long long>(p2.x) - p1.x) + 1;
  long long height = std::abs(static_cast<long long>(p2.y) - p1.y) + 1;
  return width * height;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }

  std::vector<Point> red_tiles = ReadPoints(argv[1]);
  int n = red_tiles.size();

  long long max_area = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      long long area = CalculateArea(red_tiles[i], red_tiles[j]);
      max_area = std::max(max_area, area);
    }
  }

  std::cout << max_area << std::endl;

  return 0;
}