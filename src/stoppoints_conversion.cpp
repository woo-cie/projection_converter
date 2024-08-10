#include <csv.hpp>
#include <projection_converter/converter_from_llh.hpp>
#include <projection_converter/converter_to_llh.hpp>
#include <projection_converter/lat_lon_alt.hpp>
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
  auto stoppoints = Csv::load_file(argv[3]);

  const int x_idx = stoppoints.cloumn_indices.at("x");
  const int y_idx = stoppoints.cloumn_indices.at("y");

  // Define converters
  ConverterToLLH to_llh(input_config);
  ConverterFromLLH from_llh(output_config);

  auto size = stoppoints.data.size();

  // Convert points
  for (auto i = 0; i < size; i++) {
    auto &vec = stoppoints.data.at(i);
    auto x = std::stod(vec.at(x_idx));
    auto y = std::stod(vec.at(y_idx));
    LatLonAlt llh = to_llh.convert(Coord{x, y, 0});
    auto coord = from_llh.convert(llh);
    vec.at(x_idx) = std::to_string(coord.x);
    vec.at(y_idx) = std::to_string(coord.y);
    // Update and draw the progress bar
    drawProgressBar(70, static_cast<double>(i + 1) / size);
  }

  std::cout << std::endl;
  // Save converted stoppoints to file
  stoppoints.export_file(argv[4]);

  std::cout << "Stopponts projection conversion completed successfully.\n";

  return 0;
}
