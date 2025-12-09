#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Point3D {
  int x, y, z;
};

struct Edge {
  int box1, box2;
  double distance;

  bool operator<(const Edge& other) const { return distance < other.distance; }
};

class UnionFind {
 public:
  UnionFind(int n) : parent_(n), size_(n, 1) {
    for (int i = 0; i < n; ++i) {
      parent_[i] = i;
    }
  }

  int Find(int x) {
    if (parent_[x] != x) {
      parent_[x] = Find(parent_[x]);
    }
    return parent_[x];
  }

  bool Union(int x, int y) {
    int root_x = Find(x);
    int root_y = Find(y);

    if (root_x == root_y) {
      return false;
    }

    if (size_[root_x] < size_[root_y]) {
      parent_[root_x] = root_y;
      size_[root_y] += size_[root_x];
    } else {
      parent_[root_y] = root_x;
      size_[root_x] += size_[root_y];
    }

    return true;
  }

  std::vector<int> GetAllSizes() {
    std::map<int, int> component_sizes;
    for (size_t i = 0; i < parent_.size(); ++i) {
      int root = Find(i);
      component_sizes[root] = size_[root];
    }

    std::vector<int> sizes;
    for (const auto& [root, size] : component_sizes) {
      sizes.push_back(size);
    }
    return sizes;
  }

 private:
  std::vector<int> parent_;
  std::vector<int> size_;
};

std::vector<Point3D> ReadPoints(const std::string& filename) {
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }

  std::vector<Point3D> points;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    Point3D p;
    char comma;
    iss >> p.x >> comma >> p.y >> comma >> p.z;
    points.push_back(p);
  }
  input_file.close();
  return points;
}

double Distance(const Point3D& p1, const Point3D& p2) {
  long long dx = p1.x - p2.x;
  long long dy = p1.y - p2.y;
  long long dz = p1.z - p2.z;
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

std::vector<Edge> ComputeAllEdges(const std::vector<Point3D>& points) {
  std::vector<Edge> edges;
  int n = points.size();

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      edges.push_back({i, j, Distance(points[i], points[j])});
    }
  }

  return edges;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }

  std::vector<Point3D> points = ReadPoints(argv[1]);
  int n = points.size();

  std::vector<Edge> edges = ComputeAllEdges(points);
  std::sort(edges.begin(), edges.end());

  UnionFind uf(n);

  // Procesar las 1000 aristas más cortas
  for (int i = 0; i < 1000 && i < static_cast<int>(edges.size()); ++i) {
    uf.Union(edges[i].box1, edges[i].box2);
  }

  std::vector<int> sizes = uf.GetAllSizes();
  std::sort(sizes.begin(), sizes.end(), std::greater<int>());

  long long result = 1;
  for (int i = 0; i < 3 && i < static_cast<int>(sizes.size()); ++i) {
    result *= sizes[i];
  }

  std::cout << result << std::endl;

  return 0;
}