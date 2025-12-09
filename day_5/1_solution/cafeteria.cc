#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Range {
  long long start;
  long long end;
};

struct InputData {
  std::vector<Range> ranges;
  std::vector<long long> available_ids;
};

InputData ReadFromFile(const std::string& filename) {
  InputData data;
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }
  std::string line;
  while (std::getline(input_file, line) && !line.empty()) {
    long long start, end;
    char dash;
    std::istringstream iss(line);
    iss >> start >> dash >> end;
    data.ranges.push_back({start, end});
  }
  long long id;
  while (input_file >> id) {
    data.available_ids.push_back(id);
  }
  input_file.close();
  return data;
}

std::vector<Range> MergeRanges(std::vector<Range> ranges) {
  if (ranges.empty()) return ranges;
  std::sort(ranges.begin(), ranges.end(),
            [](const Range& a, const Range& b) { return a.start < b.start; });
  std::vector<Range> merged;
  merged.push_back(ranges[0]);
  for (size_t i = 1; i < ranges.size(); ++i) {
    Range& last = merged.back();
    const Range& current = ranges[i];
    if (current.start <= last.end + 1) {
      last.end = std::max(last.end, current.end);
    } else {
      merged.push_back(current);
    }
  }
  return merged;
}

bool IsInRanges(long long id, const std::vector<Range>& ranges) {
  for (const auto& range : ranges) {
    if (id >= range.start && id <= range.end) {
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_input>" << std::endl;
    return 1;
  }
  InputData data = ReadFromFile(argv[1]);
  std::vector<Range> merged_ranges = MergeRanges(data.ranges);
  int fresh_count = 0;
  for (long long id : data.available_ids) {
    if (IsInRanges(id, merged_ranges)) {
      fresh_count++;
    }
  }
  std::cout << fresh_count << std::endl;
  return 0;
}