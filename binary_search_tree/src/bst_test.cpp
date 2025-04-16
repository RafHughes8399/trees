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

//TODO: copy constructor and operator overload testing
TEST_CASE("copy constructor and operator overload") {

}
TEST_CASE("move constructor and overload") {

}

//TODO: ==operator overload
TEST_CASE("== operator overload") {
	SECTION("empty trees ") {
		auto empty_tree = tree::BinarySearchTree<int>();
		auto empty_tree_2 = tree::BinarySearchTree<int>();
		bool equal = empty_tree == empty_tree_2;
		CHECK(equal);
	}
	SECTION("non empty and empty trees") {
		auto empty_tree = tree::BinarySearchTree<int>();
		auto non_empty_tree = tree::BinarySearchTree<int>(1);
		bool equal = empty_tree == non_empty_tree;
		CHECK(!equal);
	}
	SECTION("two roots") {
		auto root_1 = tree::BinarySearchTree<int>({ 1 });
		auto root_2 = tree::BinarySearchTree<int>({ 1 });
		bool equal = root_1 == root_2;
		CHECK(equal);
	}
	SECTION("same data, different structure") {
		auto tree_1 = tree::BinarySearchTree<int>({ 2, 1, 3 });
		auto tree_2 = tree::BinarySearchTree<int>({1, 2, 3});
		/**
		 * tree 1 looks like 
		 * . 2
		 * /   \
		 * 1	3
		 * 
		 * tree 2 looks like
		 *   1
		 *	  \
		 *	   2
		 *		\
		 *		 3
		 *  same data, but different structure so not equal
		 */
		auto equal = tree_1 == tree_2;
		CHECK(!equal);
	}
	SECTION("same data, same structure,") {
		// first a simple tree comparison
		auto tree_1 = tree::BinarySearchTree<int>({ 2, 1, 3 });
		auto tree_2 = tree::BinarySearchTree<int>({ 2, 1, 3 });
		auto equal = tree_1 == tree_2;
		CHECK(equal);

		// then, a more complex tree comparison
		auto tree_3 = tree::BinarySearchTree<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });
		auto tree_4 = tree::BinarySearchTree<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });

		/**
		 * both trees look like
		 *				-3
		 *			/		\
		 *		  -4		 4
		 *		/		  /
		 *	  -6		1
		 *  .	\	  /   \
		 *		-5	 -1	   2
		 */

		equal = tree_3 == tree_4;
		CHECK(equal);


		auto tree_5 = tree::BinarySearchTree<int>({ 1, 9, -4, 3, 0, 2, -5 });
		auto tree_6 = tree::BinarySearchTree<int>({ 1, 9, -4, 3, 0, 2, -5 });
		equal = tree_5 == tree_6;
		CHECK(equal);


		auto tree_7 = tree::BinarySearchTree<int>({2, 3, -1, 34, -19, 201, 17, -90, 0, -23});
		/**
		 * pending copy constructor implementation
		 * auto tree_8 = tree_7;
		 * equal = tree_7 == tree_8;
		 */
	}
	SECTION("different data, same structure") {
		auto tree_1 = tree::BinarySearchTree<int>({ 2, 0, 3 });
		auto tree_2 = tree::BinarySearchTree<int>({ 2, 1, 3 });
		auto equal = tree_1 == tree_2;
		CHECK(!equal);

		// then, a more complex tree comparison
		auto tree_3 = tree::BinarySearchTree<int>({ -3, 4, 1, -4, 3, -1, -6, -5 });
		auto tree_4 = tree::BinarySearchTree<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });

		/**
		 * both trees look like
		 *				-3
		 *			/		\
		 *		  -4		 4
		 *		/		  /
		 *	  -6		1
		 *  .	\	  /   \
		 *		-5	 -1	   (3 or 2)
		 */

		equal = tree_3 == tree_4;
		CHECK(!equal);
	}
	SECTION("different data, different structure") {
		auto tree_1 = tree::BinarySearchTree<int>({ 3, 2 });
		auto tree_2 = tree::BinarySearchTree<int>(1);

		auto tree_3 = tree::BinarySearchTree<int>({ -2, 1, 2, 0, -6 });
		auto tree_4 = tree::BinarySearchTree<int>({ 9, 1, 3, 10, 12 });

		auto tree_5 = tree::BinarySearchTree<int>({1, 9, 10, 3, 12});
		auto tree_6 = tree::BinarySearchTree<int>(2);

		auto tree_7 = tree::BinarySearchTree<int>({ 2, 3, 4, 9 });
		auto tree_8 = tree::BinarySearchTree<int>({10, 8, 4, 2, -1, 9});

		auto equal = tree_1 == tree_2;
		CHECK(!equal);

		equal = tree_2 == tree_3;
		CHECK(!equal);

		equal = tree_3 == tree_4;
		CHECK(!equal);

		equal = tree_5 == tree_6;
		CHECK(!equal);
		
		equal = tree_2 == tree_6;
		CHECK(!equal);

		equal = tree_7 == tree_8;
		CHECK(!equal);

		equal = tree_4 == tree_8;
		CHECK(!equal);
	}
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
	//TODO insert balancing
	SECTION("checking balancing and rotation ") {

	}
}

//TODO insertion list char
TEST_CASE("tree insertion init list char") {
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


//TODO: traversal testing
TEST_CASE("tree traversal int ") {
	auto tree = tree::BinarySearchTree<int>({6,2,9, 3, -2, 11, 1, 5, 8, -6, 12});
}


TEST_CASE("tree min and max") {
	SECTION("int max and min") {
		auto tree = tree::BinarySearchTree<int>({2,3,-1,5, -3, 6, 8, -5, 10});
		auto min = tree.min();
		CHECK(min == -5);
		auto max = tree.max();
		CHECK(max == 10);
	}
}


TEST_CASE("tree joining") {
	SECTION("case max t1 < min t2, and min t2 is root node") {
		auto tree1 = tree::BinarySearchTree<int>({2,3,4,1,0});
		auto tree2 = tree::BinarySearchTree<int>(5);
		
		/** tree 1 looks like
		 *			2
		 * .	/		\
		 *		1		3
		 *	/				\
		 *	0				4
		 * 
		 * tree 2 looks like 
		 *			5
		 * 
		 * after joining it should look like
		 *					5
		 *				/		\
		 *				2
		 * .		/		\
		 *			1		3
		 *		/				\
		 *		0				4
		 * 
		 */
		auto pre_join_size = tree1.size();
		
		tree1.join(tree2);

		// check size increase
		CHECK(tree1.size() != pre_join_size);
		CHECK(tree1.size() == 6);

		// check tree 1 contains the new nodes
		CHECK(tree1.contains(5));

		// check that min tree 2 is the new root of tree 1
		CHECK(tree1.root()->data_ == 5);
		CHECK(tree1.root()->left_->data_ == 2);
		CHECK(tree1.root()->right_ == nullptr);


	}
	//TODO: join test cases
	SECTION("case max t1 < min t2, and min t2 is not root node") {
		// simple trees
		auto tree_1 = tree::BinarySearchTree<int>({3, 0, 4, 2});
		auto tree_2 = tree::BinarySearchTree<int>({7, 5, 9});
		auto size_1 = tree_1.size();
		auto size_2 = tree_2.size();
		auto join_size = size_1 + size_2;
		/** tree 1 looks like
		 *			3	
		 *		/		\
		 *		0		4
		 * .	 \
		 *			2 
		 *  
		 *  
		 * tree 2 looks liek
		 *		7
		 *	/	 \
		 * 5	  9
		 * 
		 *  after joining
		 *			5
		 *		/		\
		 *		3		 7
		 *	/		\	 \ 
		 *	0		4	  9
		 * .  \
		 *		2 
		 *	
		 */

		tree_1.join(tree_2);
		CHECK(tree_1.size() == join_size);
		// more complex trees
		auto tree_3 = tree::BinarySearchTree<int>();
		auto tree_4 = tree::BinarySearchTree<int>();
	
	}

	SECTION("case max t1 > min t2, should throw exception") {
		// throws exception
		auto tree_1 = tree::BinarySearchTree<int>({ 4,2,1, 6,3 }); // max is 6
		auto tree_2 = tree::BinarySearchTree<int>({ 5, 9, 10, 7}); // min is 7

		CHECK_THROWS(tree_1.join(tree_2));
	}
	SECTION("case max t2 < min t1, and min t1 is not root node") {
		// throws exception
	}

	SECTION("case max t1 == min t2, and min t2 is root node") {
	
	}
	SECTION("case max t1 == min t2, and min t2 is not root node") {
	
	}
}
