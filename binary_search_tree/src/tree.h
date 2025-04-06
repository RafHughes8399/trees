#pragma once

#include <memory>
#include <algorithm>
#include <iostream>

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
		node* erase(const T& data, node& tree) {
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
			return nullptr;
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
		BinarySearchTree(InputIt first, InputIt last) {
		
		}

		BinarySearchTree(const BinarySearchTree& other)
			: root_(other.root_) {
		}

		void insert(const T& data) {
			root_ = insert(data, root_);
		}

		int size() const {
			return size(root_);
		}

		bool contains(const T& data) const {
			return contains(data, root_);
		}
		BinarySearchTree(BinarySearchTree&& other) noexcept {
			
		}

		BinarySearchTree& operator=(const BinarySearchTree& other) {
			return *this;
		}
		BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
			return *this;
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

		// TRAVERSAL AND TRAITS

		// get tree height
		int height() const {
			return 1;
		}

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
			if (root_->left_ != nullptr) {
				root_->left_->infix_traversal();
			}
			std::cout << root_->data_ << std::endl;
			if (root_->right_ != nullptr) {
				root_->right_->infix_traversal();
			}
		}
		void postfix_traversal() const {
			if (root_->left_ != nullptr) {
				root_->left_->postfix_traversal();
			}
			if (root_->right_ != nullptr) {
				root_->right_->postfix_traversal();
			}
			std::cout << root_->data_ << std::endl;
		}

		// ROTATION AND BALANCING
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

		void balance(BinarySearchTree& tree, int index) {
			
		}

		// OPERATOR OVERLOADS
		friend std::ostream& operator<<(std::ostream& os, const BinarySearchTree& tree) {
			os << tree.root_->data_;
			return os;
		}

		bool operator==(const BinarySearchTree& other) const {
			return false;
		}

		class AVLTree {

		};
	};
}