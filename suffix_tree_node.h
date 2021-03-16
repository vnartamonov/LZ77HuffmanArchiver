#ifndef SUFFIX_TREE_NODE_H
#define SUFFIX_TREE_NODE_H

#include <map>
#include <iostream>
#include <cstdlib>

void print();

class Suffix_tree_node;

class Suffix_tree_area{
public:
	Suffix_tree_area();
	~Suffix_tree_area();

	Suffix_tree_node* left;
	Suffix_tree_node* right;

	char char_first;
	int pos_first, pos_last;
};

class Suffix_tree_node{
public:
	Suffix_tree_node();

	void Add_area(char c, Suffix_tree_area* area);
	Suffix_tree_area* Get_area_by_idx(int i);
	Suffix_tree_area* Get_area(char c);
	int Get_numb_area();

	~Suffix_tree_node();

	Suffix_tree_area* parent_area;
	Suffix_tree_node* suffix_link;

private:
	std::map <char, int> * map;
	Suffix_tree_area** areas;
	int last_idx;
};

#endif
