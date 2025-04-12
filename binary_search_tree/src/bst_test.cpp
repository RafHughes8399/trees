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

TEST_CASE("copy constructor and operator overload") {

}

TEST_CASE("tree insertion init list int") {
	SECTION("basic construction") {
		auto nums = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8};
		auto tree_it = tree::BinarySearchTree<int>(nums.begin(), nums.end());
		// without proper balancing should constantly insert to the right and create a linked list
		auto tree_list = tree::BinarySearchTree<int>({1, 2, 3, 4, 5, 6, 7, 8});
		
		CHECK(tree_it.size() == tree_list.size());

		for (auto& n : nums) {
			CHECK(tree_it.contains(n));
			CHECK(tree_list.contains(n));

			CHECK(tree_it.find(n) != nullptr);
			CHECK(tree_list.find(n) != nullptr);
		}

		// presently (as of 12/04), tree balancing has not been implemented so, the two trees should
		CHECK(tree_it.size() == 8);
		CHECK(tree_list.size() == 8);

		CHECK(tree_it.height() == nums.size() - 1);
		CHECK(tree_list.height() == nums.size() - 1);
	}
	SECTION("checking balancing and rotation ") {

	}
}

TEST_CASE("tree insertion init list  char") {
	auto tree = tree::BinarySearchTree<int>();
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

TEST_CASE("insert duplicate") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(1);
	tree.insert(2);
	tree.insert(0);
	CHECK(tree.size() == 3);
	tree.insert(1);
	CHECK(tree.size() == 3);

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
	auto tree = tree::BinarySearchTree<int>();
	for (auto i = 0; i < 10; ++i) {
		tree.insert(i);
		CHECK(tree.size() == i + 1);
	}
}

TEST_CASE("size char") {
	auto tree = tree::BinarySearchTree<char>();
	auto size = 1;
	for (auto i = 'a'; i < 'f'; ++i) {
		tree.insert(i);
		CHECK(tree.size() == size);
		size++;
	}
}


TEST_CASE("tree height root") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(1);
	CHECK(tree.height() == 0);
}

TEST_CASE("tree height, left and right child ") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(1);
	tree.insert(0);
	CHECK(tree.size() == 2);

	CHECK(tree.height() == 1);
	
}

TEST_CASE("tree height, left > right") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(2);
	tree.insert(0);
	tree.insert(-1);
	tree.insert(1);

	/* tree looks like
			2
		/		\
		0		
	/	 \
  -1	  1
	*/
	CHECK(tree.height() == 2);



	auto another_tree = tree::BinarySearchTree<int>();
	another_tree.insert(2);
	another_tree.insert(3);
	another_tree.insert(1);
	another_tree.insert(0);


	/* tree looks like
			2
		/		\
		1		3
	/	 \
	0
	*/
	CHECK(another_tree.height() == 2);
	
	auto third_tree = tree::BinarySearchTree<int>();
	third_tree.insert(3);
	third_tree.insert(2);
	third_tree.insert(1);
	third_tree.insert(0);
	third_tree.insert(4);
	third_tree.insert(5);
	third_tree.insert(-2);
	third_tree.insert(-1);



	/* tree looks like
				3
			/		\
			2		4
		/	 \		  \
		1	 			5
	  /
	  0
	/	
	-2
		\
		-1
	*/
	CHECK(third_tree.height() == 5);

}

TEST_CASE("tree height, right > left") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(0);
	tree.insert(1);
	tree.insert(2);


	CHECK(tree.height() == 2);

	auto another_tree = tree::BinarySearchTree<int>();
	another_tree.insert(1);
	another_tree.insert(0);
	another_tree.insert(3);
	another_tree.insert(2);
	another_tree.insert(4);

	/** tree looks like
	 *				1
	 *			/	  \
	 *			0		3
	 *				  /	  \
	 *				2     4
	 */
	CHECK(another_tree.height() == 2);
	CHECK(another_tree.size() == 5);
}

TEST_CASE("more intricate insertion, checking height and size") {
	auto tree = tree::BinarySearchTree<int>();
	tree.insert(1);
	tree.insert(-1);
	tree.insert(2);
	tree.insert(0);
	tree.insert(3);
	tree.insert(-3);

	/**
	 * tree looks like
	 *			1
	 * .	-1		  2
	 *	  -3   0	     3 
	 */
	CHECK(tree.size() == 6);
	CHECK(tree.height() == 2);

}


TEST_CASE("contains and find int") {
	auto tree = tree::BinarySearchTree<int>({-2, 4, 1, 2, 8, 9, 10, 7 , -6});

	CHECK(tree.contains(7));
	CHECK(tree.find(4) != nullptr);

	auto root = tree.find(-2);
	CHECK(root->data_ == -2);

}

TEST_CASE("does not contain and cannot find int") {
	auto tree = tree::BinarySearchTree<int>({1,2,3,4, -5, -2, 9, -2});

	CHECK(tree.contains(7) == false);
	CHECK(tree.find(10) == nullptr);
}


TEST_CASE("tree traversal int ") {
	auto tree = tree::BinarySearchTree<int>({6,2,9, 3, -2, 11, 1, 5, 8, -6, 12});
}
