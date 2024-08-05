#include <map_projector/map_projector.hpp>
#include <map_projector/mgrs_map_projector.hpp>
#include <map_projector/tm_map_projector.hpp>

MapProjector *MapProjector::getMapProjector(const YAML::Node &config) {
  auto projector_type_ = config["projector_type"].as<std::string>();

  if (projector_type_ == "MGRS") {
    auto mgrs_grid = config["mgrs_grid"].as<std::string>();
    return new MgrsMapProjector{mgrs_grid};
  } else if (projector_type_ == "TransverseMercator") {
    auto origin_lon = config["map_origin"]["longitude"].as<double>();
    auto origin_lat = config["map_origin"]["latitude"].as<double>();
    return new TmMapProjector{origin_lat, origin_lon};
  } else {
    // TODO: error handling
    return nullptr;
  }
}
