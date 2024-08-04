#include "./map_projector.hpp"

class MgrsMapProjector : MapProjector {
private:
  int zone_;
  bool northp_;

  GeographicLib::Math::real base_x_;
  GeographicLib::Math::real base_y_;

public:
  MgrsMapProjector(std::string mgrs_grid);
  virtual void convertToLatLon(Coord coord, LatLon &ll);
  virtual void convertToCoord(LatLon ll, Coord &coord);
};
