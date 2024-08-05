#ifndef MAP_PROJECTOR__TM_MAP_PROJECTOR_HPP
#define MAP_PROJECTOR__TM_MAP_PROJECTOR_HPP

#include "./map_projector.hpp"

class TmMapProjector : public MapProjector {
private:
  GeographicLib::Math::real central_meridian_;
  GeographicLib::Math::real base_y_;

public:
  TmMapProjector(GeographicLib::Math::real origin_lat,
                 GeographicLib::Math::real origin_lon);
  virtual LatLon convertToLatLon(Coord coord);
  virtual Coord convertToCoord(LatLon ll);
};

#endif // MAP_PROJECTOR__TM_MAP_PROJECTOR_HPP
