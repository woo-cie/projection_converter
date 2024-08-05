#include <GeographicLib/MGRS.hpp>
#include <iomanip>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <string>
#include <yaml-cpp/yaml.h>

#include <map_projector/map_projector.hpp>
#include <projection_converter/progress_bar.hpp>

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
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ>(argv[3], *cloud) == -1) {
    std::cerr << "Couldn't read file " << argv[3] << "\n";
    return -1;
  }

  // Define converters
  auto in_map_projector = MapProjector::getMapProjector(input_config);
  auto out_map_projector = MapProjector::getMapProjector(output_config);
  ProgressBar pg;

  // Convert points
  size_t n_points = cloud->points.size();
  pg.start(n_points);

#pragma omp parallel for
  for (size_t i = 0; i < n_points; ++i) {
    auto &point = cloud->points[i];
    auto ll = in_map_projector->convertToLatLon(Coord{point.x, point.y});
    auto coord = out_map_projector->convertToCoord(ll);
    point.x = coord.x;
    point.y = coord.y;

    // Update and draw the progress bar
    // drawProgressBar(70, static_cast<double>(i + 1) / n_points);
    pg.update(1);
  }
  std::cout << std::endl;

  // Save converted point cloud to file
  pcl::io::savePCDFileBinary(argv[4], *cloud);

  std::cout << "Point cloud projection conversion completed successfully.\n";

  return 0;
}
