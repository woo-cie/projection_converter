#include <projection_converter/converter_from_llh.hpp>

ConverterFromLLH::ConverterFromLLH(const YAML::Node &config) {
  projector_type_ = config["projector_type"].as<std::string>();

  if (projector_type_ == "TransverseMercator") {
    central_meridian_ = config["map_origin"]["longitude"].as<double>();

    // Calculate origin in Transverse Mercator coordinate
    const GeographicLib::TransverseMercatorExact &proj =
        GeographicLib::TransverseMercatorExact::UTM();
    double x, y;
    proj.Forward(central_meridian_,
                 config["map_origin"]["latitude"].as<double>(),
                 config["map_origin"]["longitude"].as<double>(), x, y);
    origin_xy_ = std::pair<double, double>(x, y);
  }
}

pcl::PointXYZ ConverterFromLLH::convert(const LatLonAlt &llh) {
  pcl::PointXYZ xyz;
  if (projector_type_ == "TransverseMercator") {
    const GeographicLib::TransverseMercatorExact &proj =
        GeographicLib::TransverseMercatorExact::UTM();

    // Variables to hold the results
    double x, y;

    // Convert to transverse mercator coordinates
    proj.Forward(central_meridian_, llh.lat, llh.lon, x, y);
    xyz.x = x - origin_xy_.first;
    xyz.y = y - origin_xy_.second;
    xyz.z = llh.alt;

  } else {
    std::cerr << "Error: Only conversion to "
                 "TransverseMercator is supported currently.\n";
    std::cerr << "Not supported projector type: " << projector_type_
              << std::endl;
    return pcl::PointXYZ();
  }
  return xyz;
}
