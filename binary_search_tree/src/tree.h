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
			// Implementation needed
			return nullptr;
		}

		// HEIGHT, SIZE, CONTAINS AND LOOKUP
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

		BinarySearchTree(std::initializer_list<T> list)
			: BinarySearchTree(list.begin(), list.end()) {
		}

		//TODO copy and move 
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
		// ACCESSOR

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

		//TODO: find
		node* find(const T& data) const {
			return find(data, root_);
		}
		


		// TRAVERSAL AND TRAITS
		
		// TODO: public prefix
		void prefix_traversal() const {
			std::cout << root_->data_ << std::endl;
			if (root_->left_ != nullptr) {
				root_->left_->prefix_traversal();
			}
			if (root_->right_ != nullptr) {
				root_->right_->prefix_traversal();
			}
		}
		void infix_traversal() const {
			infix(root_);
		}

		//TODO  public postfix
		void postfix_traversal() const {
			if (root_->left_ != nullptr) {
				root_->left_->postfix_traversal();
			}
			if (root_->right_ != nullptr) {
				root_->right_->postfix_traversal();
			}
			std::cout << root_->data_ << std::endl;
		}

		// TODO ROTATION AND BALANCING
		void join(BinarySearchTree& other) {
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