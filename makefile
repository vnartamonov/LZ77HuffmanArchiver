all:
	g++ -O2 -o lz77hzip main.cpp lz77h_archive.cpp lz77.cpp suffix_tree.cpp suffix_tree_node.cpp huffman_code.cpp bitmap.cpp