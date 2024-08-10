#include <GeographicLib/MGRS.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <pcl/PCLPointCloud2.h>
#include <pcl/io/auto_io.h>
#include <pcl/point_types.h>
#include <projection_converter/converter_from_llh.hpp>
#include <projection_converter/converter_to_llh.hpp>
#include <projection_converter/lat_lon_alt.hpp>
#include <projection_converter/progress_bar.hpp>
#include <string>
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

  // Load point cloud data from file
  pcl::PCLPointCloud2::Ptr cloud(new pcl::PCLPointCloud2);
  if (pcl::io::load(argv[3], *cloud) == -1) {
    std::cerr << "Couldn't read file " << argv[3] << "\n";
    return -1;
  }

  // Define converters
  ConverterToLLH to_llh(input_config);
  ConverterFromLLH from_llh(output_config);
  ProgressBar pg;

  // Convert points
  pcl::uindex_t n_points = cloud->width * cloud->height;
  pg.start(n_points);

  auto x_idx = std::find_if(cloud->fields.begin(), cloud->fields.end(),
                            [](auto v) { return v.name == "x"; });
  if (x_idx == cloud->fields.end()) {
    std::cerr << "Input pcd file has no 'x' field." << "\n";
    return -1;
  }

  auto y_idx = std::find_if(cloud->fields.begin(), cloud->fields.end(),
                            [](pcl::PCLPointField v) { return v.name == "y"; });
  if (y_idx == cloud->fields.end()) {
    std::cerr << "Input pcd file has no 'y' field." << "\n";
    return -1;
  }

#pragma omp parallel for
  for (pcl::uindex_t i = 0; i < n_points; ++i) {
    auto prev_x = cloud->at<float>(i, x_idx->offset);
    auto prev_y = cloud->at<float>(i, y_idx->offset);

    LatLonAlt llh = to_llh.convert(Coord{prev_x, prev_y, 0});
    auto coord = from_llh.convert(llh);

    cloud->at<float>(i, x_idx->offset) = coord.x;
    cloud->at<float>(i, y_idx->offset) = coord.y;
    pg.update(1);
  }
  std::cout << std::endl;

  // Save converted point cloud to file
  pcl::io::save(argv[4], *cloud);

  std::cout << "Point cloud projection conversion completed successfully.\n";

  return 0;
}
