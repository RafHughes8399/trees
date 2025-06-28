#include <experimental/random>
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
TEST_CASE("insert multiple - same node"){
    auto otree = tree::octree(WORLD_BOX);

    // inserting into right top back 
    auto position = game::Vector3{300, 20, -300};
    auto size = game::Vector3{10, 10, 10};
    std::unique_ptr<game::Object> test_obj = std::make_unique<game::TestObject>(position, size);
    otree.insert(test_obj);
    
    position = game::Vector3Add(position, game::Vector3{10, -5, 20});
    std::unique_ptr<game::Object> another_test_obj = std::make_unique<game::TestObject>(position, size);
    otree.insert(another_test_obj);
    
    position = game::Vector3Add(position, game::Vector3{-20, 0, -10});
    std::unique_ptr<game::Object> and_another_test_obj = std::make_unique<game::TestObject>(position, size);
    otree.insert(and_another_test_obj);
    
    CHECK(otree.size() == 3);
    CHECK(otree.height() == 2);
}
TEST_CASE("insert multiple - different nodes"){
    auto position = game::Vector3{}; 
    auto size = game::Vector3{5, 5, 5};

    auto otree = tree::octree(WORLD_BOX, 4);
    // non-deterministic - TODO: change
    for(auto i = 0; i < 24; ++i){
        auto x = std::experimental::randint(int(WORLD_MIN.x), int(WORLD_MAX.x));
        auto y = std::experimental::randint(int(WORLD_MIN.y), int(WORLD_MAX.y));
        auto z = std::experimental::randint(int(WORLD_MIN.z), int(WORLD_MAX.z));

        position = game::Vector3{float(x), float(y),float(z)};
        std::unique_ptr<game::Object> obj = std::make_unique<game::TestObject>(position, size);
        otree.insert(obj);
    }
    CHECK(otree.size() == 23);
    CHECK(otree.height() <= 4);
}

TEST_CASE("insert to max depth"){
    // insert an object at the max depth of a tree, compare two trees
    // with different max depths
    auto otree_3 = tree::octree(WORLD_BOX, 3);
    auto otree_4 = tree::octree(WORLD_BOX, 4);
    auto otree_max = tree::octree(WORLD_BOX);

    auto position = game::Vector3{5, 3, 5};
    auto size = game::Vector3{0.5, 0.5, 0.5};

    std::unique_ptr<game::Object> obj_3 = std::make_unique<game::TestObject>(position, size);
    std::unique_ptr<game::Object> obj_4 = std::make_unique<game::TestObject>(position, size);
    std::unique_ptr<game::Object> obj_5 = std::make_unique<game::TestObject>(position, size);
    
    otree_3.insert(obj_3);
    otree_4.insert(obj_4);
    otree_max.insert(obj_5);

    CHECK(otree_3.size() == otree_4.size());
    CHECK(otree_4.size() == otree_max.size());

    CHECK(otree_3.height() == otree_3.max_depth());
    CHECK(otree_4.height() == otree_4.max_depth());
    CHECK(otree_max.height() == otree_max.max_depth());


    CHECK(otree_3.height() != otree_4.height());
    CHECK(otree_3.height() != otree_max.height());
    CHECK(otree_4.height() != otree_max.height());

}

// after insert do erase

// after erase do reposition

// after reposition do pruning 