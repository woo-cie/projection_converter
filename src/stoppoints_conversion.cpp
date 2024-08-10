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

// Function to draw a progress bar
void drawProgressBar(int len, double percent) {
  std::cout << "Progress: ";
  for (int i = 0; i < len; ++i) {
    if (i < static_cast<int>(len * percent)) {
      std::cout << '=';
    } else {
      std::cout << ' ';
    }
  }
  std::cout << " [" << static_cast<int>(100 * percent) << "%]\r";
  std::cout.flush();
}

struct

    int
    main(int argc, char **argv) {
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

  std::string raw_header = str_buf;

  std::vector<std::vector<std::string>> data;
  while (std::getline(ifs_stoppoints, str_buf)) {
    std::vector<std::string> vec_buf;
    boost::algorithm::split(vec_buf, str_buf, boost::is_any_of(","));
    data.push_back(vec_buf);
  }

  // Define converters
  auto in_map_projector = MapProjector::getMapProjector(input_config);
  auto out_map_projector = MapProjector::getMapProjector(output_config);

  auto size = data.size();

  // Convert points
  for (auto i = 0; i < size; i++) {
    auto &vec = data.at(i);
    auto ll = in_map_projector->convertToLatLon(
        Coord{std::stod(vec.at(x_idx)), std::stod(vec.at(y_idx))});
    auto coord = out_map_projector->convertToCoord(ll);
    vec.at(x_idx) = std::to_string(coord.x);
    vec.at(y_idx) = std::to_string(coord.y);
    // Update and draw the progress bar
    drawProgressBar(70, static_cast<double>(i + 1) / size);
  }

  std::cout << std::endl;
  // Save converted stoppoints to file
  ofs_stoppoints << raw_header << std::endl;
  for (auto vec : data) {
    ofs_stoppoints << boost::algorithm::join(vec, ",") << std::endl;
  }

  std::cout << "Stopponts projection conversion completed successfully.\n";

  return 0;
}
