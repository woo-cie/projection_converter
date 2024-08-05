#ifndef MAP_PROJECTOR__MGRS_MAP_PROJECTOR_HPP
#define MAP_PROJECTOR__MGRS_MAP_PROJECTOR_HPP

#include "./map_projector.hpp"

class MgrsMapProjector : public MapProjector {
private:
  int zone_;
  bool northp_;

  GeographicLib::Math::real base_x_;
  GeographicLib::Math::real base_y_;

public:
  MgrsMapProjector(std::string mgrs_grid);
  virtual LatLon convertToLatLon(Coord coord);
  virtual Coord convertToCoord(LatLon ll);
};

#endif //  MAP_PROJECTOR__MGRS_MAP_PROJECTOR_HPP
