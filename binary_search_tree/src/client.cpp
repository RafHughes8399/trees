// binary_search_tree.cpp : Defines the entry point for the application.
//

#include "binary_search_tree.h"


int main(){

	auto tree = tree::BinarySearchTree<char>('f');
	tree.insert('a');
	tree.insert('r');

	std::cout << "postfix traversal" << std::endl;
	tree.postfix_traversal();
	std::cout << "prefix traversal" << std::endl;
	tree.prefix_traversal();
	std::cout << "infix traversal" << std::endl;
	tree.infix_traversal();
}
