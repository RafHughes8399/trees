#pragma once


#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>


// the private methods take a node* tree, the public methods 
// call the private method and pass in root_

#define BALANCE_INTERVAL 15


namespace game{
	struct Vector3{
		float x;
		float y;
		float z;

	};
	
	inline Vector3 Vector3Add(Vector3 a, Vector3 b){
		return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};

	}
	inline Vector3 Vector3Scale(Vector3 a, float s){
		return Vector3{a.x * s, a.y * s, a.z * s};

	}

	inline bool Vector3Equal(Vector3 a, Vector3 b){
		return a.x == b.x and a.y == b.y and a.z == b.z;
	}
	struct BoundingBox{
		Vector3 min;
		Vector3 max;

	};
	inline bool BoundingBoxEqual(BoundingBox a, BoundingBox b){
		return Vector3Equal(a.min, b.min) and Vector3Equal(a.max, b.max);
	}
	inline void print_box(BoundingBox& a){
		std::cout << "MIN : " << a.min.x << ", " << a.min.y << ", " << a.min.z << std::endl; 
		std::cout << "MAX : " << a.max.x << ", " << a.max.y << ", " << a.max.z << std::endl; 

	}
	class Object {
		public:
			virtual ~Object() = default;
			Object(Vector3 position, Vector3 size)
				: position_(position), size_(size){
				// generate the bounding box, min and max
				bounding_box_ = BoundingBox{ Vector3{position_.x - (size_.x / 2), position_.y - (size_.y / 2), position_.z - (size_.z / 2)},
					Vector3{position_.x + (size_.x / 2), position_.y + (size_.y / 2), position_.z + (size_.z / 2)} };			
				};
			Object(const Object& other)
				:  position_(other.position_), size_(other.size_), bounding_box_(other.bounding_box_){
			};
		
			Object(Object&& other)
				: position_(std::move(other.position_)), size_(std::move(other.size_)), bounding_box_(std::move(other.bounding_box_)) {
			};

			BoundingBox get_bounding_box(){
				return bounding_box_;
			}
			
    		virtual bool operator==(const Object& other) const = 0;  // Make it const and pure virtual
			inline void print_object(){
				
				std::cout << "object at position " << position_.x << ", " << position_.y << ", " 
				<< position_.z <<  std::endl;
				
				print_box(bounding_box_);
			}
		protected:
			// all objects have a model and position, and a hitbox
			Vector3 position_;
			Vector3 size_;
			BoundingBox bounding_box_;
		};
		
	class TestObject : public Object {
	public:
		~TestObject() override = default;
		TestObject(game::Vector3 position, game::Vector3 size)
		: Object(position, size){
		};
		TestObject(const TestObject& other)
			: Object(other){
	
		};
		TestObject(TestObject&& other)
			: Object(other) {
		};			
		bool operator==(const Object& other) const override {  // Make it const
			auto* other_test = dynamic_cast<const TestObject*>(&other);
			if (!other_test) return false;
			
			return game::Vector3Equal(position_, other_test->position_) and Vector3Equal(
				size_, other_test->size_) and BoundingBoxEqual(bounding_box_, other_test->bounding_box_);
			}
	};	
}


namespace tree {
	template <typename T>
	class bst {
		struct node {
			T data_;
			std::unique_ptr<node> left_;
			std::unique_ptr<node> right_;

			node() : data_(), left_(nullptr), right_(nullptr) {}
			node(const T& data) : data_(data), left_(nullptr), right_(nullptr) {}
		};

	private:

		std::unique_ptr<node> root_;
		short inserts_since_balance_ = 0;

//----------------EQUALS----------------------------------
		// checks equal content and structure
		bool equals(node* tree1, node* tree2) const {
			// cases :
				// both null - equal
				// neither null - check data
				// otherwise in equal 
			// both null - true
			if (tree1 == nullptr && tree2 == nullptr) {
				return true;
			}
			// neither null - check data
			else if (tree1 != nullptr && tree2 != nullptr) {
				// if data is equal continue checking
				if (tree1->data_ == tree2->data_) {
					bool left = equals(tree1->left_.get(), tree2->left_.get());
					bool right = equals(tree1->right_.get(), tree2->right_.get());
					return left && right;
				}

				// if data is not equal - false
				else {
					return false;
				}
			}

			// one is null but the other is not - false
			else {
				return false;
			}
		}
//------------- MIN AND MAX -------------------------------
		node* min(node* tree) const {
			// go as far left as possible
			if (tree == nullptr) { return nullptr; }
			auto current = tree;
			while (current->left_ != nullptr) {
				current = current->left_.get();
			}
			return current;
		}
		node* max(node* tree) const {
			// same logic as left, just with the right subtree instead
			if (tree == nullptr) { return nullptr; }
			auto current = tree;
			while (current->right_ != nullptr) {
				current = current->right_.get();
			}
			return current;
		}

//---------------------JOIN-----------------------------
		void join(std::unique_ptr<node>& tree1, std::unique_ptr<node>& tree2) {
			// assumes that max t1 < min t2, min t2 becomes the new root of the overall tree
			if (tree1 == nullptr || tree2 == nullptr) { 
				return; 
			}
			else {

				// current and parent should be pointers to unique pointers
				// then to access the unique pointer, defreference the pointer
				// these might need to be unique ptrs but i still need to access the rest of tree 2 later on 
				std::unique_ptr<node>* current = &tree2;
				std::unique_ptr<node>* parent = nullptr;
				while ((*current)->left_ != nullptr) {
					parent = current;
					current = &((*current)->left_);
				}

				// if current (the minimum node of tree 2) is the root, then it has no
				// left subtree, so you only need to attach tree 1 to it 

				// othwerwise, if current is not the root node of tree two
				if(parent != nullptr) {
					// then set parent->left to current->right, replace the minimum node with its right subtree to retain it
					// then attach tree2 to current's right subtree
					std::unique_ptr<node> min_node = std::move(*current);
					*current = std::move(min_node->right_);
					min_node->right_ = std::move(tree2);
					min_node->left_ = std::move(tree1);
					tree1 = std::move(min_node);
					return;
				}
				else {
					(*current)->left_ = std::move(tree1);
					tree1 = std::move(*current);
					infix(tree1);
					return;
				}
			}
		}

//--------INSERT AND ERASE-------------------
		// a lesson in scope and copying
		// that is where most of your memeory management errors would come from, you're making a copy and assigning it
		// to the original, yet the copy goes out of scope making the original null 
		// so when you want to modify, use a reference to retain the original 

		// it all goes back to those two key ideas, that's where your memory is being lost
		// copies that go out of scope
		void insert(const T& data, std::unique_ptr<node>& tree) {
			if (tree == nullptr) {
				tree = std::make_unique<node>(node(data));
				inserts_since_balance_++;
				return;
			}
			else {
				if (data < tree->data_) {
					insert(data, tree->left_);
				}
				else if (data > tree->data_) {
					insert(data, tree->right_);
				}
			}
		}

		// delete a node with data
		void erase(const T& data, std::unique_ptr<node>& tree) {
			if (tree == nullptr) {
				return;
			}
			// traverse the tree to find the node
			if (data < tree->data_) {
				erase(data, tree->left_);
			}
			else if (data > tree->data_) {
				erase(data, tree->right_);
			}
			// if found
			else{
				// case 1 : no children, just remove the root / leaf 
				if (tree->left_ == nullptr && tree->right_ == nullptr) {
					tree.reset(); // delete the pointer
				}
				// case 2 : only right child, repositioning the right child
				else if (tree->left_ == nullptr) {
					// replace tree with the right child 
					tree = std::move(tree->right_);
					return;
				}
				// Case 3: Only left child
				else if (tree->right_ == nullptr) {
					tree = std::move(tree->left_);
					return;
				}
				// case 4 : both children, joining their subtrees
				else {					
					auto left = std::move(tree->left_);
					auto right = std::move(tree->right_);


					join(left, right);
					tree = std::move(left);
					return;
				}
			}
			return;				// if not found
		}

//------------------------CLEAR--------------------------------------------
		void clear(std::unique_ptr<node>& tree) {
			// recursively reset the pointers of each tree
			if (tree != nullptr) {
				// clear the left subtree 
				clear(tree->left_);
				// clear the right subtree
				clear(tree->right_);
				// reset the root
				tree.reset();
			}
		}
//----------------------HEIGHT, SIZE, CONTAINS AND LOOKUP------------------

		size_t size(node* tree) const {
			if (tree == nullptr) { return 0;}
			else {return size_t(1 + size(tree->left_.get()) + size(tree->right_.get()));}
		}

		int height(node* tree) const {
			if (tree == nullptr) {return -1;}
			else {
				int left_height = height(tree->left_.get());
				int right_height = height(tree->right_.get());

				if (left_height > right_height) {					
					return 1 + left_height;
				}
				else {
					return 1 + right_height;
				}
			}
		}

		bool contains(const T& data, node* tree) const {
			if (tree == nullptr) { return false;}

			else {
				if (data < tree->data_) {
					return contains(data, tree->left_.get());
				}
				else if (data > tree->data_) {
					return contains(data, tree->right_.get());
				}
				else {
					return true;
				}
			}
		}

		node* find(const T& data, node* tree) const {
			if (tree == nullptr) {return nullptr;}

			else {
				if (data < tree->data_) {
					return find(data, tree->left_.get());
				}

				else if (data > tree->data_) {
					return find(data, tree->right_.get());
				}
				
				else {
					return tree;
				}
			}
		}

//----------------------------ROTATION, BALANCING AND balance------------------
		void rotate_right(std::unique_ptr<node>& tree) {
			if (tree == nullptr || tree->left_ == nullptr) {
				return;
			}
			std::unique_ptr<node> new_root = std::move(tree->left_);
			tree->left_ = std::move(new_root->right_);
			new_root->right_ = std::move(tree);
			tree = std::move(new_root);
		}

		void rotate_left(std::unique_ptr<node>& tree) {
			// left and right swap from rotate_right
			if (tree == nullptr || tree->right_ == nullptr) {
				return;
			}

			std::unique_ptr<node> new_root = std::move(tree->right_);
			tree->right_ = std::move(new_root->left_);
			new_root->left_ = std::move(tree);
			tree = std::move(new_root);
		}

		// a tree is considered balanced if  the left and right subtrees differ by less than one
		void balance(std::unique_ptr<node>& tree, size_t index) {
			size_t left_size = size(tree->left_.get());

			// repeats until the index and the left subtree are the same height
			// in the case of size / 2, it evens the tree out  

			// comparably an expensive operation, do not want to do that often, once every x inserts 
			/*
				if the mid point of the tree is on the left, it means that the left 
				is "taller" than the right, so rotate right
			*/
			if (index < left_size) {
				balance(tree->left_, index);
				rotate_right(tree);
				return;
			}
			// rebalance in favour of the right
			/*
				if the mid point is on the right hand side, then rotate left,
				then index moves one to the lefft 
			*/
			else if(index > left_size){
				balance(tree->right_, size_t(index - left_size - 1));
				rotate_left(tree);
				return;
			}
			// a tree is balanced if index == left_size
			return;
		}

//-----------------------------CLEAR AND COPY-------------------------------------
		std::unique_ptr<node> copy_subtree(node* tree) {
			if (tree == nullptr) {
				return nullptr;
			}

			auto new_node = std::make_unique<node>();
			new_node->data_ = tree->data_;
			new_node->left_ = copy_subtree(tree->left_.get());
			new_node->right_ = copy_subtree(tree->right_.get());

			return new_node;
		}
//---------------------------TRAVERSAL-----------------------------------
		void infix(std::unique_ptr<node>& tree) const {
			if (tree != nullptr) {
				// go left
				infix(tree->left_);
				// print node
				std::cout << tree->data_ << " ";
				// go right
				infix(tree->right_);
			}
			else {
				return;
			}
		}
		void prefix(std::unique_ptr<node>& tree) const {
			if (tree != nullptr) {
				// print node
				std::cout << tree->data_ << " ";
				// go left
				prefix(tree->left_);
				// go right
				prefix(tree->right_);
			}
			else {
				return;
			}
		}

		void postfix(const std::unique_ptr<node>& tree) const {
			if (tree != nullptr) {
				// go left
				postfix(tree->left_);
				// go right
				postfix(tree->right_);
				// print node
				std::cout << tree->data_ << " " ;
			}
			else {
				return;
			}
		}
		void infix(std::unique_ptr<node>& tree) {
			if (tree != nullptr) {
				// go left
				infix(tree->left_);
				// print node
				std::cout << tree->data_ << " ";
				// go right
				infix(tree->right_);
			}
			else {
				return;
			}
		}
		void prefix(std::unique_ptr<node>& tree) {
			if (tree != nullptr) {
				// print node
				std::cout << tree->data_ << " ";
				// go left
				prefix(tree->left_);
				// go right
				prefix(tree->right_);
			}
			else {
				return;
			}
		}

		void postfix(std::unique_ptr<node>& tree) {
			if (tree != nullptr) {
				// go left
				postfix(tree->left_);
				// go right
				postfix(tree->right_);
				// print node
				std::cout << tree->data_ << " ";
			}
			else {
				return;
			}
		}
	public:
		~bst() = default;

		// empty tree construction
		// tree with root node construction, has data and no children

		bst()
			: root_{ nullptr } {

		}
		bst(const T& data)
			: root_(std::make_unique<node>(node(data))) {
		}

		template<typename InputIt>
		bst(InputIt first, InputIt last)
			// start with an empty tree 
			: bst() {
			auto num = std::distance(first, last);
			for (auto it = first; it != last; ++it) {
				insert(*it);
			}
			balance(size_t(num / 2));
		}
		// INIT LIST CONSTRUCTOR
		bst(std::initializer_list<T> list)
			: bst(list.begin(), list.end()) {
		}

		bst(const bst& other) 
			: inserts_since_balance_(other.inserts_since_balance_) {
				root_ = copy_subtree(other.root_.get());
		}

		// Copy assignment operator
		bst& operator=(const bst& other) {
			inserts_since_balance_ = other.inserts_since_balance_;
			root_ = copy_subtree(other.root_.get());
			return *this;
		}

		// Keep move semantics too (best of both worlds)
		bst(bst&&) = default;
		bst& operator=(bst&&) = default;

		// INSERTION AND DELETION
		void insert(const T& data) {
			insert(data, root_);
			if (inserts_since_balance_ >= BALANCE_INTERVAL) {
				balance(size_t(size() / 2));
				inserts_since_balance_ = 0;
			}
		}

		void erase(const T& data) {
			erase(data, root_);
		}

		// ACCESSORS
		node* root() const {
			return root_.get();
		}
		node* left() const {
			return root_->left_.get();
		}
		node* right() const {
			return root_->right_.get();
		}

		// SIZE AND HEIGHT
		size_t size() const {
			return size(root_.get());
		}
		bool is_empty() const {
			return root_ == nullptr;
		}
		int height() const {
			return height(root_.get());
		}

		// CONTAINS AND FIND
		bool contains(const T& data) const {
			return contains(data, root_.get());
		}

		node* find(const T& data) const {
			return find(data, root_.get());
		}
		
		void clear() {
			clear(root_);
		}

		// MIN AND MAX
		T min() const {
			return min(root_.get())->data_;
		}

		T max() const {
			return max(root_.get())->data_;
		}
		// TRAVERSAL
		void prefix_traversal() const {
			prefix(root_);
			std::cout << std::endl;
		}
		void infix_traversal() const {
			infix(root_);
			std::cout << std::endl;
		}
		void postfix_traversal() const {
			postfix(root_);
			std::cout << std::endl;
		}
		void prefix_traversal() {
			prefix(root_);
			std::cout << std::endl;
		}
		void infix_traversal(){
			infix(root_);
			std::cout << std::endl;
		}

		void postfix_traversal(){
			postfix(root_);
			std::cout << std::endl;
		}
		void join(bst& other) {
			// the tree calling join is tree1, the tree passed into the function is tree 2
			// interpretable as joining this tree to the other tree
				// ensure that the left tree is less than the right subtree
				// to maintain ordering
			if (is_empty()) {
				root_ = std::move(other.root_);
			}
			else if (other.is_empty()) {
				return;
			}
			else {
				if (max() < other.min()) {
					join(root_, other.root_);
				}
				else if (max() == other.min()) {
					throw std::runtime_error("cannot join the trees, max value of this tree is the same as the min value of the other tree");
				}
				else{
						throw std::runtime_error("cannot join the trees, max value of this tree is not less than min value of other tree");
					}

			}
		}
		void rotate_right() {
			rotate_right(root_);
		}
		void rotate_left() {
			rotate_left(root_);
		}

		void balance(size_t index) {
			balance(root_, index);
		}		

		// OPERATOR OVERLOADS
		friend std::ostream& operator<<(std::ostream& os, const bst& tree) {
			os << tree.root_->data_;
			return os;
		}
		bool operator==(const bst& other) const {
			// check the data of the node
			return equals(root_.get(), other.root_.get());
		}
	};


	#define NODE_LIFETIME 30 // seconds
	#define MAX_DEPTH 5 // max height of the tree 

	class octree {
	protected:
		// node definition
		struct o_node {
			std::vector<std::unique_ptr<game::Object>> objects_;
			std::vector<std::unique_ptr<o_node>> children_;
			game::BoundingBox bounds_;
			int depth_;
			short life_; // how long a o_node has lived without any objects
		};
	
		private:
		std::unique_ptr<o_node> root_;
		int max_depth_;
		// methods
	
	
		// checks if an object is contained within a o_node's bounding box
		bool node_contains_object(game::BoundingBox& node, game::BoundingBox& object);
		bool object_contained_by_child(game::BoundingBox& node, game::BoundingBox& object);
		// build the children of a leaf node in the tree
		void build_children(std::unique_ptr<o_node>& tree);

		// insert an object into the tree
		void insert(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);
		void erase(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);

		// reposition an Object within the tree after it moves
		std::unique_ptr<game::Object> extract(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);
		void reposition(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);


		// get the o_node the object is located in 
		o_node* find_object_node(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);
	
		// same logic but returns the object instead of the o_node 
		game::Object* find_object(std::unique_ptr<o_node>& tree, std::unique_ptr<game::Object>& object);
		
		int height(std::unique_ptr<o_node>& tree);
		size_t size(std::unique_ptr<o_node>& tree);
		
		bool is_empty(std::unique_ptr<o_node>& tree);
		bool is_leaf(std::unique_ptr<o_node>& tree);
	
		void check_leaves(std::unique_ptr<o_node>& tree, double delta);
		
		void traverse_tree(std::unique_ptr<o_node>& tree);
		public:
		// CONSTRUCTORS
		~octree() = default;
		// creates an empty octree with a root o_node
		octree(game::BoundingBox root_bounds, int depth=MAX_DEPTH)
		: root_(std::make_unique<o_node>()), max_depth_(depth) {
			root_->bounds_ = root_bounds;
			root_->life_ = 0;
			root_->depth_ = 0;
			
			// build lazily
		}
		// creates an empty octree, then populates it with the list of objects
		template<typename InputIt>
		octree(game::BoundingBox root_bounds, InputIt first, InputIt last)
		: octree(root_bounds) { // initialise the root o_node
			for (auto i = first; i != last; ++i) {
				insert(*i);
			}
		}
		
		octree(game::BoundingBox root_bounds, std::vector<std::unique_ptr<game::Object>>& objects)
		: octree(root_bounds, objects.begin(), objects.end()) {
		}
		
		// copy and move overloads
		octree(const octree& other);
		octree(octree&& other);
		
		octree& operator= (const octree& other);
		octree& operator=(octree&& other);
	
		void insert(std::unique_ptr<game::Object>& obj) {
			insert(root_, obj);
		}
		void erase(std::unique_ptr<game::Object>& obj);
		std::unique_ptr<game::Object> extract(std::unique_ptr<game::Object>& obj);
		void reposition(std::unique_ptr<game::Object>& obj);
		
		std::unique_ptr<o_node>& get_root() {
			return root_;
		}
		std::vector<std::unique_ptr<o_node>>& get_children() {
			return root_->children_;
		}
		std::vector<std::unique_ptr<game::Object>>& get_objects() {
			return root_->objects_;
		}
		
		int max_depth(){
			return max_depth_;
		}
		int height() {
			return height(root_);
		}
		size_t size() {
			return size(root_);
		}
	
		bool is_leaf() {
			return is_leaf(root_);
		}
		bool is_empty() {
			return is_empty(root_);
		}
	
		// checks leaves for their life, prunes if need be
		void check_leaves(double delta) {
			check_leaves(root_, delta);
		}
	
		o_node* find_object_node(std::unique_ptr<game::Object>& obj) {
			return find_object_node(root_, obj);
		}
	
		game::Object* find_object(std::unique_ptr<game::Object>& obj) {
			return find_object(root_, obj);
		}
	
		// for testing purposes 
		bool object_in_node(game::BoundingBox& node, game::BoundingBox& obj) {
			return node_contains_object(node, obj);
		}
		
		void traverse_tree(){
			traverse_tree(root_);
		}
	};
}

