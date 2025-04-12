#pragma once

#include <memory>
#include <algorithm>
#include <iostream>



// the private methods take a node* tree, the public methods 
// call the private method and pass in root_
namespace tree {
	template <typename T>
	class BinarySearchTree {
		struct node {
			T data_;
			node* left_;
			node* right_;
		};

	private:

		node* root_;


		// JOIN, ROTATION AND BALANCING 
		
		node* join(node* tree1, node* tree2) {
			// assumes that tree1 max is less than tree2 min 

			//pseudocode 
			/**
			 *  if t1 empty return t2
			 * if t2 empty return t1
			 * 
			 * otherwise
			 *	current = t2
			 *	parent = null
			 * while current->left is not null
			 *	parent = current
			 *	current = current->left
			 * 
			 * 
			 * this finds the minimum of t2
			 * if parent not null
			 *	parent->left = current->right
			 * current->right = t2
			 * 
			 * 
			 * attaches t1 to the left of the minimum node in t2
			 * current->left = t1 
			 * return current
			 * .
			 */
			


			return tree1; // placeholder return;
		}
		// INSERTION AND DELETION
		node* insert(const T& data, node* tree) {
			if (tree == nullptr) {
				tree = new node();
				tree->data_ = data;
				tree->left_ = nullptr;
				tree->right_ = nullptr;
			}
			else {
				if (data < tree->data_) {
					tree->left_ = insert(data, tree->left_);
				}
				else if (data > tree->data_) {
					// go right
					tree->right_ = insert(data, tree->right_);
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
			else {
				// traverse the tree to find the node
				if (data < tree->data_) {
					tree->left_ = erase(data, tree->left_);
				}
				else if (data > tree->data_) {
					tree->right_ = erase(data, tree->right_);
				}
				else {
					// case 1 : no children, just remove the root / leaf 
					auto new_tree = tree;
					if (tree->left_ == nullptr and tree->right_ == nullptr) {
						new_tree = nullptr;
					}
					// case 2 : only right child, repositioning the right child
					else if (tree->left_ == nullptr) {
						new_tree = tree->right_;
					}
					// case 3 : only left child, repositioning the left child
					else if (tree->right_ == nullptr) {
						
					}
					// case 4 : both children, joining their subtrees
					else {
						new_tree = join(tree->left_, tree->right_);
					}
					tree = new_tree;
					return tree;
				}
			}
		}

//--------HEIGHT, SIZE, MIN AND MAX, CONTAINS AND LOOKUP----------------


		node* min(node* tree) const {
			 // go as far left as possible
			if (tree == nullptr) { return nullptr; }
			auto current = tree;
			auto parent = nullptr;
			while (current->left_ != nullptr) {
				parent = current;
				current = current->left_;
			}
			return current;
		}
		node* max(node* tree) const {
			// same logic as left, just with the right subtree instead
			if (tree == nullptr) { return nullptr; }
			auto current = tree;
			auto parent = nullptr;
			while (current->right_ != nullptr) {
				parent = current;
				current = current->right_;
			}
			return current;
		}
		size_t size(node* tree) const {
			if (tree == nullptr) {
				return 0;
			}
			else {
				return size_t(1 + size(tree->left_) + size(tree->right_));
			}
		}

		size_t height(node* tree) const {
			if (tree == nullptr) {
				return -1;
			}
			else {
				int left_height = height(tree->left_);
				int right_height = height(tree->right_);
				if (left_height > right_height) {					
					return 1 + left_height;
				}
				else {
					return 1 + right_height;
				}
			}
		}

		bool contains(const T& data, node* tree) const {
			if (tree == nullptr) {
				return false;
			}
			else {
				if (data < tree->data_) {
					return contains(data, tree->left_);
				}
				else if (data > tree->data_) {
					return contains(data, tree->right_);
				}
				else {
					return true;
				}
			}
		}

		node* find(const T& data, node* tree) const {
			if (tree == nullptr) {
				return nullptr;
			}
			else {
				if (data < tree->data_) {
					return find(data, tree->left_);
				}
				else if (data > tree->data_) {
					return find(data, tree->right_	);
				}
				else {
					return tree;
				}
			}
		}



		// BALANCING, ROTATING AND PARTITIONING
		node* balance(node* tree, int index) {
			return nullptr;
		}

		// TRAVERSAL
		void infix(node* tree) const {
			if (tree != nullptr) {
				if (tree->left_ != nullptr) {
					infix(tree->left_);
				}
				std::cout << tree->data_ << std::endl;
				if (tree->right_ != nullptr) {
					infix(tree->right_);
				}
			}
		}

		void prefix(node* tree) const {
			if (tree != nullptr) {
				std::cout << tree->data_ << std::endl;
				if (tree->left_ != nullptr) {
					prefix(tree->left_);
				}
				if (tree->right_ != nullptr) {
					prefix(tree->right_);
				}
			}
		}

		void postfix(node* tree) const {
			if (tree != nullptr) {
				if (tree->left_ != nullptr) {
					postfix(tree->left_);
				}
				if (tree->right_ != nullptr) {
					postfix(tree->right_);
				}
				std::cout << tree->data_ << std::endl;
			}

		}
	public:
		~BinarySearchTree() = default;

		// empty tree construction
		// tree with root node construction, has data and no children

		BinarySearchTree()
			: root_{ nullptr } {

		}
		BinarySearchTree(const T& data) {
			root_ = new node();
			root_->data_ = data;
			root_->left_ = nullptr;
			root_->right_ = nullptr;
		}

		template<typename InputIt>
		BinarySearchTree(InputIt first, InputIt last)
			// start with an empty tree 
			: BinarySearchTree() {
			auto num = std::distance(first, last);
			for (auto it = first; it != last; ++it) {
				insert(*it);
			}

			// move through the rest of the list and insert
			// then balance
			//balance(num / 2);

			}
		// INIT LIST CONSTRUCTOR
		BinarySearchTree(std::initializer_list<T> list)
			: BinarySearchTree(list.begin(), list.end()) {
		}

		// COPY AND MOVE CONSTRUCTORS AND OVERLOADS
		BinarySearchTree(const BinarySearchTree& other)
			: root_(other.root_) {
		}
		BinarySearchTree(BinarySearchTree&& other) noexcept {
			
		}

		BinarySearchTree& operator=(const BinarySearchTree& other) {
			return *this;
		}
		BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
			return *this;
		}


		// INSERTION AND DELETION
		void insert(const T& data) {
			root_ = insert(data, root_);
		}

		void erase(const T& data) {
			root_ = erase(data, root_);
		}

		// ACCESSORS
		node* root() const {
			return root_;
		}
		node* left() const {
			return root_->left_;
		}
		node* right() const {
			return root_->right_;
		}


		// SIZE AND HEIGHT
		int size() const {
			return size(root_);
		}
		int height() const {
			return height(root_);
		}

		// CONTAINS AND FIND
		bool contains(const T& data) const {
			return contains(data, root_);
		}

		node* find(const T& data) const {
			return find(data, root_);
		}
		
		// TRAVERSAL AND TRAITS
		void prefix_traversal() const {
			prefix(root_);
		}
		void infix_traversal() const {
			infix(root_);
		}

		void postfix_traversal() const {
			postfix(root_);
		}


		// TODO ROTATION AND BALANCING
		void join(BinarySearchTree& other) {
			// takes two trees and joins them together, requires the max of one tree to be
			// less than  the min of the other
			// there are the following cases
			// 1. tree1 max is less than tree2 min
			// 2. or tree2 max is less than tree2 min 
			// 3. or tree1 max = tree2 min
			// requires tree1 max to be less than tree2 min

			// case 1 and 3 can be merged, just need to avoid the duplicate node
			// case 2 is separate
			return;
		}
		void rotate_right(BinarySearchTree& tree) {
			return;
		}
		void rotate_left(BinarySearchTree& tree) {
			return;
		}

		void parition(BinarySearchTree& tree, int index) {
			return;
		}

		void balance(int index) {
			root_ = balance(root_, index);
		}
		

		// OPERATOR OVERLOADS
		friend std::ostream& operator<<(std::ostream& os, const BinarySearchTree& tree) {
			os << tree.root_->data_;
			return os;
		}

		bool operator==(const BinarySearchTree& other) const {
			return false;
		}
	};
}