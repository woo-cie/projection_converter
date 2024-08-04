#include <GeographicLib/MGRS.hpp>
#include <GeographicLib/UTMUPS.hpp>
#include <iostream>
#include <map_projector/mgrs_map_projector.hpp>

MgrsMapProjector::MgrsMapProjector(std::string mgrs_grid) {
  try {
    int prec = 8;
    GeographicLib::MGRS::Reverse(mgrs_grid, zone_, northp_, base_x_, base_y_,
                                 prec, false);

  } catch (const std::exception &e) {
    std::cerr << "Error: Could not convert from MGRS to UTM: " << e.what()
              << "\n";
  }
};

void MgrsMapProjector::convertToLatLon(Coord coord, LatLon &ll) {
  GeographicLib::UTMUPS::Reverse(zone_, northp_, coord.x + base_x_,
                                 coord.y + base_y_, ll.lat, ll.lon);
};

void MgrsMapProjector::convertToCoord(LatLon ll, Coord &coord) {
  GeographicLib::UTMUPS::Forward(ll.lat, ll.lon, zone_, northp_, coord.x,
                                 coord.y);
  coord.x -= base_x_;
  coord.y -= base_y_;
};
