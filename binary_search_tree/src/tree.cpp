#include "tree.h"


bool tree::octree::node_contains_object(game::BoundingBox& node, game::BoundingBox& object){
	return (object.min.x > node.min.x and object.min.y > node.min.y and object.min.z > node.min.z)
		and (object.max.x < node.max.x and object.max.y < node.max.y and object.max.z < node.max.z);
}
bool tree::octree::is_child_built(std::unique_ptr<o_node>& tree, std::unique_ptr<o_node>& child){
    for(auto& c : tree->children_){
        if(*c == *child){return true;}
    }
    return false;
}

void tree::octree::build_child(std::unique_ptr<o_node>& tree, int child_to_build){
    auto centre = game::Vector3Add(tree->bounds_.max, tree->bounds_.min);
    centre = game::Vector3Scale(centre, 0.5f);

    auto child = std::make_unique<o_node>();
    child->depth_ = tree->depth_ + 1;
    child->life_ = 0;
    child->parent_ = tree.get();
    switch (child_to_build)
    {
    case 0:
        child->bounds_ =  game::BoundingBox{tree->bounds_.min, centre};
        break;
    case 1:
        child->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x,tree->bounds_.min.y, centre.z},  game::Vector3{centre.x, centre.y, tree->bounds_.max.z}};
        break;
    case 2:
        child->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x, centre.y, tree->bounds_.min.z},  game::Vector3{centre.x, tree->bounds_.max.y, centre.z}};
        break;
    case 3:
       child->bounds_ = game::BoundingBox{game::Vector3{tree->bounds_.min.x, centre.y, centre.z},  game::Vector3{centre.x, tree->bounds_.max.y, tree->bounds_.max.z}};
        break;
    case 4:
        child->bounds_ = game::BoundingBox{game::Vector3{centre.x, tree->bounds_.min.y, tree->bounds_.min.z},  game::Vector3{tree->bounds_.max.x, centre.y, centre.z}};
        break;
    case 5:
        child->bounds_ = game::BoundingBox{game::Vector3{centre.x, tree->bounds_.min.y, centre.z},  game::Vector3{tree->bounds_.max.x, centre.y, tree->bounds_.max.z}};
        break;
    case 6:
        child->bounds_ = game::BoundingBox{game::Vector3{centre.x, centre.y, tree->bounds_.min.z},  game::Vector3{tree->bounds_.max.x, tree->bounds_.max.y, centre.z}};
        break;
    case 7:
        child->bounds_ = game::BoundingBox{centre, tree->bounds_.max};
        break;
    default:
        break;
    }
    // check if child_to_build has been built, if not build it 
    if(not is_child_built(tree, child)){
        tree->children_.push_back(std::move(child));
    }
    // Create 8 children octants
    // Octant subdivision: Left/Right (x), Bottom/Top (y), Back/Front (z)
    // Left Bottom Back (min corner octan
}

int tree::octree::object_contained_by_child(game::BoundingBox& node, game::BoundingBox& object){

	// check if the object will fit into potential children of the node 
	auto centre = game::Vector3Add(node.max, node.min);
	centre = game::Vector3Scale(centre, 0.5f);
    // first check if the object crosses the centre of any axis, if it does then no child will fit it
    bool crosses_centre = (object.min.x < centre.x and centre.x < object.max.x) 
    or (object.min.y < centre.y and centre.y < object.max.y) 
    or (object.min.z < centre.z and centre.z < object.max.z);
    
    if(crosses_centre) {return -1;}

    auto children = std::vector<game::BoundingBox>{};
    children.push_back(
        game::BoundingBox{node.min, centre}
    );
    children.push_back(
        game::BoundingBox{game::Vector3{node.min.x,node.min.y, centre.z},  game::Vector3{centre.x, centre.y, node.max.z}}
    );
    children.push_back(
        game::BoundingBox{game::Vector3{node.min.x, centre.y, node.min.z},  game::Vector3{centre.x, node.max.y, centre.z}}
    );
    children.push_back(
        game::BoundingBox{game::Vector3{node.min.x, centre.y, centre.z},  game::Vector3{centre.x, node.max.y, node.max.z}}
    );

    children.push_back(
        game::BoundingBox{game::Vector3{centre.x, node.min.y, node.min.z},  game::Vector3{node.max.x, centre.y, centre.z}}
    );
    children.push_back(
        game::BoundingBox{game::Vector3{centre.x, node.min.y, centre.z},  game::Vector3{node.max.x, centre.y, node.max.z}}
    );
    children.push_back(
        game::BoundingBox{game::Vector3{centre.x, centre.y, node.min.z},  game::Vector3{node.max.x, node.max.y, centre.z}}
    );
    children.push_back(
        game::BoundingBox{centre, node.max}
    );

	// otherwise, check which child will fit the object
    for(size_t i = 0; i < CHILDREN; ++i){
        // check the child that will contain the object
        auto child_node = children.at(i);
        if(node_contains_object(child_node, object)){
            return int(i);
        }
    }
    return -1;
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

        // if not all children for the tree have been built
		else if(tree->children_.size() < CHILDREN){
			// check which child needs to be built, and build that child
            auto child_to_build = object_contained_by_child(tree->bounds_, object_bounds);
            if(child_to_build != -1){
                    build_child(tree, child_to_build);
            }
			else{
				// if the object will not fit into a child, insert into the node without making the children
				tree->objects_.push_back(std::move(object));
                return;
			}
		}
	    for (auto& child : tree->children_) {
		// if does fit in a child, recursively insert
            if (node_contains_object(child->bounds_, object_bounds)) {
                insert(child, object);
                return;
            }
		}
        tree->objects_.push_back(std::move(object));
	}
}

void tree::octree::insert(std::unique_ptr<o_node>& tree, std::vector<std::unique_ptr<game::Object>>& objects){
    // quite similar logic to before, just goes with a list of objects instead, level by level, less overall
    // traversal cost than inserting one by one

    //TODO: implement
    (void) tree;
    (void) objects;
    return;

}

std::unique_ptr<game::Object> tree::octree::erase(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object){
    (void) tree;
    (void) object;
    return std::unique_ptr<game::Object>();
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
        if(*obj == *object) {return obj.get();}
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

// i suspect some issues with this,
// tree builds all nodes at a time, yet they are not all deleted in one go
void tree::octree::prune_leaves(std::unique_ptr<o_node>& tree, double delta) {
        if (is_leaf(tree)) {
            tree->life_ += short(delta);
            if (tree->life_ > NODE_LIFETIME) {
                tree.reset();
                return;
            }
        }
        else {
            // if not a leaf node reset the life
            tree->life_ = 0;
            for (auto& child : tree->children_) {
                prune_leaves(child, delta);
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


void tree::octree::update(double delta){
    // check the lifespan of the node 
    // update objects within the node, tag ones that have been moved
    (void) delta;
    auto moved_objects = std::vector<std::reference_wrapper<std::unique_ptr<game::Object>>>{};
    for(auto& obj : root_->objects_){
        // this depends on obj implementation 
        /*  if(obj->update(delta) == MOVED){
                moved_objects.push_back(obj);
        } */
        (void) obj;
    }
    // reinsert moved objects 
    for(auto& m_obj : moved_objects){
        auto current = root_.get();
        // while the current region does not contain the object, move up a level
        auto box = m_obj.get()->get_bounding_box();
        while(not node_contains_object(current->bounds_, box)){
           current = current->parent_;
        }
        // once the parent is found, erase and then reinsert
        erase(m_obj);
        //insert(current, m_obj.get()); figure out his
    }

    // prune dead objects from the tree
    prune_leaves(delta);
    // then look for collisions

    return;
}