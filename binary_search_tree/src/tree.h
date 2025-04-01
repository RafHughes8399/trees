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
		BinarySearchTree();

		// tree with root node construction
		BinarySearchTree(const T& root);

		
		template<typename InputIt>
		BinarySearchTree(InputIt first, InputIt last);

		BinarySearchTree(const BinarySearchTree& other);	
		BinarySearchTree(BinarySearchTree&& other) noexcept;

		BinarySearchTree& operator=(const BinarySearchTree& other);	
		BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

		// insert a new node with data
		BinarySearchTree& insert(const T& data);

		// delete a node with data
		BinarySearchTree& erase(const T& data);

		// check if tree contains data
		bool contains(const T& data) const;

		// get tree size
		int size() const;

		// get tree height
		int height() const;

		// join two trees
		void join(BinarySearchTree& other);


		// print tree
		std::ostream& operator<<(std::ostream& os) const;

		void rotate_right(BinarySearchTree& tree);
		void rotate_left(BinarySearchTree& tree);

		void parition(BinarySearchTree& tree, int index);

		void balance(BinarySearchTree& tree, int index);
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
