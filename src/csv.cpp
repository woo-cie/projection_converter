#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <csv.hpp>
#include <fstream>
#include <sstream>

Csv Csv::load_file(char *file_path) {
  Csv csv;
  std::string str_buf;
  std::vector<std::string> str_vec_buf;

  std::ifstream ifs(file_path);

  std::getline(ifs, csv.raw_header);
  boost::algorithm::split(str_vec_buf, csv.raw_header, boost::is_any_of(","));
  for (auto i = 0; i < str_vec_buf.size(); i++) {
    auto v = str_vec_buf.at(i);
    csv.cloumn_indices.insert_or_assign(v, i);
  }

  while (std::getline(ifs, str_buf)) {
    std::vector<std::string> vec_buf;
    boost::algorithm::split(vec_buf, str_buf, boost::is_any_of(","));
    csv.data.push_back(vec_buf);
  }

  return csv;
}

void Csv::export_file(char *file_path) {
  std::ofstream ofs(file_path);
  ofs << raw_header << std::endl;
  for (auto vec : data) {
    ofs << boost::algorithm::join(vec, ",") << std::endl;
  }
}
