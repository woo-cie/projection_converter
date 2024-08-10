#include <map>
#include <string>
#include <vector>

class Csv {
public:
  std::string raw_header;
  std::map<std::string, int> cloumn_indices;
  std::vector<std::vector<std::string>> data;

  static Csv load_file(char *file_path);
  void export_file(char *file_path);
};
