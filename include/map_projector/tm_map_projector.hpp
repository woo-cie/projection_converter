#include "./map_projector.hpp"

class TmMapProjector : MapProjector {
private:
  GeographicLib::Math::real central_meridian_;
  GeographicLib::Math::real base_y_;

public:
  TmMapProjector(GeographicLib::Math::real origin_lat,
                 GeographicLib::Math::real origin_lon);
  virtual void convertToLatLon(Coord coord, LatLon &ll);
  virtual void convertToCoord(LatLon ll, Coord &coord);
};
