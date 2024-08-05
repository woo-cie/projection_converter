#ifndef MAP_PROJECTOR__MAP_PROJECTOR_HPP
#define MAP_PROJECTOR__MAP_PROJECTOR_HPP

#include <GeographicLib/Math.hpp>
#include <GeographicLib/TransverseMercatorExact.hpp>
#include <string>
#include <yaml-cpp/yaml.h>

struct LatLon {
  GeographicLib::Math::real lat;
  GeographicLib::Math::real lon;
};

struct Coord {
  GeographicLib::Math::real x;
  GeographicLib::Math::real y;
};

class MapProjector {
public:
  virtual LatLon convertToLatLon(Coord coord) { return LatLon{}; };
  virtual Coord convertToCoord(LatLon ll) { return Coord{}; };
  static MapProjector *getMapProjector(const YAML::Node &config);
};

#endif // MAP_PROJECTOR__MAP_PROJECTOR_HPP
