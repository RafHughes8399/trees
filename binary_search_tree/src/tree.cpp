#include "tree.h"


bool tree::octree::node_contains_object(game::BoundingBox& node, game::BoundingBox& object){
	return (object.min.x > node.min.x and object.min.y > node.min.y and object.min.z > node.min.z)
		and (object.max.x < node.max.x and object.max.y < node.max.y and object.max.z < node.max.z);
}

void tree::octree::build_children(std::unique_ptr<o_node>& tree){
			// first do quadrants then do octant	
			// left or right, bottom or top, front or back
			auto centre_point = game::Vector3Add(tree->bounds_.max, tree->bounds_.min);
			centre_point = game::Vector3Scale(centre_point, 0.5);
	
			// left bottom front, (min -> centre x, y) (centre -> max z)
			auto lbf = std::make_unique<o_node>();
			lbf->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x, tree->bounds_.min.y, centre_point.z},
				game::Vector3{centre_point.x, centre_point.y, tree->bounds_.max.z}};
			lbf->life_ = 0;
			lbf->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(lbf));
	
			// left top front (min -> centre x), (centre -> max y, z)
			auto ltf = std::make_unique<o_node>();
			ltf->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x, centre_point.y, centre_point.z},
				game::Vector3{centre_point.x, tree->bounds_.max.y, tree->bounds_.max.z}
			};
			ltf->life_ = 0;
			ltf->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(ltf));
	
			// left bottom back (min -> centre x, y, z)
			auto lbb = std::make_unique<o_node>();
			lbb->bounds_ = game::BoundingBox{tree->bounds_.min, centre_point};
			lbb->life_ = 0;
			lbb->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(lbb));
	
			// left top back (min -> centre x, z) (centre -> max, y)
			auto ltb = std::make_unique<o_node>();
			ltb->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x, centre_point.y, tree->bounds_.min.z},
				game::Vector3{centre_point.x, tree->bounds_.max.y, centre_point.z}
			};
			ltb->life_ = 0;
			ltb->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(ltb));
	
			// right bottom front (min -> centre y) (centre -> max x, z)
			auto rbf = std::make_unique<o_node>();
			rbf->bounds_ = game::BoundingBox{game::Vector3{centre_point.x, tree->bounds_.min.y, centre_point.z}, 
				game::Vector3{tree->bounds_.max.x, centre_point.y, tree->bounds_.max.z}
			};
			rbf->life_ = 0;
			rbf->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(rbf));
	
			// right top front (centre -> max x, y, z)
			auto rtf = std::make_unique<o_node>();
			rtf->bounds_ = game::BoundingBox{centre_point, tree->bounds_.max};
			rtf->life_ = 0;
			rtf->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(rtf));
	
			// right bottom back (min -> centre y, z) (centre -> max, x)
			auto rbb = std::make_unique<o_node>();
			rbb->bounds_ = game::BoundingBox{game::Vector3{centre_point.x, tree->bounds_.min.y, tree->bounds_.min.z}, 
				game::Vector3{tree->bounds_.max.x, centre_point.y, centre_point.z} };
			rbb->life_ = 0;
			rbb->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(rbb));
	
			// right top back (min -> centre z) ( centre -> max x y)
			auto rtb = std::make_unique<o_node>();
			rtb->bounds_ = game::BoundingBox{ game::Vector3{centre_point.x, centre_point.y, tree->bounds_.min.z},
			game::Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre_point.z} };
			rtb->life_ = 0;
			rtb->depth_ = tree->depth_ + 1;
			tree->children_.push_back(std::move(rtb));
}

void tree::octree::insert(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object){
	// find the smallest node that the object fits in and insert it into the vector
	// such that the max depth of the tree is not exceeded
	
	// if fits in parent, check the children
	auto object_bounds = object->get_bounding_box();
	if (node_contains_object(tree->bounds_, object_bounds)) {
		// if there are no children, check depth, if node is not at the max dpeth, generate the children
		if (is_leaf(tree) && tree->depth_ != max_depth_) {
			build_children(tree);
			// then check the children,  
		}
		else if (tree->depth_ == max_depth_) {
			tree->objects_.push_back(std::move(object));
			return;
		}
		// then check the children
		for (auto& child : tree->children_) {
			// if does fit in a child, recursively
			if (node_contains_object(child->bounds_, object_bounds)) {
				insert(child, object);
				return;
			}
		}
		// otherwise insert into current node
		tree->objects_.push_back(std::move(object));
	}
	// if does not fit in node do not insert
	return;
}

tree::octree::o_node* tree::octree::find_object_node(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object) {
    if (!tree) {
        return nullptr;
    }
    for (auto& obj : tree->objects_) {
        if (*obj == *object) {
            return tree.get();
        }
    }
    for (auto& child : tree->children_) {
        auto result =  find_object_node(child, object);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

game::Object* tree::octree::find_object(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object) {
    if (!tree) return nullptr;

    // Check if object is in current o_node
    for (auto& obj : tree->objects_) {
        if (obj.get() == object.get()) {  // Compare raw pointers
            return obj.get();
        }
    }

    // Recursively search children
    for (auto& child : tree->children_) {
        auto result = find_object(child, object);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;  // Not found
}

int tree::octree::height(std::unique_ptr<o_node>& tree) {
    if (!tree) {
        return -1;
    }
    else {
        int max_child_height = -1;
        for (auto& child : tree->children_) {
            int child_height = height(child);
            if (child_height >= max_child_height) {
                max_child_height = child_height;
            }
        }
        return 1 + max_child_height;
    }
}
size_t tree::octree::size(std::unique_ptr<o_node>& tree) {
    auto empty = is_empty(tree);
    if (empty) { return 0; }

    if (tree != nullptr) {
        auto t_size = tree->objects_.size();
        for (auto& child : tree->children_) {
            t_size += size(child);
        }
        return t_size;
    }
    return 0;
}


bool tree::octree::is_empty(std::unique_ptr<o_node>& tree) {
    // check the current list 
    auto empty = tree->objects_.size() == 0 ? true : false;
    // check the children
    if (! empty) { return false; }
    for (auto& child : tree->children_) {
        if (not is_empty(child)) {
            return false;
        }
    }
    return true;
}
bool tree::octree::is_leaf(std::unique_ptr<o_node>& tree) {
    return tree->children_.size() == 0 ? true : false;
}

void tree::octree::check_leaves(std::unique_ptr<o_node>& tree, double delta) {
        if (is_leaf(tree)) {
            tree->life_ += short(delta);
            if (tree->life_ > NODE_LIFETIME) {
                tree.reset();
                return;
            }
        }
        else {
            for (auto& child : tree->children_) {
                check_leaves(child, delta);
            }
        }
    return;
} 

void tree::octree::traverse_tree(std::unique_ptr<o_node>& tree){
		// print the box of the node 
		if(!tree){
			return;
		}
		game::print_box(tree->bounds_);
		for(auto& object : tree->objects_){
			object->print_object();
		}
		for(auto& child : tree->children_){
			traverse_tree(child);
		}
		// print its children
		return;
}