#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
  long long x, y;
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

bool IsPointInOrOnPolygon(const Point& point,
                          const std::vector<Point>& polygon) {
  int n = polygon.size();

  // Verificar si está en algún borde
  for (int i = 0; i < n; ++i) {
    const Point& p1 = polygon[i];
    const Point& p2 = polygon[(i + 1) % n];

    long long min_x = std::min(p1.x, p2.x);
    long long max_x = std::max(p1.x, p2.x);
    long long min_y = std::min(p1.y, p2.y);
    long long max_y = std::max(p1.y, p2.y);

    if (point.x >= min_x && point.x <= max_x && point.y >= min_y &&
        point.y <= max_y) {
      long long cross =
          (p2.x - p1.x) * (point.y - p1.y) - (p2.y - p1.y) * (point.x - p1.x);
      if (cross == 0) {
        return true;
      }
    }
  }

  // Ray casting
  int intersections = 0;
  for (int i = 0; i < n; ++i) {
    const Point& p1 = polygon[i];
    const Point& p2 = polygon[(i + 1) % n];

    if ((p1.y > point.y) != (p2.y > point.y)) {
      double x_intersect =
          p1.x + (double)(point.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
      if (point.x < x_intersect) {
        intersections++;
      }
    }
  }

  return (intersections % 2) == 1;
}

bool IsRectangleValid(const Point& corner1, const Point& corner2,
                      const std::vector<Point>& polygon) {
  long long x_min = std::min(corner1.x, corner2.x);
  long long x_max = std::max(corner1.x, corner2.x);
  long long y_min = std::min(corner1.y, corner2.y);
  long long y_max = std::max(corner1.y, corner2.y);

  // Verificar las 4 esquinas
  if (!IsPointInOrOnPolygon({x_min, y_min}, polygon)) return false;
  if (!IsPointInOrOnPolygon({x_min, y_max}, polygon)) return false;
  if (!IsPointInOrOnPolygon({x_max, y_min}, polygon)) return false;
  if (!IsPointInOrOnPolygon({x_max, y_max}, polygon)) return false;

  // Verificar puntos a lo largo de los bordes
  long long step = 10;
  for (long long x = x_min; x <= x_max; x += step) {
    if (!IsPointInOrOnPolygon({x, y_min}, polygon)) return false;
    if (!IsPointInOrOnPolygon({x, y_max}, polygon)) return false;
  }
  for (long long y = y_min; y <= y_max; y += step) {
    if (!IsPointInOrOnPolygon({x_min, y}, polygon)) return false;
    if (!IsPointInOrOnPolygon({x_max, y}, polygon)) return false;
  }

  // Verificar cuadrícula interna
  long long width = x_max - x_min;
  long long height = y_max - y_min;
  int checks = std::min(50LL, std::max(width, height) / 100);
  for (int i = 1; i < checks; ++i) {
    for (int j = 1; j < checks; ++j) {
      long long x = x_min + width * i / checks;
      long long y = y_min + height * j / checks;
      if (!IsPointInOrOnPolygon({x, y}, polygon)) return false;
    }
  }

  return true;
}

long long CalculateArea(const Point& p1, const Point& p2) {
  long long width = std::abs(p2.x - p1.x) + 1;
  long long height = std::abs(p2.y - p1.y) + 1;
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
      if (IsRectangleValid(red_tiles[i], red_tiles[j], red_tiles)) {
        long long area = CalculateArea(red_tiles[i], red_tiles[j]);
        max_area = std::max(max_area, area);
      }
    }
  }

  std::cout << max_area << std::endl;

  return 0;
}