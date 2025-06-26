#include "tree.h"
#include "../lib/catch2/catch.hpp"

#define WORLD_X 1524.0f
#define WORLD_Y 128.0f
#define WORLD_Z 1524.0f

#define WORLD_MIN game::Vector3{(WORLD_X / 2) * -1, (WORLD_Y /2) * -1, (WORLD_Z / 2) * -1}
#define WORLD_MAX game::Vector3{WORLD_X / 2, WORLD_Y / 2, WORLD_Z / 2}
#define WORLD_BOX game::BoundingBox{WORLD_MIN, WORLD_MAX}


TEST_CASE("octree empty construction"){
    auto otree = tree::octree(WORLD_BOX); 
    CHECK(otree.size() == 0);
    CHECK(otree.height() == 0);
}