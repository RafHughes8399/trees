#include "tree.h"


bool tree::octree::node_contains_object(game::BoundingBox& node, game::BoundingBox& object){
	return (object.min.x > node.min.x and object.min.y > node.min.y and object.min.z > node.min.z)
		and (object.max.x < node.max.x and object.max.y < node.max.y and object.max.z < node.max.z);
}

void tree::octree::build_children(std::unique_ptr<o_node>& tree){
    auto centre_point = game::Vector3Add(tree->bounds_.max, tree->bounds_.min);
    centre_point = game::Vector3Scale(centre_point, 0.5f);
    
    // Create 8 children octants
    // Octant subdivision: Left/Right (x), Bottom/Top (y), Back/Front (z)
    
    // Left Bottom Back (min corner octant)
    auto lbb = std::make_unique<o_node>();
    lbb->bounds_ = game::BoundingBox{
        tree->bounds_.min,  // min corner
        centre_point        // center point
    };
    lbb->life_ = 0;
    lbb->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(lbb));
    
    // Left Bottom Front
    auto lbf = std::make_unique<o_node>();
    lbf->bounds_ = game::BoundingBox{
        game::Vector3{tree->bounds_.min.x, tree->bounds_.min.y, centre_point.z},
        game::Vector3{centre_point.x, centre_point.y, tree->bounds_.max.z}
    };
    lbf->life_ = 0;
    lbf->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(lbf));
    
    // Left Top Back
    auto ltb = std::make_unique<o_node>();
    ltb->bounds_ = game::BoundingBox{
        game::Vector3{tree->bounds_.min.x, centre_point.y, tree->bounds_.min.z},
        game::Vector3{centre_point.x, tree->bounds_.max.y, centre_point.z}
    };
    ltb->life_ = 0;
    ltb->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(ltb));
    
    // Left Top Front
    auto ltf = std::make_unique<o_node>();
    ltf->bounds_ = game::BoundingBox{
        game::Vector3{tree->bounds_.min.x, centre_point.y, centre_point.z},
        game::Vector3{centre_point.x, tree->bounds_.max.y, tree->bounds_.max.z}
    };
    ltf->life_ = 0;
    ltf->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(ltf));
    
    // Right Bottom Back
    auto rbb = std::make_unique<o_node>();
    rbb->bounds_ = game::BoundingBox{
        game::Vector3{centre_point.x, tree->bounds_.min.y, tree->bounds_.min.z},
        game::Vector3{tree->bounds_.max.x, centre_point.y, centre_point.z}
    };
    rbb->life_ = 0;
    rbb->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(rbb));
    
    // Right Bottom Front
    auto rbf = std::make_unique<o_node>();
    rbf->bounds_ = game::BoundingBox{
        game::Vector3{centre_point.x, tree->bounds_.min.y, centre_point.z},
        game::Vector3{tree->bounds_.max.x, centre_point.y, tree->bounds_.max.z}
    };
    rbf->life_ = 0;
    rbf->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(rbf));
    
    // Right Top Back
    auto rtb = std::make_unique<o_node>();
    rtb->bounds_ = game::BoundingBox{
        game::Vector3{centre_point.x, centre_point.y, tree->bounds_.min.z},
        game::Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre_point.z}
    };
    rtb->life_ = 0;
    rtb->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(rtb));
    
    // Right Top Front (max corner octant)
    auto rtf = std::make_unique<o_node>();
    rtf->bounds_ = game::BoundingBox{
        centre_point,        // center point
        tree->bounds_.max    // max corner
    };
    rtf->life_ = 0;
    rtf->depth_ = tree->depth_ + 1;
    tree->children_.push_back(std::move(rtf));
}


bool tree::octree::object_contained_by_child(game::BoundingBox& node, game::BoundingBox& object){

	// check if the object will fit into potential children of the node 
	auto centre = game::Vector3Add(node.max, node.min);
	centre = game::Vector3Scale(centre, 0.5f);

	// if an object bounding box crosses the centre of a node bounding box then it will 
	// not fit into a child 
	return (not (object.min.x < centre.x and centre.x < object.max.x )) 
	and (not (object.min.y < centre.y and centre.y < object.max.y )) 
	and (not (object.min.z < centre.z and centre.z < object.max.z ));

	return true;
}
// amend such that it only creates the children if necessary
void tree::octree::insert(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object){
	auto object_bounds = object->get_bounding_box();
	if(not node_contains_object(tree->bounds_, object_bounds)){ return; }
	else{
		if(tree->depth_ == max_depth_){
			tree->objects_.push_back(std::move(object));
			return;
		}
		else if(is_leaf(tree)){
			// check if children need to be built, otherwise don't build
			// so will the object fit in the prospective children
			if(object_contained_by_child(tree->bounds_, object_bounds)){
				build_children(tree);
				// and then iterate over the children
			}
			else{
				// if the object will not fit into a child, insert into the node without making the children
				tree->objects_.push_back(std::move(object));
			}
		}
		// by this stage, either the leaf's children have been built, 
		for (auto& child : tree->children_) {
		// if does fit in a child, recursively insert
		if (node_contains_object(child->bounds_, object_bounds)) {
			insert(child, object);
			return;
				}
			}
	}
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
		std::cout << "node bounds: "; 
		game::print_box(tree->bounds_);
		std::cout << "node objects:  " << std::endl; 
		for(auto& object : tree->objects_){
			object->print_object();
		}
		for(auto& child : tree->children_){
			traverse_tree(child);
		}
		// print its children
		return;
}