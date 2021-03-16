#include "suffix_tree_node.h"

Suffix_tree_area::Suffix_tree_area(){
	left = NULL;
	right = NULL;
	pos_first = -1;
	pos_last = 0;
	char_first = '\0';
}

Suffix_tree_area::~Suffix_tree_area(){
	delete right;
}

Suffix_tree_node::Suffix_tree_node(){
	parent_area = NULL;
	map = new std::map <char, int>();
	suffix_link = NULL;
	areas = NULL;
	last_idx = 0;
}

void Suffix_tree_node::Add_area(char c, Suffix_tree_area* area){
	map->insert(std::pair<char, int>(c, last_idx));
	last_idx++;
	areas = (Suffix_tree_area**)realloc(areas, last_idx*sizeof(Suffix_tree_area*));
	areas[last_idx-1] = area;
}

Suffix_tree_area* Suffix_tree_node::Get_area_by_idx(int i){
	return areas[i];
}

Suffix_tree_area* Suffix_tree_node::Get_area(char c){
	std::map<char, int>::iterator it = map->find(c);
	int idx;

	if (it != map->end()) {
		idx = it->second;
	}
	else {
		return NULL;
	}

	return areas[idx];
}

int Suffix_tree_node::Get_numb_area(){
	return last_idx;
}

Suffix_tree_node::~Suffix_tree_node(){
	for (int i = 0; i < last_idx; ++i){
		delete areas[i];
	}

	delete [] areas;
	delete map;
}
