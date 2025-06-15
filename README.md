# tree::bst

A self-balancing binary search tree implementation in C++ using smart pointers for automatic memory management.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Template Parameters](#template-parameters)
  - [Member Types](#member-types)
  - [Constructors](#constructors)
  - [Modifiers](#modifiers)
  - [Lookup Operations](#lookup-operations)
  - [Capacity](#capacity)
  - [Element Access](#element-access)
  - [Traversal](#traversal)
  - [Tree Operations](#tree-operations)
  - [Comparison](#comparison)
- [Examples](#examples)
- [Complexity](#complexity)
- [Requirements](#requirements)



## Overview

`tree::bst` is a template-based binary search tree that automatically maintains balance through periodic rebalancing. It uses `std::unique_ptr` for memory management and provides efficient insertion, deletion, and search operations.



## Features

- ✅ **Automatic Memory Management**: Uses `std::unique_ptr` for RAII
- ✅ **Self-Balancing**: Automatically rebalances every 75 insertions
- ✅ **Generic Template**: Works with any comparable type
- ✅ **Standard Container Interface**: Familiar STL-like API
- ✅ **Multiple Construction Methods**: Default, single element, range, and initializer list
- ✅ **Tree Operations**: Join, rotate, and manual balancing
- ✅ **Traversal Methods**: In-order, pre-order, and post-order traversal



## Installation

Simply include the header file in your project:

```cpp
#include "bst.h"
```

## Quick Start

```cpp
#include <iostream>
#include "bst.h"

int main() {
    // Create a tree with initializer list
    tree::bst<int> bst{5, 3, 7, 1, 9, 4, 6};
    
    // Insert elements
    bst.insert(2);
    bst.insert(8);
    
    // Check if element exists
    if (bst.contains(5)) {
        std::cout << "Tree contains 5\n";
    }
    
    // Print tree size and height
    std::cout << "Size: " << bst.size() << "\n";
    std::cout << "Height: " << bst.height() << "\n";
    
    // Traverse the tree
    std::cout << "In-order: ";
    bst.infix_traversal();
    
    return 0;
}
```

## API Reference

### Template Parameters

| Parameter | Description |
|-----------|-------------|
| `T` | The type of elements stored in the container. Must support `<` and `>` operators. |

### Member Types

| Type | Definition |
|------|------------|
| `node` | Internal node structure containing `T data_`, `std::unique_ptr<node> left_`, and `std::unique_ptr<node> right_` |

### Constructors

#### Default Constructor
```cpp
bst();
```
Constructs an empty binary search tree.

#### Single Element Constructor
```cpp
explicit bst(const T& data);
```
Constructs a binary search tree with a single root node.

**Parameters:**
- `data` - The value to store in the root node

#### Range Constructor
```cpp
template<typename InputIt>
bst(InputIt first, InputIt last);
```
Constructs a binary search tree from the range `[first, last)`. Automatically balances the tree after construction.

**Parameters:**
- `first, last` - Input iterators defining the range of elements

#### Initializer List Constructor
```cpp
bst(std::initializer_list<T> list);
```
Constructs a binary search tree from an initializer list.

**Parameters:**
- `list` - Initializer list containing elements to insert

#### Copy Constructor
```cpp
bst(const bst& other);
```
Constructs a deep copy of another binary search tree.

#### Move Constructor
```cpp
bst(bst&& other) = default;
```
Move constructs a binary search tree from another tree.

### Modifiers

#### insert
```cpp
void insert(const T& data);
```
Inserts an element into the tree. Duplicates are ignored. Automatically rebalances every 75 insertions.

**Parameters:**
- `data` - The value to insert

**Complexity:** O(log n) average, O(n) worst case before rebalancing

#### erase
```cpp
void erase(const T& data);
```
Removes the element with the specified value from the tree.

**Parameters:**
- `data` - The value to remove

**Complexity:** O(log n) average, O(n) worst case

#### clear
```cpp
void clear();
```
Removes all elements from the tree.

**Complexity:** O(n)

#### join
```cpp
void join(bst& other);
```
Joins this tree with another tree. The maximum value in this tree must be less than the minimum value in the other tree.

**Parameters:**
- `other` - The tree to join with

**Throws:** `std::runtime_error` if the join precondition is not met

**Complexity:** O(log n)

### Lookup Operations

#### contains
```cpp
bool contains(const T& data) const;
```
Checks if the tree contains an element with the specified value.

**Parameters:**
- `data` - The value to search for

**Returns:** `true` if found, `false` otherwise

**Complexity:** O(log n) average, O(n) worst case

#### find
```cpp
node* find(const T& data) const;
```
Finds the node containing the specified value.

**Parameters:**
- `data` - The value to search for

**Returns:** Pointer to the node if found, `nullptr` otherwise

**Complexity:** O(log n) average, O(n) worst case

#### min
```cpp
T min() const;
```
Returns the minimum element in the tree.

**Returns:** The minimum value

**Precondition:** Tree must not be empty

**Complexity:** O(log n)

#### max
```cpp
T max() const;
```
Returns the maximum element in the tree.

**Returns:** The maximum value

**Precondition:** Tree must not be empty

**Complexity:** O(log n)

### Capacity

#### size
```cpp
int size() const;
```
Returns the number of elements in the tree.

**Returns:** Number of elements

**Complexity:** O(n)

#### is_empty
```cpp
bool is_empty() const;
```
Checks if the tree is empty.

**Returns:** `true` if empty, `false` otherwise

**Complexity:** O(1)

#### height
```cpp
int height() const;
```
Returns the height of the tree.

**Returns:** Height of the tree (-1 for empty tree)

**Complexity:** O(n)

### Element Access

#### root
```cpp
node* root() const;
```
Returns a pointer to the root node.

**Returns:** Pointer to root node or `nullptr` if empty

#### left
```cpp
node* left() const;
```
Returns a pointer to the left child of the root.

**Returns:** Pointer to left child or `nullptr`

**Precondition:** Tree must not be empty

#### right
```cpp
node* right() const;
```
Returns a pointer to the right child of the root.

**Returns:** Pointer to right child or `nullptr`

**Precondition:** Tree must not be empty

### Traversal

#### infix_traversal
```cpp
void infix_traversal() const;
void infix_traversal();
```
Performs in-order traversal, printing elements in sorted order.

#### prefix_traversal
```cpp
void prefix_traversal() const;
void prefix_traversal();
```
Performs pre-order traversal (root, left, right).

#### postfix_traversal
```cpp
void postfix_traversal() const;
void postfix_traversal();
```
Performs post-order traversal (left, right, root).

### Tree Operations

#### rotate_left
```cpp
void rotate_left();
```
Performs a left rotation on the root node.

#### rotate_right
```cpp
void rotate_right();
```
Performs a right rotation on the root node.

#### balance
```cpp
void balance(int index);
```
Manually balances the tree with the specified element at the given index position.

**Parameters:**
- `index` - Target position for balancing

### Comparison

#### operator==
```cpp
bool operator==(const bst& other) const;
```
Compares two trees for equality (structure and content).

**Parameters:**
- `other` - Tree to compare with

**Returns:** `true` if trees are equal, `false` otherwise

#### operator<<
```cpp
friend std::ostream& operator<<(std::ostream& os, const bst& tree);
```
Outputs the root node's data to the stream.

## Examples

### Basic Usage
```cpp
tree::bst<int> tree;
tree.insert(5);
tree.insert(3);
tree.insert(7);

std::cout << "Size: " << tree.size() << std::endl;  // Output: Size: 3
std::cout << "Contains 5: " << tree.contains(5) << std::endl;  // Output: Contains 5: 1
```

### Range Construction
```cpp
std::vector<int> data = {1, 3, 5, 7, 9};
tree::bst<int> tree(data.begin(), data.end());
tree.infix_traversal();  // Output: 1 3 5 7 9
```

### Tree Joining
```cpp
tree::bst<int> tree1{1, 2, 3};
tree::bst<int> tree2{4, 5, 6};
tree1.join(tree2);  // tree1 now contains all elements
```

### Custom Types
```cpp
struct Person {
    std::string name;
    int age;
    
    bool operator<(const Person& other) const {
        return age < other.age;
    }
    
    bool operator>(const Person& other) const {
        return age > other.age;
    }
};

tree::bst<Person> people;
people.insert({"Alice", 25});
people.insert({"Bob", 30});
```

## Complexity

| Operation | Average |
|-----------|---------|
| Insert | O(log n) |
| Erase | O(log n) | 
| Find | O(log n) | 
| Contains | O(log n) |
| Min/Max | O(log n) |
| Size | O(n) |
| Height | O(n) |
| Clear | O(n) |

## Requirements

- C++11 or later
- Standard library support for `<memory>`, `<algorithm>`, `<iostream>`, `<stdexcept>`

## Notes

- The tree automatically rebalances every 75 insertions (configurable via `BALANCE_INTERVAL`)
- Duplicate values are ignored during insertion
- The tree uses the DSW (Day-Stout-Warren) algorithm approach for balancing
- Memory management is handled automatically through `std::unique_ptr`
