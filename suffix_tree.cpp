#include "suffix_tree.h"

SuffixTree::SuffixTree(){
	root = new Suffix_tree_node();
	extra_node = new Suffix_tree_node();
    extra_node->suffix_link = NULL;

	for (int i = SCHAR_MIN; i <= SCHAR_MAX; ++i){
		Suffix_tree_area* area = new Suffix_tree_area();
        area->right = root;
        area->left = extra_node;
        area->char_first = i;
        extra_node->Add_area(i, area);
    }

    root->parent_area = extra_node->Get_area(SCHAR_MIN);
    root->suffix_link = extra_node;
    navigator.current_area = extra_node->Get_area(SCHAR_MIN);
    navigator.step = 0;
}

void SuffixTree::add_char(char item){
	while (true){
		if (navigator.current_area->pos_last != navigator.step){
			if (str[navigator.step] == item){
				navigator.step++;
				break;
			}
			else{
				Suffix_tree_node* temp_node = Split(navigator);
				Suffix_tree_area* area = new Suffix_tree_area();

				area->char_first = item;
				area->pos_first = str.size();
				area->pos_last = INF;
				area->right = new Suffix_tree_node();
				area->left = navigator.current_area->right;

				temp_node->Add_area(item, area);
				if (navigator.current_area->left == extra_node) break;
				navigator = Get_suffix_link(navigator);
			}
		}
		else{
			if (navigator.current_area->right->Get_area(item) != NULL){
				navigator.current_area = navigator.current_area->right->Get_area(item);
				navigator.step = navigator.current_area->pos_first + 1;
				break;
			}
			else{
				Suffix_tree_area* area = new Suffix_tree_area();

				area->char_first = item;
				area->pos_first = str.size();
				area->pos_last = INF;
				area->right = new Suffix_tree_node();
				area->left = navigator.current_area->right;

				navigator.current_area->right->Add_area(item, area);
				if (navigator.current_area->left == extra_node) break;
				navigator = Get_suffix_link(navigator);
			}
		}
	}
	str.push_back(item);
}

Suffix_tree_node* SuffixTree::Split(Navigator navigator){
	if (navigator.current_area->pos_last == navigator.step){
		return navigator.current_area->right;
	}

	Suffix_tree_node* temp_node = new Suffix_tree_node();
	Suffix_tree_area* temp_area = new Suffix_tree_area();

	temp_area->char_first = str[navigator.step];
	temp_area->pos_first = navigator.step;
	temp_area->pos_last = navigator.current_area->pos_last;
	temp_area->left = temp_node;
	temp_area->right = navigator.current_area->right;
	temp_node->Add_area(str[navigator.step], temp_area);

	navigator.current_area->right = temp_node;
	navigator.current_area->pos_last = navigator.step;

	temp_node->parent_area = navigator.current_area;
	Navigator temp_navigator = Get_suffix_link(navigator);
	temp_node->suffix_link = Split(temp_navigator);

	return temp_node;
}

Navigator SuffixTree::Get_suffix_link(Navigator navigator){
	Suffix_tree_node* node = navigator.current_area->left->suffix_link;
	Navigator temp_navigator;
	int current_step = navigator.current_area->pos_first;

	if (node == NULL){
		return navigator;
	}
	
	while (true){
		Suffix_tree_area* current_area = node->Get_area(str[current_step]);
		current_step += (current_area->pos_last - current_area->pos_first);

		if (navigator.step <= current_step){
			temp_navigator.step = navigator.step + current_area->pos_last - current_step;
			temp_navigator.current_area = current_area;
			return temp_navigator;
		}

		node = current_area->right;
	}
}

unsigned SuffixTree::search(std::vector <char> & text){
	if (size() == 0) return 0;
	if (text.size() > size()) return 0;

	Navigator _navigator;
	unsigned count = 0, output = 0;
	
	_navigator.current_area = root->Get_area(text[0]);
	
	if (_navigator.current_area == NULL){
		return 0;
	}
	
	_navigator.step = _navigator.current_area->pos_first;
	
	while ((count < text.size()) && ((unsigned)_navigator.step < size()) && str[_navigator.step] == text[count] && ((unsigned)_navigator.step < str.size())){
		++_navigator.step;
		++count;

		if (count >= text.size()) {
			return _navigator.step - text.size()+1;
		}

		if (_navigator.step == _navigator.current_area->pos_last){
			if (_navigator.current_area->right->Get_area(text[count]) != NULL){
				_navigator.current_area = _navigator.current_area->right->Get_area(text[count]);
				_navigator.step = _navigator.current_area->pos_first;
				output = _navigator.current_area->pos_first;
			}
			else break;
		}
	}

	return 0;
}

unsigned SuffixTree::size(){
	return str.size();
}

SuffixTree::~SuffixTree(){
	delete root;
}
