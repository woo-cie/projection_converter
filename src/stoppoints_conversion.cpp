#include <GeographicLib/MGRS.hpp>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map_projector/map_projector.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

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
  auto in_map_projector = MapProjector::getMapProjector(input_config);
  auto out_map_projector = MapProjector::getMapProjector(output_config);

  // Convert points
  while (std::getline(ifs_stoppoints, str_buf)) {
    boost::algorithm::split(str_vec_buf, str_buf, boost::is_any_of(","));
    auto ll = in_map_projector->convertToLatLon(Coord{
        std::stod(str_vec_buf.at(x_idx)), std::stod(str_vec_buf.at(y_idx))});
    auto coord = out_map_projector->convertToCoord(ll);
    str_vec_buf.at(x_idx) = std::to_string(coord.x);
    str_vec_buf.at(y_idx) = std::to_string(coord.y);
    ofs_stoppoints << boost::algorithm::join(str_vec_buf, ",") << std::endl;
  }

  std::cout << std::endl;
  // Save converted point cloud to file

  std::cout << "Stopponts projection conversion completed successfully.\n";

  return 0;
}
