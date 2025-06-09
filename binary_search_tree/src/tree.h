#pragma once

#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>


// the private methods take a node* tree, the public methods 
// call the private method and pass in root_

#define BALANCE_INTERVAL 15

namespace tree {
	template <typename T>
	class bst {
		struct node {
			T data_;
			std::unique_ptr<node> left_;
			std::unique_ptr<node> right_;
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
		node* join(node* tree1, node* tree2) {
			// assumes that max t1 < min t2, min t2 becomes the new root of the overall tree
			if (tree1 == nullptr) { return tree2; }
			else if (tree2 == nullptr) { return tree1; }
			else {
				node* current = tree2;
				node* parent = nullptr;
				// find the minimum node in tree 2	
				while (current->left_ != nullptr) {
					parent = current;
					current = current->left_.get();

				}
				if (parent != nullptr) {
					// replace min by its right subtree if it exists (to make space for tree1)
					parent->left_.reset(current->right_.get());
					// elevate min to the root node of the new tree
					current->right_.reset(tree2);
				}
				// attach tree1 to the left 
				current->left_.reset(tree1);
				return current;
			}
			return tree1;
		}

//--------INSERT AND ERASE-------------------
		node* insert(const T& data, node* tree) {
			if (tree == nullptr) {
				tree = new node();
				tree->data_ = data;
				tree->left_ = nullptr;
				tree->right_ = nullptr;
				inserts_since_balance_++;
			}
			else {
				if (data < tree->data_) {
					tree->left_.reset(insert(data, tree->left_.get()));
				}
				else if (data > tree->data_) {
					// go right
					tree->right_.reset(insert(data, tree->right_.get()));
				}
			}
			return tree;
		}

		// delete a node with data
		node* erase(const T& data, node* tree) {
			// TODO, cases depend on the number of children of the deleted node
			if (tree == nullptr) {
				return nullptr;
			}
				// traverse the tree to find the node
			if (data < tree->data_) {
				tree->left_.reset(erase(data, tree->left_.get()));
			}
			else if (data > tree->data_) {
				tree->right_.reset(erase(data, tree->right_.get()));
			}
			// if found
			else{
				// case 1 : no children, just remove the root / leaf 
				if (tree->left_ == nullptr && tree->right_ == nullptr) {
					return nullptr;
				}
				// case 2 : only right child, repositioning the right child
				else if (tree->left_ == nullptr) {
					node* temp = tree->right_.get();
					delete tree;
					return temp;
				}
				// Case 3: Only left child
				else if (tree->right_ == nullptr) {
					node* temp = tree->left_.get();
					delete tree;
					return temp;
				}
				// case 4 : both children, joining their subtrees
				else {
					// min right becomes the new root
					tree = join(tree->left_.get(), tree->right_.get());
					return tree;
				}
			}
			return tree;				// if not found
		}

//--------HEIGHT, SIZE, CONTAINS AND LOOKUP----------------

		size_t size(node* tree) const {
			if (tree == nullptr) { return 0;}
			else {return size_t(1 + size(tree->left_.get()) + size(tree->right_.get()));}
		}

		size_t height(node* tree) const {
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
		node* rotate_right(node* tree) {
			if (tree == nullptr || tree->left_ == nullptr) {
				return tree;
			}
			// new root is the left subtree
			node* new_root = tree->left_.get();

			// original root left subtree becomes new root's right subtree
			tree->left_.reset(new_root->right_.get());
			// the original root becomes the left subtree of the new root 
			new_root->right_.reset(tree);
			return new_root;
		}

		node* rotate_left(node* tree) {
			// left and right swap from rotate_right
			if (tree == nullptr || tree->right_ == nullptr) {
				return tree;
			}
			node* new_root = tree->right_.get();
			tree->right_.reset(new_root->left_.get());
			new_root->left_.reset(tree);
			return new_root;
		}

		// a tree is oncisdered balanced if  the left and right subtrees differ by less than one
		node* balance(node* tree, size_t index) {
			auto left_size = int(size(tree->left_.get()));

			// repeats until the index and the left subtree are the same height
			// in the case of size / 2, it evens the tree out  

			// comparably an expensive operation, do not want to do that often, once every x inserts 
			/*
				if the mid point of the tree is on the left, it means that the left 
				is "taller" than the right, so rotate right
			*/
			if (index < left_size) {
				tree->left_.reset(balance(tree->left_.get(), index));
				tree = rotate_right(tree);
				return tree;
			}
			// rebalance in favour of the right
			/*
				if the mid point is on the right hand side, then rotate left,
				then index moves one to the lefft 
			*/
			else if(index > left_size){
				tree->right_.reset(balance(tree->right_.get(), index - left_size - 1));
				tree = rotate_left(tree);
				return tree;
			}
			return tree;
		}

//-----------------------------CLEAR AND COPY-------------------------------------

//---------------------------TRAVERSAL-----------------------------------
		void infix(node* tree) const {
			if (tree != nullptr) {
				if (tree->left_ != nullptr) {
					infix(tree->left_.get());
				}
				std::cout << tree->data_ << std::endl;
				if (tree->right_ != nullptr) {
					infix(tree->right_.get());
				}
			}
		}

		void prefix(node* tree) const {
			if (tree != nullptr) {
				std::cout << tree->data_ << std::endl;
				if (tree->left_ != nullptr) {
					prefix(tree->left_.get());
				}
				if (tree->right_ != nullptr) {
					prefix(tree->right_.get());
				}
			}
		}

		void postfix(node* tree) const {
			if (tree != nullptr) {
				if (tree->left_ != nullptr) {
					postfix(tree->left_.get());
				}
				if (tree->right_ != nullptr) {
					postfix(tree->right_.get());
				}
				std::cout << tree->data_ << std::endl;
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
			: root_(std::make_unique<node>(node{ data, nullptr, nullptr })) {
		}

		template<typename InputIt>
		bst(InputIt first, InputIt last)
			// start with an empty tree 
			: bst() {
			auto num = std::distance(first, last);
			for (auto it = first; it != last; ++it) {
				insert(*it);
			}
			//balance(num / 2);
		}
		// INIT LIST CONSTRUCTOR
		bst(std::initializer_list<T> list)
			: bst(list.begin(), list.end()) {
		}

		// I mean theoretically this is all you need right ?
		bst(const bst& other) : inserts_since_balance_(other.inserts_since_balance_) {
			if (other.root_) {
				root_ = copy_subtree(other.root_.get());
			}
		}

		// Copy assignment operator
		bst& operator=(const bst& other) {
			if (this != &other) {  // Self-assignment check
				root_.reset();  // Clear current tree
				inserts_since_balance_ = other.inserts_since_balance_;
				if (other.root_) {
					root_ = copy_subtree(other.root_.get());
				}
			}
			return *this;
		}

		// Keep move semantics too (best of both worlds)
		bst(bst&&) = default;
		bst& operator=(bst&&) = default;

		// INSERTION AND DELETION
		void insert(const T& data) {
			root_.reset(insert(data, root_.get()));
			if (inserts_since_balance_ >= BALANCE_INTERVAL) {
				balance(size() / 2);
				inserts_since_balance_ = 0;
			}
		}

		void erase(const T& data) {
			root_.reset(erase(data, root_.get()));
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
		int size() const {
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
		

		// MIN AND MAX
		T min() const {
			return min(root_.get())->data_;
		}

		T max() const {
			return max(root_.get())->data_;
		}
		// TRAVERSAL
		void prefix_traversal() const {
			prefix(root_.get());
		}
		void infix_traversal() const {
			infix(root_.get());
		}

		void postfix_traversal() const {
			postfix(root_.get());
		}


		// TODO ROTATION AND BALANCING
		void join(bst& other) {
			// the tree calling join is tree1, the tree passed into the function is tree 2
			// interpretable as joining this tree to the other tree
				// ensure that the left tree is less than the right subtree
				// to maintain ordering
			if (is_empty()) {
				root_.reset(other.root_.get());
			}
			else if (other.is_empty()) {
				return;
			}
			else {
				if (max() < other.min()) {
						root_.reset(join(root_.get(), other.root_.get()));
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
			root_.reset(rotate_right(root_.get()));
		}
		void rotate_left() {
			root_.reset(rotate_left(root_.get()));
		}

		void balance(int index) {
			root_.reset(balance(root_.get(), index));
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
}