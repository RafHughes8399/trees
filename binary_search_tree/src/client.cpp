// binary_search_tree.cpp : Defines the entry point for the application.
//

#include "binary_search_tree.h"


int main(){
	auto tree = tree::bst<int>({ 5, 3, 7, 2, 1, 6, 8 });
	std::cout << "FULL TREE: ";
	tree.infix_traversal();
		/** tree looks like
	 * .			5
	 *			/		\
	 *			3		7
	 *		/	 \	  /	  \
	 *		1	 2   6    8
	 */
	tree.erase(3);
	std::cout << " POST ERASE 3: ";
	tree.infix_traversal();
		/** after erasing, tree looks like
	tree.infix_traversal();
	 * .			5
	 *			/		\
	 *			2		8
	 *		/	 \	  /	  \
	 *		1	     6    .
	 */

	tree.erase(7);
	std::cout << "POST ERASE 7: ";
	tree.prefix_traversal();

	/** after erasing, tree looks like
 * .			5
 *			/		\
 *			2		8
 *		/	 \	  /	  \
 *		1	     6    .
 */

	tree.erase(5);

}
