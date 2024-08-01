#ifndef PROJECTION_CONVERTER__CONVERTER_FROM_LLH_HPP
#define PROJECTION_CONVERTER__CONVERTER_FROM_LLH_HPP

#include <GeographicLib/MGRS.hpp>
#include <GeographicLib/TransverseMercatorExact.hpp>
#include <iostream>
#include <pcl/point_types.h>
#include <projection_converter/lat_lon_alt.hpp>
#include <yaml-cpp/yaml.h>

#include <utility>

class ConverterFromLLH {
public:
  ConverterFromLLH(const YAML::Node &config);
  pcl::PointXYZ convert(const LatLonAlt &xyz);

private:
  std::string projector_type_;
  std::pair<double, double> origin_xy_;
  double central_meridian_;
};

#endif // PROJECTION_CONVERTER__CONVERTER_FROM_LLH_HPP
