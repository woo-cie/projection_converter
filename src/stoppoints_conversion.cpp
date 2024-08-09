#include <GeographicLib/MGRS.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <projection_converter/lat_lon_alt.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include <projection_converter/converter_from_llh.hpp>
#include <projection_converter/converter_to_llh.hpp>

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cout << "Usage: ./ProjectionConverter input_yaml output_yaml "
                 "input_pcd output_pcd\n";
    return -1;
  }

  // Parse YAML configuration files
  YAML::Node input_config = YAML::LoadFile(argv[1]);
  YAML::Node output_config = YAML::LoadFile(argv[2]);

  // Load Stoppoints file
  std::ifstream ifs_stoppoints(argv[3]);
  std::ofstream ofs_stoppoints(argv[4]);

  std::string str_buf;
  std::vector<std::string> str_vec_buf;

  std::getline(ifs_stoppoints, str_buf);
  boost::algorithm::split(str_vec_buf, str_buf, boost::is_any_of(","));
  const int x_idx =
      std::distance(str_vec_buf.begin(),
                    std::find(str_vec_buf.begin(), str_vec_buf.end(), "x"));
  const int y_idx =
      std::distance(str_vec_buf.begin(),
                    std::find(str_vec_buf.begin(), str_vec_buf.end(), "y"));
  ofs_stoppoints << str_buf << std::endl;

  // Define converters
  ConverterToLLH to_llh(input_config);
  ConverterFromLLH from_llh(output_config);

  // Convert points
  while (std::getline(ifs_stoppoints, str_buf)) {
    boost::algorithm::split(str_vec_buf, str_buf, boost::is_any_of(","));
    auto x = std::stod(str_vec_buf.at(x_idx));
    auto y = std::stod(str_vec_buf.at(x_idx));
    LatLonAlt llh = to_llh.convert(Coord{std::stod(str_vec_buf.at(x_idx)),
                                         std::stod(str_vec_buf.at(y_idx)), 0});
    auto coord = from_llh.convert(llh);
    str_vec_buf.at(x_idx) = std::to_string(coord.x);
    str_vec_buf.at(y_idx) = std::to_string(coord.y);
    ofs_stoppoints << boost::algorithm::join(str_vec_buf, ",") << std::endl;
  }

  std::cout << std::endl;
  // Save converted point cloud to file

  std::cout << "Stopponts projection conversion completed successfully.\n";

  return 0;
}
