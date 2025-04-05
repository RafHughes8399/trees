#pragma once

#include <memory>
#include <algorithm>
#include <iostream>
namespace tree {
	template <typename T>
	class BinarySearchTree {

	public:
		~BinarySearchTree() = default;
		
		// empty tree construction
		// tree with root node construction, has data and no children
		BinarySearchTree(const T& root)
			: data_{ root }, left_{ nullptr }, right_{ nullptr } {
		}

		template<typename InputIt>
		BinarySearchTree(InputIt first, InputIt last)
			: BinarySearchTree() {
			// insert all nodes
			for (auto it = first; it != last; ++it) {
				insert(*it);
			}
			auto n = std::distance(first, last);
			// reblaance the tree around the middle index
			balance(*this, n / 2);
		}

		BinarySearchTree(const BinarySearchTree& other)
			: data_{ other.data_ }, left_{ nullptr }, right_{ nullptr } {
			// Recursively copy left subtree if it exists
			if (other.left_) {
				left_ = std::make_unique<BinarySearchTree>(*other.left_);
			}
			// Recursively copy right subtree if it exists
			if (other.right_) {
				right_ = std::make_unique<BinarySearchTree>(*other.right_);
			}
		}


		BinarySearchTree(BinarySearchTree&& other) noexcept;

		BinarySearchTree& operator=(const BinarySearchTree& other);	
		BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

		// ACCESSORS
		T data() const{
			return data_;
		}

		BinarySearchTree* left() const {
			return left_.get();
		}
		BinarySearchTree* right() const {
		
			return right_.get();
		}
		// INSERTION AND DELETION
		// insert a new node with data, for now without tree balancing
		BinarySearchTree& insert(const T& data) {
			if (data_ == T()) {
				// if empty tree, set root
				std::cout << "fill root " << std::endl;
				data_ = data;
			}
			if (data < data_) {
				// go left, insert if leaf
				if (left_ == nullptr) {
					left_ = std::make_unique<BinarySearchTree<T>>(data);
				}
				// otherwise keep going
				else {
					left_->insert(data);
				}
			}
			else if (data > data_) {
				// go right
				if (right_ == nullptr) {
					right_ = std::make_unique<BinarySearchTree<T>>(data);
				}
				else {
					right_->insert(data);
				}
			}
			else {
				return *this;
			}
		}

		// delete a node with data
		BinarySearchTree& erase(const T& data);
		bool contains(const T& data) const;


		// TRAVERSAL AND TRAITS

		size_t size() const {
			size_t size = 1;
			// size = 1;
			if (left_ != nullptr) {
				size += left_->size();
			}
			if (right_ != nullptr) {
				size += right_->size();
			}
			return size;
		}
		// get tree height
		int height() const {
			// something like std max left.size(), right.size()
		}

		void prefix_traversal() const {
			std::cout << data_ << std::endl;
			if (left_ != nullptr) {
				left_->prefix_traversal();
			}
			if (right_ != nullptr) {
				right_->prefix_traversal();
			}
		}
		void infix_traversal() const {
			if (left_ != nullptr) {
				left_->infix_traversal();
			}
			std::cout << data_ << std::endl;
			if (right_ != nullptr) {
				right_->infix_traversal();
			}
		}
		void postfix_traversal() const {
			if (left_ != nullptr) {
				left_->postfix_traversal();
			}
			if (right_ != nullptr) {
				right_->postfix_traversal();
			}
			std::cout << data_ << std::endl;
		}


		// ROTATION AND BALANCING
		void join(BinarySearchTree& other);
		void rotate_right(BinarySearchTree& tree);
		void rotate_left(BinarySearchTree& tree);

		void parition(BinarySearchTree& tree, int index);

		void balance(BinarySearchTree& tree, int index);
		
		
		// OPERATOR OVERLOADS
		std::ostream& operator<<(std::ostream& os) const {
			os << data_;
			return os;
		}

		bool operator==(const BinarySearchTree & other) const {
			return false;
		}
	private:
		// has data
		T data_;
		std::unique_ptr<BinarySearchTree> left_; // initially a leaf node
		std::unique_ptr<BinarySearchTree> right_; // initially a leaf node
	};

	template <typename T>
	class AVLTree {

	};
}
