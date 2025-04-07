// binary_search_tree.cpp : Defines the entry point for the application.
//

#include "binary_search_tree.h"


int main(){
	auto another_tree = tree::BinarySearchTree<int>();
	another_tree.insert(2);
	another_tree.insert(1);
	another_tree.insert(0);
	another_tree.insert(3);
	another_tree.insert(4);

	another_tree.infix_traversal();
}
