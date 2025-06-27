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
TEST_CASE("insert objects into children"){
    auto otree = tree::octree(WORLD_BOX);

    // bang in the centre of the octant
    
    auto centre = game::Vector3Add(game::Vector3{-762, -64, -762}, game::Vector3{0, 0 ,0});
    centre = game::Vector3Scale(centre, 0.5);
    auto size = game::Vector3{10, 10, 10};
    std::unique_ptr<game::Object> lbb = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(lbb);
    
    centre = game::Vector3Add(game::Vector3{-762, -64, 0}, game::Vector3{0, 0, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> lbf = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(lbf);

    centre = game::Vector3Add(game::Vector3{-762, 0, -762}, game::Vector3{0, 64, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> ltb = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(ltb);
    
    centre = game::Vector3Add(game::Vector3{-762, 0, 0}, game::Vector3{0, 64, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> ltf = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(ltf);
    
    centre = game::Vector3Add(game::Vector3{0, -64, -762}, game::Vector3{762, 0, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rbb = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(rbb);
    
    
    centre = game::Vector3Add(game::Vector3{0, -64, 0}, game::Vector3{762, 0, 762});
    centre = game::Vector3Scale(centre, 0.5);
    
    std::unique_ptr<game::Object> rbf = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(rbf);
    
    centre = game::Vector3Add(game::Vector3{0, 0, -762}, game::Vector3{762, 64, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rtb = std::make_unique<game::TestObject>(centre, size);
    
    otree.insert(rtb);
    
    centre = game::Vector3Add(game::Vector3{0, 0, 0}, game::Vector3{762, 64, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rtf = std::make_unique<game::TestObject>(centre, size);

    otree.insert(rtf);

    CHECK(otree.size() == 8);

    CHECK(otree.height() == 1); // should insert directly into the root's children
    otree.traverse_tree();

}
TEST_CASE("insert single - leaf node"){


}
TEST_CASE("insert multiple - same node"){


}
TEST_CASE("insert multiple - different nodes"){

}