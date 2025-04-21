#include "tree.h"
#include "../lib/catch2/catch.hpp"


// ------------------------------------- CONSTRUCTOR AND INITALISATION TESTS ----------------------------------------
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
	
	// copy constructor 
	auto tree_1 = tree::BinarySearchTree<int>({ 3, 0, -2, 1, 5, -6 });
	auto tree_2 = tree_1;
	auto equal = tree_2 == tree_1;
	CHECK(equal);

	// operator overload
	auto tree_3 = tree::BinarySearchTree<int>({8, 0, -2, 4, 5, -6, -1, 2});
	tree_1 = tree_3;
	
	auto equal13 = tree_3 == tree_1;
	CHECK(equal13);
	// should no longer hold
	auto equal12 = tree_2 == tree_1;
	CHECK(!equal12);

}
TEST_CASE("move constructor and overload") {

	// move constructor
	auto tree_nodes = std::vector<int>({ 4, -2, -1, 0, 6, 2, 9 });
	auto tree_1 = tree::BinarySearchTree<int>(tree_nodes.begin(), tree_nodes.end());
	auto tree_2 = std::move(tree_1);

	// what are we checking - tree 2 has everything, tree 1 has nothing 

	CHECK(tree_1.is_empty());
	CHECK(tree_1.root() == nullptr);

	// check tree_2 contains all the original nodes
	for (auto i = 0; i < tree_nodes.size(); ++i) {
		CHECK(tree_2.contains(tree_nodes.at(i)));
	}
	CHECK(tree_2.size() == tree_nodes.size());

	auto tree_3 = tree::BinarySearchTree<int>({1,2});
	tree_3 = std::move(tree_2);

	CHECK(tree_2.is_empty());

	for (auto i = 0; i < tree_nodes.size(); ++i) {
		CHECK(tree_3.contains(tree_nodes.at(i)));
	}
	CHECK(tree_3.size() == tree_nodes.size());

}

TEST_CASE("tree insertion init list int") {
	SECTION("basic construction") {
		auto nums = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8 };
		auto tree_it = tree::BinarySearchTree<int>(nums.begin(), nums.end());
		// without proper balancing should constantly insert to the right and create a linked list
		auto tree_list = tree::BinarySearchTree<int>({ 1, 2, 3, 4, 5, 6, 7, 8 });

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
	SECTION("init list has duplicates") {
	
	}
	//TODO insert balancing
	SECTION("checking balancing and rotation ") {

	}

}

//TODO insertion list char
TEST_CASE("tree insertion init list char") {
	auto tree = tree::BinarySearchTree<int>();
}
// -------------------------------- OPERATOR OVERLOADS ------------------------------------------
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

// --------------------------------- INSERT TESTS -------------------------------------------

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

// ----------------------------- ERASE AND CLEAR TESTS -------------------------------------
TEST_CASE("erase nodes") {
	SECTION("erase root") {
		auto tree = tree::BinarySearchTree<int>(4);
		CHECK(tree.size() == 1);
		tree.erase(4);
		CHECK(tree.size() == 0);
		CHECK(tree.root() == nullptr);
	}
	SECTION("erase - node does not exist") {
		auto tree = tree::BinarySearchTree<int>(6);
		tree.erase(4);
		tree.infix_traversal();


		tree = tree::BinarySearchTree<int>({ 8,-2,1,-3,4, 5, 9, 13 });
		tree.infix_traversal();
		CHECK(tree.size() == 8);
		tree.erase(10);
	}
	SECTION("erase case - no children ") {
		auto tree = tree::BinarySearchTree<int>({7, 2, 9, 1});
		CHECK(tree.height() == 2);
		CHECK(tree.size() == 4);
		/** tree looks like
		 * .		7
		 *		/		\
		 *		2		9
		 *	/	 
		 *  1
		 */	

		tree.erase(1);
		/** after erasing 1
		 * .		7
		 *		/		\
		 *		2		9
		 */
		CHECK(tree.height() == 1);
		CHECK(tree.size() == 3);
		CHECK(!tree.contains(1));

		tree.erase(2);
		CHECK(tree.height() == 1);
		CHECK(tree.size() == 2);
		CHECK(!tree.contains(2));

		tree.erase(9);
		/** after erasing 2 and 9
		 * .		7
		 * 
		 */
		CHECK(tree.height() == 0);
		CHECK(tree.size() == 1);
		CHECK(!tree.contains(9));
	}
	SECTION("erase case - left child") {
	
	}
	SECTION("erase case - right child") {
	
	}
	SECTION("erase case - both children") {
	
	}
	SECTION("multiple erases") {
	
	}

	SECTION("erase complex trees") {
	
	}
}
// -------------------------------- SIZE AND HEIGHT TESTS -----------------------------------
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

// ------------------------ CONTAINS, FIND AND TREE TRAVERSAL TESTS ----------------------------------

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


// -------------------------- MIN, MAX AND JOINING --------------------------------
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
	SECTION("join t1 is empty, t2 is not empty") {
		auto tree_1 = tree::BinarySearchTree<int>();
		auto tree_2 = tree::BinarySearchTree<int>({1, 9, 2, -1, -4, 7});
		
		tree_1.join(tree_2);
		CHECK(tree_1.size() == tree_2.size());
	}
	SECTION("join t1 is not empty, t2 is empty") {
		auto tree_1 = tree::BinarySearchTree<int>({ 1, 9, 2, -1, -4, 7 });
		auto tree_2 = tree::BinarySearchTree<int>();

		auto pre_size = tree_1.size();
		tree_1.join(tree_2);

		auto post_size = tree_1.size();
		CHECK(pre_size == post_size);
	}
	SECTION("case max t1 > min t2, should throw exception") {
		// throws exception
		auto tree_1 = tree::BinarySearchTree<int>({ 4,2,1, 6,3 }); // max is 6
		auto tree_2 = tree::BinarySearchTree<int>({ 5, 9, 10, 7}); // min is 7

		CHECK_THROWS(tree_1.join(tree_2));
		
	}
	SECTION("case max t1 > min t2, and max t1 is not root node") {
		auto tree_1 = tree::BinarySearchTree<int>({8, 4, -3, 1, 2, 10, 12});
		auto tree_2 = tree::BinarySearchTree<int>({ -9, -6, -5, -4 });
		
		CHECK_THROWS(tree_1.join(tree_2));
	}

	SECTION("case max t1 == min t2, and min t2 is root node") {
		// max t1 is 5, so min t2 is 5, handling the case of duplicates
		auto tree_1 = tree::BinarySearchTree<int>({3,2, 4, 5});
		auto tree_2 = tree::BinarySearchTree<int>({7, 8, 9, 6, 5, 19});
		
		CHECK_THROWS(tree_1.join(tree_2));

		// and the other way around

		CHECK_THROWS(tree_2.join(tree_2));
	
	}
	SECTION("case t1 and t2 share nodes") {
		// throws an exception because the max < min condition is 
		// no longer satisfied 


		// they share one node
		auto tree_1 = tree::BinarySearchTree<int>({3, 9, 7, 2, -1});
		auto tree_2 = tree::BinarySearchTree<int>{10, 11, 2, 15, 32};

		CHECK_THROWS(tree_1.join(tree_2));
		// they share multiple nodes
		auto tree_3 = tree::BinarySearchTree<int>({2, 9, -1, 0 , 6, 4});
		auto tree_4 = tree::BinarySearchTree<int>({8, 9, 6, 2, 5, 3, -5});
		// the two trees are the same
		CHECK_THROWS(tree_3.join(tree_4));
		CHECK_THROWS(tree_4.join(tree_3));

		auto tree_5 = tree::BinarySearchTree<int>({1,2,3,4,5,6});
		auto tree_6 = tree::BinarySearchTree<int>({1,2,3,4,5,6});
		CHECK_THROWS(tree_5.join(tree_6));
		CHECK_THROWS(tree_6.join(tree_5));

	}
}
