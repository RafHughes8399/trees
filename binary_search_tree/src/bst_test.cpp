#include "tree.h"
#include "../lib/catch2/catch.hpp"


// ------------------------------------- CONSTRUCTOR AND INITALISATION TESTS ----------------------------------------
TEST_CASE("empty and root tree construction and accessors int") {
	// empty tree
	auto tree = tree::bst<int>();
	CHECK(tree.size() == 0);


	// tree with node
	auto tree_with_node = tree::bst<int>(1);
	CHECK(tree_with_node.size() == 1);
	CHECK(tree_with_node.contains(1) == true);
	
}	
TEST_CASE("empty and root tree construction char") {

	auto tree = tree::bst<char>();
	CHECK(tree.size() == 0);
	
	auto tree_with_node = tree::bst<char>('a');
	CHECK(tree_with_node.size() == 1);
	CHECK(tree_with_node.contains('a') == true);

}

//TODO: copy constructor and operator overload testing
TEST_CASE("copy constructor and operator overload") {
	
	// copy constructor 
	auto tree_1 = tree::bst<int>({ 3, 0, -2, 1, 5, -6 });
	auto tree_2 = tree_1;
	auto equal = tree_2 == tree_1;
	CHECK(equal);

	// operator overload
	auto tree_3 = tree::bst<int>({8, 0, -2, 4, 5, -6, -1, 2});
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
	auto tree_1 = tree::bst<int>(tree_nodes.begin(), tree_nodes.end());
	auto tree_2 = std::move(tree_1);

	// what are we checking - tree 2 has everything, tree 1 has nothing 

	CHECK(tree_1.is_empty());
	CHECK(tree_1.root() == nullptr);

	// check tree_2 contains all the original nodes
	for (auto i = 0; i < tree_nodes.size(); ++i) {
		CHECK(tree_2.contains(tree_nodes.at(i)));
	}
	CHECK(tree_2.size() == tree_nodes.size());

	auto tree_3 = tree::bst<int>({1,2});
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
		auto tree_it = tree::bst<int>(nums.begin(), nums.end());
		// without proper balancing should constantly insert to the right and create a linked list
		auto tree_list = tree::bst<int>({ 1, 2, 3, 4, 5, 6, 7, 8 });

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
	auto tree = tree::bst<int>();
}
// -------------------------------- OPERATOR OVERLOADS ------------------------------------------
TEST_CASE("== operator overload") {
	SECTION("empty trees ") {
		auto empty_tree = tree::bst<int>();
		auto empty_tree_2 = tree::bst<int>();
		bool equal = empty_tree == empty_tree_2;
		CHECK(equal);
	}
	SECTION("non empty and empty trees") {
		auto empty_tree = tree::bst<int>();
		auto non_empty_tree = tree::bst<int>(1);
		bool equal = empty_tree == non_empty_tree;
		CHECK(!equal);
	}
	SECTION("two roots") {
		auto root_1 = tree::bst<int>({ 1 });
		auto root_2 = tree::bst<int>({ 1 });
		bool equal = root_1 == root_2;
		CHECK(equal);
	}
	SECTION("same data, different structure") {
		auto tree_1 = tree::bst<int>({ 2, 1, 3 });
		auto tree_2 = tree::bst<int>({1, 2, 3});
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
		auto tree_1 = tree::bst<int>({ 2, 1, 3 });
		auto tree_2 = tree::bst<int>({ 2, 1, 3 });
		auto equal = tree_1 == tree_2;
		CHECK(equal);

		// then, a more complex tree comparison
		auto tree_3 = tree::bst<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });
		auto tree_4 = tree::bst<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });

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


		auto tree_5 = tree::bst<int>({ 1, 9, -4, 3, 0, 2, -5 });
		auto tree_6 = tree::bst<int>({ 1, 9, -4, 3, 0, 2, -5 });
		equal = tree_5 == tree_6;
		CHECK(equal);


		auto tree_7 = tree::bst<int>({2, 3, -1, 34, -19, 201, 17, -90, 0, -23});
		/**
		 * pending copy constructor implementation
		 * auto tree_8 = tree_7;
		 * equal = tree_7 == tree_8;
		 */
	}
	SECTION("different data, same structure") {
		auto tree_1 = tree::bst<int>({ 2, 0, 3 });
		auto tree_2 = tree::bst<int>({ 2, 1, 3 });
		auto equal = tree_1 == tree_2;
		CHECK(!equal);

		// then, a more complex tree comparison
		auto tree_3 = tree::bst<int>({ -3, 4, 1, -4, 3, -1, -6, -5 });
		auto tree_4 = tree::bst<int>({ -3, 4, 1, -4, 2, -1, -6, -5 });

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
		auto tree_1 = tree::bst<int>({ 3, 2 });
		auto tree_2 = tree::bst<int>(1);

		auto tree_3 = tree::bst<int>({ -2, 1, 2, 0, -6 });
		auto tree_4 = tree::bst<int>({ 9, 1, 3, 10, 12 });

		auto tree_5 = tree::bst<int>({1, 9, 10, 3, 12});
		auto tree_6 = tree::bst<int>(2);

		auto tree_7 = tree::bst<int>({ 2, 3, 4, 9 });
		auto tree_8 = tree::bst<int>({10, 8, 4, 2, -1, 9});

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
	auto tree = tree::bst<int>();
	auto pre_insert = tree.size();
	
	CHECK(pre_insert == 0);
	tree.insert(1);

	auto post_insert = tree.size();
	CHECK(post_insert == 1);

	CHECK(pre_insert != post_insert);
}

TEST_CASE("tree insert char") {
	auto tree = tree::bst<char>();
	tree.insert('a');
	
	CHECK(tree.size() == 1);
	
	tree.insert('b');
	CHECK(tree.size() == 2);


}

TEST_CASE("insertion order int ") {
	auto tree = tree::bst<int>();
	tree.insert(0);
	tree.insert(-1);
	tree.insert(1);

	CHECK(tree.left()->data_ == -1);
	CHECK(tree.right()->data_ == 1);

}

TEST_CASE("insert duplicate") {
	auto tree = tree::bst<int>();
	tree.insert(1);
	tree.insert(2);
	tree.insert(0);
	CHECK(tree.size() == 3);
	tree.insert(1);
	CHECK(tree.size() == 3);

}

TEST_CASE("insertion order char") {
	auto tree = tree::bst<char>();
	tree.insert('b');
	tree.insert('a');
	tree.insert('c');

	CHECK(tree.left()->data_ == 'a');
	CHECK(tree.right()->data_ == 'c');

}

// ----------------------------- ERASE AND CLEAR TESTS -------------------------------------
TEST_CASE("erase nodes") {
	SECTION("erase root") {
		auto tree = tree::bst<int>(4);
		CHECK(tree.size() == 1);
		tree.erase(4);
		CHECK(tree.size() == 0);
		CHECK(tree.root() == nullptr);
	}
	SECTION("erase case - no children ") {
		auto tree = tree::bst<int>({ 7, 2, 9, 1 });
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
}
TEST_CASE("erase - node does not exist") {
	auto tree = tree::bst<int>(6);
	tree.erase(4);


	tree = tree::bst<int>({ 8,-2,1,-3,4, 5, 9, 13 });
	//tree.infix_traversal();
	CHECK(tree.size() == 8);
	tree.erase(10);
	CHECK(tree.size() == 8);
}
TEST_CASE("erase left child") {
	SECTION("simple") {
		auto tree = tree::bst<int>({ 4, 2 });
		/** a simple tree looks like
		 * .			4
		 *			/	  \
		 *			2
		 *
		 * tree height will change
		 */
		CHECK(tree.size() == 2);
		CHECK(tree.height() == 1);

		tree.erase(4);

		CHECK(tree.size() == 1);
		CHECK(tree.height() == 0);
		CHECK(tree.contains(4) == false);

		// two should be the root 
		CHECK(tree.root()->data_ == 2);

	}
	SECTION("more compelx"){
		auto tree = tree::bst<int>({ 6, 4, 2, 1 });
		/** a slightly more complex tree looks like
		 *					6
		 * .			/
		 *				4
		 *			  /
		 *			2
		 *		  /
		 *		1
		 *
		 *
		 */
		CHECK(tree.size() == 4);
		CHECK(tree.height() == 3);

		tree.erase(4);
		CHECK(tree.root()->data_ == 6);
		auto new_left = tree.root()->left_;
		CHECK(new_left->data_ == 2);

		tree.erase(2);
		tree.erase(6);
		CHECK(tree.size() == 1);
		CHECK(tree.height() == 0);
		CHECK(tree.root()->data_ == 1);



		tree = tree::bst<int>({ 8, -1, 2, 10, 4, 3, -3, 0 });
		/** a more complex tree looks like
		 * .			8
		 *			/		\
		 *		  -1		10
		 *		/	\
		 *	  -3	2
		 *		  /	  \
		 *		0		4
		 *			  /
		 *			  3
		 * tree height wont change
		 */

		 // erase with left child
		tree.erase(4);
		// erase with no children
		tree.erase(3);
		// erase with left child
		tree.erase(2);
		// erase with no children
		// erase with left child
		tree.erase(0);
		tree.erase(-1);

		CHECK(tree.root()->left_->data_ == -3);
		CHECK(tree.height() == 1);
		CHECK(tree.size() == 3);
	}
}
TEST_CASE("erase right child") {
	SECTION("simple") {
		auto tree = tree::bst<int>({7, 9});
		CHECK(tree.size() == 2);
		CHECK(tree.height() == 1);

		tree.erase(7);
		
		CHECK(tree.size() == 1);
		CHECK(tree.height() == 0);

		CHECK(tree.root()->data_ == 9);
	}
	SECTION("more complex") {
		auto tree = tree::bst<int>({8, 3, 1, -2, 0, 9, 12, 10 });
		/** tree looks like
		 *					  8
		 *				/			\
		 *			  3				  9
		 *			/					\
		 *		.1						10
		 *		/							\	
		 *	-2								12
		 *	  \ 
		 *		0
		 */

		CHECK(tree.size() == 8);
		CHECK(tree.height() == 4);

		tree.erase(-2);
		tree.erase(9);		
		CHECK(tree.size() == 6);
		CHECK(tree.height() == 3);

		tree.erase(10);
		CHECK(tree.size() == 5);

		CHECK(tree.root()->right_->data_ == 12);
		tree.erase(12);
		CHECK(tree.root()->right_ == nullptr);


		tree = tree::bst<int>({ 5, 8, 10});
		/** tree looks like
		 * .			5
		 *					\
		 *					8
		 *					 \
		 *					10
		 */
		tree.erase(5);
		CHECK(tree.root()->data_ == 8);
		CHECK(tree.height() == 1);
		tree.erase(8);
		CHECK(tree.root()->data_ == 10);
		CHECK(tree.height() == 0);
		tree.erase(10);
		CHECK(tree.is_empty());
	}
}
TEST_CASE("erase case - both children") {
	SECTION("simple, root node"){
		auto tree = tree::bst<int>({2,1,3});

		tree.erase(2);
		CHECK(tree.size() == 2);
		CHECK(tree.height() == 1);
		
		// 3 should be the root 
		CHECK(tree.root()->data_ == 3);
	}

	SECTION("simple, not root") {
		auto tree = tree::bst<int>({5, 3, 7, 2, 1, 6, 8});
		/** tree looks like
		 * .			5
		 *			/		\
		 *			3		7
		 *		/	 \	  /	  \
		 *		1	 2   6    8
		 */

		CHECK(tree.size() == 7);

		tree.erase(3);
		tree.erase(7);

		/** after erasing, tree looks like 
		 * .			5
		 *			/		\
		 *			2		8
		 *		/	 \	  /	  \
		 *		1	     6    .
		 */

		CHECK(tree.size() == 5);
		CHECK(tree.height() == 2);

		tree.erase(5);

		/** tree now looks like 
		 * .		
		 *		  6
		 *		/	\
		 *		2	8
		 *	  /
		 *	1
		 */

		CHECK(tree.height() == 2);
		CHECK(tree.size() == 4);

		// checking the structure of the tree 
		CHECK(tree.root()->data_ == 6);
		CHECK(tree.left()->data_ == 2);
		CHECK(tree.right()->data_ == 8);
	}

	SECTION("complex tree") {
		auto tree = tree::bst<int>({8, 2, 10, 9, 6, 4, -2, 12, 11, -4, -1, 1, 3, 7});
		CHECK(tree.size() == 14);
		CHECK(tree.height() == 4);
		/**	tree looks like 
		 * .							8
		 *					/					\
		 *					2					10
		 *			/			\			/		\
		 *			-2			6		   9		12
		 *		/		\	  /	  \		/	\	  /		
		 *		-4		-1	  4	  7			     11
		 *		  		 \	 /
		 *				  1  3
		 * 
		 * 
		 */

		tree.erase(-2);
		CHECK(tree.size() == 13);
		CHECK(tree.height() == 4);
		/**	tree looks like
		 * .							8
		 *					/					\
		 *					2					10
		 *			/			\			/		\
		 *			-1			6		   9		12
		 *		/		\	  /	  \		/	\	  /
		 *		-4		1	  4	  7			     11
		 *		  		 	 /
		 *				    3
		 *
		 *
		 */

		tree.erase(6);
		CHECK(tree.size() == 12);
		CHECK(tree.height() == 4);

		tree.erase(9);
		CHECK(tree.size() == 11);
		/**
		 * tree looks like 
		 * .							8
		 *					/					\
		 *					2					10
		 *			/			\			/		\
		 *			-1			7					12
		 *		/		\	  /	 				  /
		 *		-4		1	  4	  			     11
		 *		  		 	 /
		 *				    3
		 */
			
		tree.erase(2);
		CHECK(tree.size() == 10);
		CHECK(tree.height() == 3);
		/**
		 * tree looks like
		 * .							8
		 *					/					\
		 *					3					10
		 *			/			\			/		\
		 *			-1			7					12
		 *		/		\	  /	 				  /
		 *		-4		1	  4	  			     11
		 */
		tree.erase(-1);
		tree.erase(10);
		tree.erase(7);

		/**
		 * tree now looks like
		 * .							8
		 *					/					\
		 *					3					12
		 *			/			\					\
		 *			1			4					11
		 *		/			  				  
		 *		-4			  	  			     
		 */
		CHECK(tree.size() == 7);
		CHECK(tree.height() ==  3);

		tree.erase(8);
		tree.erase(3);
		tree.erase(1);
		/**
		 * tree now looks like
		 * .							11
		 *					/					\
		 *					4					12
		 *			/						
		 *			-4								
		 *		
		 */
		CHECK(tree.size() == 4);
		CHECK(tree.height() == 2);

		tree.erase(-4);
		tree.erase(11);
		/**
		 * tree now looks like
		 * .				12
		 *					/			
		 *					4					
		 *
		 */
		CHECK(tree.size() == 2);
		CHECK(tree.height() == 1);
		tree.erase(12);
		tree.erase(4);

		CHECK(tree.is_empty());
	}
}

// -------------------------------- SIZE AND HEIGHT TESTS -----------------------------------
TEST_CASE("size int ") {
	auto tree = tree::bst<int>();
	for (auto i = 0; i < 10; ++i) {
		tree.insert(i);
		CHECK(tree.size() == i + 1);
	}
}

TEST_CASE("size char") {
	auto tree = tree::bst<char>();
	auto size = 1;
	for (auto i = 'a'; i < 'f'; ++i) {
		tree.insert(i);
		CHECK(tree.size() == size);
		size++;
	}
}


TEST_CASE("tree height root") {
	auto tree = tree::bst<int>();
	tree.insert(1);
	CHECK(tree.height() == 0);
}

TEST_CASE("tree height, left and right child ") {
	auto tree = tree::bst<int>();
	tree.insert(1);
	tree.insert(0);
	CHECK(tree.size() == 2);

	CHECK(tree.height() == 1);
	
}

TEST_CASE("tree height, left > right") {
	auto tree = tree::bst<int>();
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



	auto another_tree = tree::bst<int>();
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
	
	auto third_tree = tree::bst<int>();
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
	auto tree = tree::bst<int>();
	tree.insert(0);
	tree.insert(1);
	tree.insert(2);


	CHECK(tree.height() == 2);

	auto another_tree = tree::bst<int>();
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
	auto tree = tree::bst<int>();
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
	auto tree = tree::bst<int>({-2, 4, 1, 2, 8, 9, 10, 7 , -6});

	CHECK(tree.contains(7));
	CHECK(tree.find(4) != nullptr);

	auto root = tree.find(-2);
	CHECK(root->data_ == -2);

}

TEST_CASE("does not contain and cannot find int") {
	auto tree = tree::bst<int>({1,2,3,4, -5, -2, 9, -2});

	CHECK(tree.contains(7) == false);
	CHECK(tree.find(10) == nullptr);
}


//TODO: traversal testing
TEST_CASE("tree traversal int ") {
	auto tree = tree::bst<int>({6,2,9, 3, -2, 11, 1, 5, 8, -6, 12});
}


// -------------------------- MIN, MAX AND JOINING --------------------------------
TEST_CASE("tree min and max") {
	SECTION("int max and min") {
		auto tree = tree::bst<int>({2,3,-1,5, -3, 6, 8, -5, 10});
		auto min = tree.min();
		CHECK(min == -5);
		auto max = tree.max();
		CHECK(max == 10);
	}
}


TEST_CASE("tree joining") {
	SECTION("case max t1 < min t2, and min t2 is root node") {
		auto tree1 = tree::bst<int>({2,3,4,1,0});
		auto tree2 = tree::bst<int>(5);
		
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
		auto tree_1 = tree::bst<int>({3, 0, 4, 2});
		auto tree_2 = tree::bst<int>({7, 5, 9});
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
		auto tree_3 = tree::bst<int>();
		auto tree_4 = tree::bst<int>();
	
	}
	SECTION("join t1 is empty, t2 is not empty") {
		auto tree_1 = tree::bst<int>();
		auto tree_2 = tree::bst<int>({1, 9, 2, -1, -4, 7});
		
		tree_1.join(tree_2);
		CHECK(tree_1.size() == tree_2.size());
	}
	SECTION("join t1 is not empty, t2 is empty") {
		auto tree_1 = tree::bst<int>({ 1, 9, 2, -1, -4, 7 });
		auto tree_2 = tree::bst<int>();

		auto pre_size = tree_1.size();
		tree_1.join(tree_2);

		auto post_size = tree_1.size();
		CHECK(pre_size == post_size);
	}
	SECTION("case max t1 > min t2, should throw exception") {
		// throws exception
		auto tree_1 = tree::bst<int>({ 4,2,1, 6,3 }); // max is 6
		auto tree_2 = tree::bst<int>({ 5, 9, 10, 7}); // min is 7

		CHECK_THROWS(tree_1.join(tree_2));
		
	}
	SECTION("case max t1 > min t2, and max t1 is not root node") {
		auto tree_1 = tree::bst<int>({8, 4, -3, 1, 2, 10, 12});
		auto tree_2 = tree::bst<int>({ -9, -6, -5, -4 });
		
		CHECK_THROWS(tree_1.join(tree_2));
	}

	SECTION("case max t1 == min t2, and min t2 is root node") {
		// max t1 is 5, so min t2 is 5, handling the case of duplicates
		auto tree_1 = tree::bst<int>({3,2, 4, 5});
		auto tree_2 = tree::bst<int>({7, 8, 9, 6, 5, 19});
		
		CHECK_THROWS(tree_1.join(tree_2));

		// and the other way around

		CHECK_THROWS(tree_2.join(tree_2));
	
	}
	SECTION("case t1 and t2 share nodes") {
		// throws an exception because the max < min condition is 
		// no longer satisfied 


		// they share one node
		auto tree_1 = tree::bst<int>({3, 9, 7, 2, -1});
		auto tree_2 = tree::bst<int>{10, 11, 2, 15, 32};

		CHECK_THROWS(tree_1.join(tree_2));
		// they share multiple nodes
		auto tree_3 = tree::bst<int>({2, 9, -1, 0 , 6, 4});
		auto tree_4 = tree::bst<int>({8, 9, 6, 2, 5, 3, -5});
		// the two trees are the same
		CHECK_THROWS(tree_3.join(tree_4));
		CHECK_THROWS(tree_4.join(tree_3));

		auto tree_5 = tree::bst<int>({1,2,3,4,5,6});
		auto tree_6 = tree::bst<int>({1,2,3,4,5,6});
		CHECK_THROWS(tree_5.join(tree_6));
		CHECK_THROWS(tree_6.join(tree_5));

	}
}


// ------------------------- LEFT AND RIGHT ROTATION ------------------------------
TEST_CASE("rotate left empty tree and only root node") {
	SECTION("empty") {
		auto tree = tree::bst<int>();
		CHECK(tree.root() == nullptr);
		CHECK(tree.is_empty());
		tree.rotate_left();
		CHECK(tree.root() == nullptr);
		CHECK(tree.is_empty());
	}
	SECTION("root") {
		auto tree = tree::bst<int>({ 1 });
		CHECK(tree.size() == 1);
		// rotating without a right child does not change the structure of the tree
		tree.rotate_left();
		CHECK(tree.size() == 1);
		CHECK(tree.contains(1));
	}
}
TEST_CASE("rotate left simple") {
	SECTION("right child only") {
		auto tree = tree::bst<int>({1, 2});
		CHECK(tree.size() == 2);
		tree.rotate_left();
		
		CHECK(tree.contains(1));
		CHECK(tree.contains(2));
		CHECK(tree.height() == 1);

		auto rotated_tree = tree::bst<int>({ 2,1 });
		CHECK(tree == rotated_tree);
	}

	SECTION("right and left child") {
		auto tree = tree::bst<int>({2,1,3});
		CHECK(tree.size() == 3);
		CHECK(tree.height() == 1);
		/**		tree looks like 
		 * .		2
		 *		/		\
		 *		1		3
		 */
		tree.rotate_left();
		/**		after rotating left should look like 
		 *				3
		 *			/
		 * .	  2
		 *		/
		 *	   1
		 */
		CHECK(tree.size() == 3);
		CHECK(tree.height() == 2);
		auto rotated_tree = tree::bst<int>({3,2,1});
		CHECK(rotated_tree == tree);
	}
	SECTION("simple tree, unbalanced toward the right, ") {
		auto tree = tree::bst<int>{ 5, 3, 1, 9, 7, 8, 10, 11, 12};
		/**
		 * tree looks like 
		 *				5
		 *			/		\
		 *			3		9
		 *		/		/		\
		 *		1		7		10
		 *					\		\
		 *					8		11
		 *							 \
		 *							 12
		 */
		CHECK(tree.size() == 9);
		CHECK(tree.height() == 4);

		tree.rotate_left();
		/**
		 * 				9
		 *			/		\
		 *			5		10
		 *		/	  \			\
		 *		3		7		11
		 *	/			 \		 \ 
		 *	1				8	  12	
		 */
		CHECK(tree.size() == 9);
		CHECK(tree.height() == 3);

		auto rotated_tree = tree::bst<int>({
				9,5,10,3,7,11,1,8,12
			});
		CHECK(tree == rotated_tree);
	}
}

TEST_CASE("rotate left from linked list ") {
	auto tree = tree::bst<int>();

	// insert 0 to 9
	for (auto i = 0; i < 10; ++i) {
		tree.insert(i);
	}

	auto size = 10;
	// balanced tree 
	CHECK(tree.size() == size);
	CHECK(tree.height() == size - 1);
	tree.rotate_left();
	auto rotated_tree = tree::bst<int>({
		
		1, 0, 2, 3, 4, 5, 6, 7, 8, 9
		
		});
	CHECK(tree == rotated_tree);
	CHECK(tree.height() == 8);
	

	tree.rotate_left();
	rotated_tree = tree::bst<int>({
		2, 1, 0, 3, 4, 5, 6, 7, 8, 9
		});
	CHECK(tree == rotated_tree);
	CHECK(tree.height() == 7);

	tree.rotate_left();
	rotated_tree = tree::bst<int>({3,2,1,0,4,5,6,7,8,9});
	CHECK(tree.height() == 6);
	CHECK(rotated_tree == tree);

	tree.rotate_left();
	rotated_tree = tree::bst<int>({4,3,2,1,0,5,6,7,8,9});
	CHECK(tree.height() == 5);
	CHECK(rotated_tree == tree);


	tree.rotate_left();
	rotated_tree = tree::bst<int>({ 5,4,3,2,1,0, 6, 7, 8, 9 });
	CHECK(tree.height() == 5);
	CHECK(rotated_tree == tree);
}



TEST_CASE("rotate right, empty and root") {
	SECTION("empty") {
		auto tree = tree::bst<int>();
		CHECK(tree.root() == nullptr);
		CHECK(tree.size() == 0);
		tree.rotate_right();
		CHECK(tree.root() == nullptr);
		CHECK(tree.size() == 0);

	}
	SECTION("root") {

		// rotating the root 
		auto tree = tree::bst<int>(2);
		CHECK(tree.size() == 1);
		
		tree.rotate_right();
		auto rotated_tree = tree::bst<int>(2);
		CHECK(tree.size() == 1);
		CHECK(rotated_tree == tree);
	}
}
TEST_CASE("rotate right, simple ") {
	SECTION("rotate with only right child") {
		auto tree = tree::bst<int>({ 2,3 });
		tree.rotate_right();

		// shouldn't do anything because there is no left child
		auto rotated_tree = tree::bst<int>({ 2,3 });
		CHECK(rotated_tree == tree);
	}
	SECTION("rotate with only left child") {
		auto tree = tree::bst<int>({ 3,2 });
		tree.rotate_right();

		auto rotated_tree = tree::bst<int>({ 2, 3 });
		CHECK(rotated_tree == tree);
	}
	SECTION("rotate with both children") {
		auto tree = tree::bst<int>({2, 1, 3});
		
		tree.rotate_right();
		auto rotated_tree = tree::bst<int>({1, 2, 3});
		CHECK(rotated_tree == tree);
	}
}

TEST_CASE("start from linked list") {
	auto tree = tree::bst<int>();
	for (auto i = 9; i >= 0; --i) {
		tree.insert(i);
	}

	tree.rotate_right();
	auto rotated_tree = tree::bst<int>({8, 9, 7, 6, 5, 4, 3, 2, 1, 0});
	CHECK(rotated_tree == tree);

	tree.rotate_right();
	rotated_tree = tree::bst<int>({ 7, 8, 9, 6, 5, 4, 3, 2, 1, 0 });
	CHECK(rotated_tree == tree);

	tree.rotate_right();
	rotated_tree = tree::bst<int>({6, 7, 8, 9, 5, 4, 3, 2, 1, 0});
	CHECK(rotated_tree == tree);

	tree.rotate_right();
	rotated_tree = tree::bst<int>({5, 6, 7, 8, 9, 4, 3, 2, 1, 0});
	CHECK(rotated_tree == tree);
}

TEST_CASE("complex rotation") {
	SECTION("unbalanced toward the left, need to balance by rotating right") {
	
	}
}

TEST_CASE("simple balanceing") {
	// parition at the midpoint,
	// need some way to track the nodes, reduce the time cost for size 
	auto tree = tree::bst<int>({1,0,3,2,4});

	auto size = tree.size();
	tree.balance(size / 2);

	/*
				1
			/		\
			0		3
				/		\
				2		4

	*/
	// take a step back for a second and think, what does balanceing do 
	auto balanceed_tree = tree::bst<int>({2, 1, 3, 0, 4});
	// what should this tree look like, answer that question 
	/*
			2
		/		\
		1		3
		|		|
		0		4
	*/
	tree.prefix_traversal();
	balanceed_tree.prefix_traversal();
	CHECK(balanceed_tree == tree);
}

TEST_CASE("balance after init list construction") {

}

TEST_CASE("balance a balanced tree") {

}

TEST_CASE("non size / 2 balance") {

}

TEST_CASE("balance super large mega tree") {
	SECTION("basically a linked list") {
	
	}
	SECTION("slightly unbalanced") {
	
	}
	SECTION("nearly balanced") {
	
	}

}
TEST_CASE("balance after x inserts ") {

}
TEST_CASE("insert auto balance") {

};
