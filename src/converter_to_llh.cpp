#include <iomanip>
#include <iostream>
#include <projection_converter/converter_to_llh.hpp>

ConverterToLLH::ConverterToLLH(const YAML::Node &config) {
  projector_type_ = config["projector_type"].as<std::string>();
  if (projector_type_ == "MGRS") {
    mgrs_grid_ = config["mgrs_grid"].as<std::string>();
  }
}

LatLonAlt ConverterToLLH::convert(const Coord &xyz) {
  LatLonAlt llh;
  if (projector_type_ == "MGRS") {
    try {
      int zone;
      bool northp;
      double mgrs_base_x, mgrs_base_y;
      int prec = 8;
      bool longpath = false;
      GeographicLib::MGRS::Reverse(mgrs_grid_, zone, northp, mgrs_base_x,
                                   mgrs_base_y, prec, longpath);

      // Convert UTM to LLH
      GeographicLib::UTMUPS::Reverse(zone, northp, xyz.x + mgrs_base_x,
                                     xyz.y + mgrs_base_y, llh.lat, llh.lon);

      llh.alt = xyz.z;
    } catch (const std::exception &e) {
      std::cerr << "Error: Could not convert from MGRS to UTM: " << e.what()
                << "\n";
      return LatLonAlt();
    }
  }
  return llh;
}
