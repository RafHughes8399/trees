#include "tree.h"
#include "../lib/catch2/catch.hpp"


// do int, char, and custmon type that can be compared
TEST_CASE("empty and root tree construction and accessors int") {
	// empty tree
	auto tree = tree::BinarySearchTree<int>();
	CHECK(tree.size() == 0);


	// tree with node
	auto tree_with_node = tree::BinarySearchTree<int>(1);
	CHECK(tree_with_node.size() == 1);
	CHECK(tree_with_node.contains(1) == true);
	
}	
TEST_CASE("empty and root tree construction char") {

	auto tree = tree::BinarySearchTree<char>();
	CHECK(tree.size() == 0);
	
	auto tree_with_node = tree::BinarySearchTree<char>('a');
	CHECK(tree_with_node.size() == 1);
	CHECK(tree_with_node.contains('a') == true);

}

TEST_CASE("tree insertion input iterator int") {

}

TEST_CASE("tree insertion input iteartor char") {

}


TEST_CASE("tree insert int") {
	auto tree = tree::BinarySearchTree<int>();
	auto pre_insert = tree.size();
	
	CHECK(pre_insert == 0);
	tree.insert(1);

	auto post_insert = tree.size();
	CHECK(post_insert == 1);

	CHECK(pre_insert != post_insert);
}

TEST_CASE("tree insert char") {
	auto tree = tree::BinarySearchTree<char>();
	tree.insert('a');
	
	CHECK(tree.size() == 1);
	
	tree.insert('b');
	CHECK(tree.size() == 2);


}

TEST_CASE("insertion order int ") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(0);
	tree.insert(-1);
	tree.insert(1);

	CHECK(tree.left()->data_ == -1);
	CHECK(tree.right()->data_ == 1);

}

TEST_CASE("insertion order char") {
	auto tree = tree::BinarySearchTree<char>();
	tree.insert('b');
	tree.insert('a');
	tree.insert('c');

	CHECK(tree.left()->data_ == 'a');
	CHECK(tree.right()->data_ == 'c');

}

TEST_CASE("size int ") {

}

TEST_CASE("size char") {

}

TEST_CASE("tree height empty tree") {
}

TEST_CASE("tree height root node") {

}

TEST_CASE("tree height, left and right same ") {

}
TEST_CASE("tree height, left > right") {

}
TEST_CASE("tree height, right > left") {

}
