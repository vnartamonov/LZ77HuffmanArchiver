#ifndef HUFFMAN_HEAD
#define	HUFFMAN_HEAD

/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "lz77h_archiver.h"
#include "bitmap.h"
/////////////////////////////////////////////////////////////////////////////////////////
typedef bool Bit;
//---------------------------------------------------------------------------------------
typedef enum {
	LZ77_HUFFMAN,
	ONLY_HUFFMAN
} HuffmanCodeCompress;
//---------------------------------------------------------------------------------------
typedef struct HuffmanTreeNode
{
	HuffmanTreeNode *   left;
	HuffmanTreeNode *  right;
	HuffmanTreeNode * parent;
	unsigned        *   data;
	unsigned          weight;
}   HuffmanTreeNode;
/////////////////////////////////////////////////////////////////////////////////////////
const int        size_alphabet =  257;
const unsigned   _EOF          =  256;

const bool START_DEBUG_HUFFMAN = false;
/////////////////////////////////////////////////////////////////////////////////////////
class HuffmanCode
{
public:
	//-----------------------------------------------------------------------------------
	static ErrorCode encode
		(
			std::string &  input_file_name,
			std::string & output_file_name,
			HuffmanCodeCompress       mode
		);
	//-----------------------------------------------------------------------------------
	static ErrorCode decode
		(
			std::string &  input_file_name,
			std::string & output_file_name
		);

private:
	//-----------------------------------------------------------------------------------
	static unsigned * creat_probability_table
		(
			std::string   & input_file_name,
			std::ifstream & input_file
		);
	//-----------------------------------------------------------------------------------
	static std::vector < Bit > & encode_data
		(
			unsigned data,
			std::vector < std::vector < Bit > > * code
		);
	//-----------------------------------------------------------------------------------
	static unsigned decode_data
		(
			std::vector < Bit > & data,
			HuffmanTreeNode *     root,
			std::vector < std::vector < Bit > > * code
		);
	//-----------------------------------------------------------------------------------
	static HuffmanTreeNode * create_tree
		(
			unsigned *        probability,
			HuffmanTreeNode ** temp_nodes
		);
	//-----------------------------------------------------------------------------------
	static bool tree_node_compare
		(
			const HuffmanTreeNode * a,
			const HuffmanTreeNode * b
		);
	//-----------------------------------------------------------------------------------
	static std::vector < std::vector < Bit > > * create_static_code
		(
			HuffmanTreeNode * root,
			unsigned size_alphabet
		);
	//-----------------------------------------------------------------------------------
	static void lcr_huffman_tree
		(
			HuffmanTreeNode * point,
			std::vector < std::vector < Bit > > * code,
			std::vector <Bit> & current_code
		);
	//-----------------------------------------------------------------------------------
	static void delete_huffman_tree
		(
			HuffmanTreeNode * point
		);
	//-----------------------------------------------------------------------------------
};

#endif
