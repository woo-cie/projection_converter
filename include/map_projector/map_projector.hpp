#include <GeographicLib/Math.hpp>
#include <GeographicLib/TransverseMercatorExact.hpp>
#include <string>

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
  virtual void convertToLatLon(Coord coord, LatLon &ll) {};
  virtual void convertToCoord(LatLon ll, Coord &coord) {};
};
