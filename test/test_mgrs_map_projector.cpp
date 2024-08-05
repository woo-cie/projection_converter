#include <gtest/gtest.h>

#include "map_projector/mgrs_map_projector.hpp"

class MgrsMapProjectorTest : public ::testing::Test {
protected:
  MgrsMapProjector proj;
  LatLon ll;
  Coord coord;
  MgrsMapProjectorTest();
  void expectAlmostSameLatLon(LatLon a, LatLon b);
  void expectAlmostSameCoord(Coord a, Coord b);
};

MgrsMapProjectorTest::MgrsMapProjectorTest()
    : proj{"53SNU"},
      ll{
          34.34220456009112,
          135.0010872416819,
      },
      coord{
          100.0,
          100.0,
      } {};

void MgrsMapProjectorTest::expectAlmostSameLatLon(LatLon a, LatLon b) {
  EXPECT_NEAR(a.lat, b.lat, 1.0e-9);
  EXPECT_NEAR(a.lon, b.lon, 1.0e-9);
};

void MgrsMapProjectorTest::expectAlmostSameCoord(Coord a, Coord b) {
  EXPECT_NEAR(a.x, b.x, 1.0e-4);
  EXPECT_NEAR(a.y, b.y, 1.0e-4);
};

TEST_F(MgrsMapProjectorTest, CoordToLatlon) {
  auto val = proj.convertToLatLon(coord);
  expectAlmostSameLatLon(ll, val);
}

TEST_F(MgrsMapProjectorTest, LatlonToCoord) {
  auto val = proj.convertToCoord(ll);
  expectAlmostSameCoord(coord, val);
}
