#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <iostream>
#include <string>
#include <climits>
#include <vector>
#include "suffix_tree_node.h"

const int INF = INT_MAX / 2;

typedef struct{
	Suffix_tree_area * current_area;
	int step;
} Navigator;

class SuffixTree{
public:
	SuffixTree();

	void add_char(char item);
	unsigned search(std::vector <char> & text);
	unsigned size();

	~SuffixTree();

private:
	Navigator Get_suffix_link(Navigator navigator);
	Suffix_tree_node* Split(Navigator navigator);

	Suffix_tree_node* root;
	Suffix_tree_node* extra_node;
	Navigator navigator;
	std::vector<char> str;
};

#endif
