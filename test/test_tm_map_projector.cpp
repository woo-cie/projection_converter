#include <gtest/gtest.h>

#include "map_projector/tm_map_projector.hpp"

class TmMapProjectorTest : public ::testing::Test {
protected:
  TmMapProjector proj;
  LatLon ll;
  Coord coord;
  TmMapProjectorTest();
  void expectAlmostSameLatLon(LatLon a, LatLon b);
  void expectAlmostSameCoord(Coord a, Coord b);
};

TmMapProjectorTest::TmMapProjectorTest()
    : proj{35.0, 135.0}, ll{45.0, 138.0},
      coord{236446.02610120806, 1114286.4401646531} {};

void TmMapProjectorTest::expectAlmostSameLatLon(LatLon a, LatLon b) {
  EXPECT_NEAR(a.lat, b.lat, 1.0e-9);
  EXPECT_NEAR(a.lon, b.lon, 1.0e-9);
};

void TmMapProjectorTest::expectAlmostSameCoord(Coord a, Coord b) {
  EXPECT_NEAR(a.x, b.x, 1.0e-4);
  EXPECT_NEAR(a.y, b.y, 1.0e-4);
};

TEST_F(TmMapProjectorTest, CoordToLatlon) {
  LatLon val;

  proj.convertToLatLon(coord, val);

  expectAlmostSameLatLon(ll, val);
}

TEST_F(TmMapProjectorTest, LatlonToCoord) {
  Coord val;

  proj.convertToCoord(ll, val);

  expectAlmostSameCoord(coord, val);
}
