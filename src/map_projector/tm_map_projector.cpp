#include <GeographicLib/TransverseMercatorExact.hpp>
#include <iostream>
#include <map_projector/tm_map_projector.hpp>

TmMapProjector::TmMapProjector(double origin_lat, double origin_lon) {
  central_meridian_ = origin_lon;

  const GeographicLib::TransverseMercatorExact &proj =
      GeographicLib::TransverseMercatorExact::UTM();
  GeographicLib::Math::real x;
  proj.Forward(central_meridian_, origin_lat, origin_lon, x, base_y_);
}

void TmMapProjector::convertToLatLon(Coord coord, LatLon &ll) {
  const GeographicLib::TransverseMercatorExact &proj =
      GeographicLib::TransverseMercatorExact::UTM();
  proj.Reverse(central_meridian_, coord.x, coord.y + base_y_, ll.lat, ll.lon);
};

void TmMapProjector::convertToCoord(LatLon ll, Coord &coord) {
  const GeographicLib::TransverseMercatorExact &proj =
      GeographicLib::TransverseMercatorExact::UTM();
  proj.Forward(central_meridian_, ll.lat, ll.lon, coord.x, coord.y);
  coord.y -= base_y_;
}
