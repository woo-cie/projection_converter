#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <projection_converter/converter_from_llh.hpp>
#include <projection_converter/converter_to_llh.hpp>
#include <string>
#include <yaml-cpp/yaml.h>

bool is_osm_file(const std::string &filename) {
  return boost::algorithm::iends_with(filename, ".osm");
}
bool is_pcd_file(const std::string &filename) {
  return boost::algorithm::iends_with(filename, ".pcd");
}
bool is_csv_file(const std::string &filename) {
  return boost::algorithm::iends_with(filename, ".csv");
}

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cout << "Usage: ./ProjectionConverter input_yaml output_yaml "
                 "input_file output_file"
              << std::endl;
    return -1;
  }

  // Parse YAML configuration files
  YAML::Node input_config = YAML::LoadFile(argv[1]);
  YAML::Node output_config = YAML::LoadFile(argv[2]);

  auto input_file = std::string(argv[3]);
  auto output_file = std::string(argv[4]);

  // Define converters
  ConverterToLLH to_llh(input_config);
  ConverterFromLLH from_llh(output_config);

  if (is_osm_file(input_file)) {
    if (is_osm_file(output_file)) {
      std::cerr << "Lanelet2 conversion is not implemented." << std::endl;
      return -1;
    } else {
      std::cerr << "Output_file should have same file extensions as input_file."
                << std::endl;
      return -1;
    }
  } else if (is_pcd_file(input_file)) {
    if (is_pcd_file(output_file)) {
      std::cerr << "Pointcloud conversion is not implemented." << std::endl;
      return -1;
    } else {
      std::cerr << "Output_file should have same file extensions as input_file."
                << std::endl;
      return -1;
    }
  } else if (is_csv_file(input_file)) {
    if (is_csv_file(output_file)) {
      std::cerr << "Stoppoints conversion is not implemented." << std::endl;
      return -1;
    } else {
      std::cerr << "Output_file should have same file extensions as input_file."
                << std::endl;
      return -1;
    }
  } else {
    std::cerr << "Projection conversion only support Lanelet2 (.osm), "
                 "Pointcloud (.pcd) and Stoppoints (.csv)."
              << std::endl;
    return -1;
  }

  std::cout << "Projection conversion completed successfully.\n";
  return 0;
}
