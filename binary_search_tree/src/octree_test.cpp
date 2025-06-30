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
    std::unique_ptr<game::Object> test_object = std::make_unique<game::TestObject>(test_pos, test_size, 0);
    auto obj_box = test_object->get_bounding_box();

    std::unique_ptr<game::Object> search_object = std::make_unique<game::TestObject>(test_pos, test_size, 0);


    otree.insert(test_object);
    // check object was inserted
    CHECK(otree.size() == 1);
    
    // 
    auto node = otree.find_object_node(search_object);
    CHECK(node->objects_.size() == 1);
    
    // check that the object fits in the node but not its children,
    // if they do exist
    CHECK(otree.object_in_node(node->bounds_, obj_box));
    // check that root is a leaf, no toher nodes should be created  
    CHECK(otree.is_leaf());
    CHECK(otree.height() == 0);

    for(auto& child : node->children_){
        CHECK( not otree.object_in_node(child->bounds_, obj_box));
    }


}

TEST_CASE("insert inspect"){
    // check the qualities of the node that the object was inserted in 
    auto otree = tree::octree(WORLD_BOX);
    auto test_pos = game::Vector3{0.0, 0.0, 0.0};
    auto test_size = game::Vector3{20.0, 20.0, 20.0};
    std::unique_ptr<game::Object> test_object = std::make_unique<game::TestObject>(test_pos, test_size, 0);
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
    std::unique_ptr<game::Object> lbb = std::make_unique<game::TestObject>(centre, size, 0);
    
    CHECK(otree.get_children().size() == 0);

    otree.insert(lbb);
    CHECK(otree.get_children().size() == 1);

    centre = game::Vector3Add(game::Vector3{-762, -64, 0}, game::Vector3{0, 0, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> lbf = std::make_unique<game::TestObject>(centre, size, 1);
    
    otree.insert(lbf);
    CHECK(otree.get_children().size() == 2);
    
    centre = game::Vector3Add(game::Vector3{-762, 0, -762}, game::Vector3{0, 64, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> ltb = std::make_unique<game::TestObject>(centre, size, 2);
    
    otree.insert(ltb);
    CHECK(otree.get_children().size() == 3);
    
    centre = game::Vector3Add(game::Vector3{-762, 0, 0}, game::Vector3{0, 64, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> ltf = std::make_unique<game::TestObject>(centre, size, 3);
    
    otree.insert(ltf);
    CHECK(otree.get_children().size() == 4);
    
    centre = game::Vector3Add(game::Vector3{0, -64, -762}, game::Vector3{762, 0, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rbb = std::make_unique<game::TestObject>(centre, size, 4);
    
    otree.insert(rbb);
    CHECK(otree.get_children().size() == 5);
    
    
    centre = game::Vector3Add(game::Vector3{0, -64, 0}, game::Vector3{762, 0, 762});
    centre = game::Vector3Scale(centre, 0.5);
    
    std::unique_ptr<game::Object> rbf = std::make_unique<game::TestObject>(centre, size, 5);
    
    otree.insert(rbf);
    CHECK(otree.get_children().size() == 6);
    
    centre = game::Vector3Add(game::Vector3{0, 0, -762}, game::Vector3{762, 64, 0});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rtb = std::make_unique<game::TestObject>(centre, size, 6);
    
    otree.insert(rtb);
    CHECK(otree.get_children().size() == 7);
    
    centre = game::Vector3Add(game::Vector3{0, 0, 0}, game::Vector3{762, 64, 762});
    centre = game::Vector3Scale(centre, 0.5);
    std::unique_ptr<game::Object> rtf = std::make_unique<game::TestObject>(centre, size, 7);

    otree.insert(rtf);

    CHECK(otree.get_children().size() == 8);
    CHECK(otree.size() == 8);

    CHECK(otree.height() == 1); // should insert directly into the root's children
}
TEST_CASE("insert multiple - same node"){
    auto otree = tree::octree(WORLD_BOX);

    // inserting into right top back 
    auto position = game::Vector3{300, 20, -300};
    auto size = game::Vector3{10, 10, 10};
    std::unique_ptr<game::Object> test_obj = std::make_unique<game::TestObject>(position, size, 0);
    otree.insert(test_obj);
    
    position = game::Vector3Add(position, game::Vector3{10, -5, 20});
    std::unique_ptr<game::Object> another_test_obj = std::make_unique<game::TestObject>(position, size, 1);
    otree.insert(another_test_obj);
    
    position = game::Vector3Add(position, game::Vector3{-20, 0, -10});
    std::unique_ptr<game::Object> and_another_test_obj = std::make_unique<game::TestObject>(position, size, 2);
    otree.insert(and_another_test_obj);
    
    CHECK(otree.size() == 3);
    CHECK(otree.height() == 2);
    CHECK(otree.get_children().size() == 1);
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
        std::unique_ptr<game::Object> obj = std::make_unique<game::TestObject>(position, size, i);
        otree.insert(obj);
    }
   // CHECK(otree.size() == 23);
    CHECK(otree.height() <= 4);
}

TEST_CASE("insert to max depth"){
    // insert an object at the max depth of a tree, compare two trees
    // with different max depths
    auto otree_3 = tree::octree(WORLD_BOX, 3);
    auto otree_4 = tree::octree(WORLD_BOX, 4);
    auto otree_max = tree::octree(WORLD_BOX);

    auto position = game::Vector3{5, 1.5, 5};
    auto size = game::Vector3{0.5, 0.5, 0.5};

    std::unique_ptr<game::Object> obj_3 = std::make_unique<game::TestObject>(position, size, 0);
    std::unique_ptr<game::Object> obj_4 = std::make_unique<game::TestObject>(position, size, 0);
    std::unique_ptr<game::Object> obj_5 = std::make_unique<game::TestObject>(position, size, 0);
    
    otree_3.insert(obj_3);
    otree_4.insert(obj_4);
    otree_max.insert(obj_5);

    CHECK(otree_3.size() == 1);
    CHECK(otree_4.size() == 1);
    CHECK(otree_max.size() == 1);

    CHECK(otree_3.size() == otree_4.size());
    CHECK(otree_4.size() == otree_max.size());

    CHECK(otree_3.height() == otree_3.max_depth());
    CHECK(otree_4.height() == otree_4.max_depth());
    CHECK(otree_max.height() == otree_max.max_depth());


    CHECK(otree_3.height() != otree_4.height());
    CHECK(otree_3.height() != otree_max.height());
    CHECK(otree_4.height() != otree_max.height());
    otree_max.traverse_tree(); // examine what the "smallest" bounding box is

}
TEST_CASE("insert and erase object"){
    // insert an object

    // then erase it from the tree

}

TEST_CASE("erase object not in tree"){
    // attempt to erase an object not in the tree

}

TEST_CASE("insert multiple erase some"){

}

TEST_CASE("erase a whole node"){
    // clear all objects from a node
}

TEST_CASE("pruning leaves, simple"){
    auto octree = tree::octree(WORLD_BOX);

    auto position = game::Vector3{300, 20, -300};
    auto size = game::Vector3{10, 10, 10};
    std::unique_ptr<game::Object> test_obj = std::make_unique<game::TestObject>(position, size, 0);
    std::unique_ptr<game::Object> copy_obj = std::make_unique<game::TestObject>(position, size, 0);

    // simple case, root -> 1 child,
    octree.insert(test_obj);
    // make the child a leaf node,
    octree.update(1);
    CHECK(octree.size() == 1);
    octree.erase(copy_obj); // pending erase 
    CHECK(octree.size() == 0);
    // wait for it to be pruned
    CHECK(octree.height() == 1);
    for(auto i = 0; i <= NODE_LIFETIME; ++i){
        octree.update(1);
    }
    // the leaf should be pruned, no longer in the tree
    CHECK(octree.height() == 0);



}
TEST_CASE("pruning leaves, multiple"){


}

TEST_CASE("pruning leaves, deeper"){


}