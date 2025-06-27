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
    CHECK(true);
}

TEST_CASE("octree define max height"){
    auto otree = tree::octree(WORLD_BOX, 3);
    CHECK(otree.size() == 0);
    CHECK(otree.height() == 0);

}


TEST_CASE("octree insert"){
    auto otree = tree::octree(WORLD_BOX);
    // max depth is 5

    // test object takes a position and size vector
    auto test_pos = game::Vector3{0.0, 0.0, 0.0};
    auto test_size = game::Vector3{20.0, 20.0, 20.0};
    std::unique_ptr<game::Object> test_object = std::make_unique<game::TestObject>(test_pos, test_size);
    auto obj_box = test_object->get_bounding_box();

    std::unique_ptr<game::Object> search_object = std::make_unique<game::TestObject>(test_pos, test_size);


    otree.insert(test_object);
    // check object was inserted
    CHECK(otree.size() == 1);
    
    // 
    auto node = otree.find_object_node(search_object);
    CHECK(node->objects_.size() == 1);
    
    // check that the object fits in the node but not its children,
    // if they do exist
    CHECK(otree.object_in_node(node->bounds_, obj_box));
    
    for(auto& child : node->children_){
        CHECK( not otree.object_in_node(child->bounds_, obj_box));
    }
}

TEST_CASE("insert inspect"){
    // check the qualities of the node that the object was inserted in 
    auto otree = tree::octree(WORLD_BOX);
    auto test_pos = game::Vector3{0.0, 0.0, 0.0};
    auto test_size = game::Vector3{20.0, 20.0, 20.0};
    std::unique_ptr<game::Object> test_object = std::make_unique<game::TestObject>(test_pos, test_size);
    auto obj_box = test_object->get_bounding_box();

    std::cout << "object bounding box " << std::endl;
    game::print_box(obj_box);

    std::cout << "tree bounding box pre insert " << std::endl;
    // traverse through the tree, print the node box and the child boxes
    otree.traverse_tree();
    
    otree.insert(test_object);
    std::cout << "tree bounding box pre insert " << std::endl;
    otree.traverse_tree();
    

}

TEST_CASE("insert single - leaf node"){


}
TEST_CASE("insert multiple - same node"){


}
TEST_CASE("insert multiple - different nodes"){

}