#ifndef LZ77H_ARCHIVER_HEAD
#define	LZ77H_ARCHIVER_HEAD

/////////////////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
	NO_ERROR,
	ERROR_OPEN_FILE,
	FILE_NOT_FOUND,
	INVALID_OPERATION,
	INVALID_FILE,
	NO_MEMORY
} ErrorCode;
//---------------------------------------------------------------------------------------
typedef enum {
	NORMAL_MODE,
	GOOD_MODE,
	MAXIMUM_MODE
} LZ77Mode;
/////////////////////////////////////////////////////////////////////////////////////////
#include "lz77.h"
#include "huffman_code.h"
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned check_len = 8;
const char check_str[check_len+1] = { 0x12, 0x10, 0x43, 0x11, 0x77, 0x43, 0x20, 0x32 };
/////////////////////////////////////////////////////////////////////////////////////////
class LZ77Harchiver
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
	//-----------------------------------------------------------------------------------
	static ErrorCode check
		(
			std::string & file_name
		);
	
private:
	//-----------------------------------------------------------------------------------
	static int size_file
		(
			std::string & file_name
		);
	//-----------------------------------------------------------------------------------
	static bool is_only_huffman
		(
			std::string & file_name
		);
	//-----------------------------------------------------------------------------------
};

#endif
