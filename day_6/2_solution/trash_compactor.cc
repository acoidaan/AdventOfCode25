#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Problem {
  std::vector<long long> numbers;
  char operation;
};

std::vector<std::string> ReadLines(const std::string& filename) {
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(input_file, line)) {
    lines.push_back(line);
  }
  input_file.close();
  return lines;
}

size_t GetMaxWidth(const std::vector<std::string>& lines) {
  size_t max_width = 0;
  for (const auto& line : lines) {
    max_width = std::max(max_width, line.size());
  }
  return max_width;
}

bool IsColumnNonEmpty(const std::vector<std::string>& lines, size_t col) {
  for (const auto& line : lines) {
    if (col < line.size() && line[col] != ' ') {
      return true;
    }
  }
  return false;
}

std::vector<std::pair<size_t, size_t>> FindProblemColumns(
    const std::vector<std::string>& lines) {
  size_t max_width = GetMaxWidth(lines);
  std::vector<std::pair<size_t, size_t>> problems;

  size_t start = 0;
  bool in_problem = false;

  for (size_t col = 0; col <= max_width; ++col) {
    bool has_content = (col < max_width) && IsColumnNonEmpty(lines, col);

    if (has_content && !in_problem) {
      start = col;
      in_problem = true;
    } else if (!has_content && in_problem) {
      problems.push_back({start, col - 1});
      in_problem = false;
    }
  }

  return problems;
}

// Extrae un problema leyendo de DERECHA a IZQUIERDA
Problem ExtractProblemRightToLeft(const std::vector<std::string>& lines,
                                  size_t col_start, size_t col_end) {
  Problem problem;

  // Extraer operador de la última línea
  const std::string& last_line = lines.back();
  for (size_t col = col_start; col <= col_end; ++col) {
    if (col < last_line.size() && last_line[col] != ' ') {
      problem.operation = last_line[col];
      break;
    }
  }

  // Leer columnas de DERECHA a IZQUIERDA
  for (int col = col_end; col >= static_cast<int>(col_start); --col) {
    std::string num_str = "";

    // Leer dígitos de arriba a abajo (sin incluir última fila del operador)
    for (size_t row = 0; row < lines.size() - 1; ++row) {
      if (static_cast<size_t>(col) < lines[row].size() &&
          lines[row][col] != ' ') {
        num_str += lines[row][col];
      }
    }

    if (!num_str.empty()) {
      problem.numbers.push_back(std::stoll(num_str));
    }
  }

  return problem;
}

long long SolveProblem(const Problem& problem) {
  if (problem.numbers.empty()) return 0;

  long long result = problem.numbers[0];

  if (problem.operation == '+') {
    for (size_t i = 1; i < problem.numbers.size(); ++i) {
      result += problem.numbers[i];
    }
  } else if (problem.operation == '*') {
    for (size_t i = 1; i < problem.numbers.size(); ++i) {
      result *= problem.numbers[i];
    }
  }

  return result;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }

  std::vector<std::string> lines = ReadLines(argv[1]);

  if (lines.empty()) {
    std::cerr << "Error: Archivo vacío" << std::endl;
    return 1;
  }

  std::vector<std::pair<size_t, size_t>> problem_cols =
      FindProblemColumns(lines);

  long long grand_total = 0;
  for (const auto& [col_start, col_end] : problem_cols) {
    Problem problem = ExtractProblemRightToLeft(lines, col_start, col_end);
    long long result = SolveProblem(problem);
    grand_total += result;
  }

  std::cout << grand_total << std::endl;

  return 0;
}