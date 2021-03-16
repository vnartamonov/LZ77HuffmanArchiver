#ifndef ZL77_HEAD
#define	ZL77_HEAD

/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "lz77h_archiver.h"
#include "suffix_tree.h"
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct LZ77Triplet{
	unsigned  shift;
	unsigned length;
	char       data;
} LZ77Triplet;
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned NORMAL_MODE_SIZE  = 1*1024*1024; //  1Mb
const unsigned GOOD_MODE_SIZE    = 2*1024*1024; //  2Mb
const unsigned MAXIMUM_MODE_SIZE = 4*1024*1024; //  4Mb

const bool START_DEBUG_LZ77 = false;
/////////////////////////////////////////////////////////////////////////////////////////
class LZ77
{
public:
	//-----------------------------------------------------------------------------------
	static ErrorCode encode
		(
			std::string &  input_file_name,
			std::string & output_file_name,
			LZ77Mode mode
		);
	//-----------------------------------------------------------------------------------
	static ErrorCode decode
		(
			std::string &  input_file_name,
			std::string & output_file_name
		);

private:
	//-----------------------------------------------------------------------------------
	static unsigned set_dictionary_size
		(
			LZ77Mode mode
		);
	//-----------------------------------------------------------------------------------
	static LZ77Triplet get_next_triplet
		(
			std::ifstream & input_file,
			SuffixTree * dictionary
		);
	//-----------------------------------------------------------------------------------
	static unsigned get_unsigned 
		(
			std::ifstream & input_file
		);
	//-----------------------------------------------------------------------------------
	static char get_char 
		(
			std::ifstream & input_file
		);
	//-----------------------------------------------------------------------------------
	static void put_unsigned 
		(
			std::ofstream & output_file,
			unsigned data
		);
	//-----------------------------------------------------------------------------------
	static void put_char
		(
			std::ofstream & output_file,
			char data
		);
	//-----------------------------------------------------------------------------------
};

#endif
