// binary_search_tree.cpp : Defines the entry point for the application.
//

#include "binary_search_tree.h"


int main(){
	auto tree = tree::bst<int>({ 8, 2, 10, 9, 6, 4, -2, 12, 11, -4, -1, 1, 3, 7 });
	std::cout << "prior to erase " << std::endl;
	tree.infix_traversal();
	tree.erase(-2);
	std::cout << "post erase" << std::endl;
	tree.infix_traversal();

	if (tree.size() == 13) {
		std::cout << "checked size" << std::endl;

	}
	if (tree.height() == 4) {
		std::cout << "checked height" << std::endl;
	
	}
	// use this area to tinker with balance interval numbers

}
