#include "tree.h"
#include "../lib/catch2/catch.hpp"


// do int, char, and custmon type that can be compared
TEST_CASE("tree construction and accessors int ") {
	// insert root leaf node
	auto root = tree::BinarySearchTree<int>(1);
	CHECK(root.data() == 1);

	// check no children
	CHECK(root.left() == nullptr);
	CHECK(root.right() == nullptr);
	
}	

TEST_CASE("tree insertion int") {
	auto root = tree::BinarySearchTree<int>(1);
	CHECK(root.data() == 1);

	// insert < 
	root.insert(0);
	CHECK(root.left() != nullptr);
	CHECK(root.left()->data() == 0);

	CHECK(root.size() == 2);

	// insert >
	root.insert(2);
	CHECK(root.right() != nullptr);
	CHECK(root.right()->data() == 2);
	
	CHECK(root.size() == 3);

}

TEST_CASE("tree insert multiple levels int") {
	//auto root = tree::BinarySearchTree<int>();
}

TEST_CASE("tree insertion char") {
	auto root = tree::BinarySearchTree<char>('f');
	CHECK(root.data() == 'f');

	root.insert('a');
	root.insert('r');

	CHECK(root.size() == 3);
	CHECK(root.left()->data() == 'a');
	CHECK(root.right()->data() == 'r');
}
