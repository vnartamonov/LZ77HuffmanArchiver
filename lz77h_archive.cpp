#include "lz77h_archiver.h"
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode LZ77Harchiver::encode
(
	std::string &  input_file_name,
	std::string & output_file_name,
	LZ77Mode mode
)
{
	std::string temp_file_name = "." + output_file_name + ".lz77h.zip.temp";

	int original_file_size  = 0;
	int temp_lz77_file_size = 0;

	try
		{
			original_file_size = size_file ( input_file_name );
		}
	catch
		(
			ErrorCode error
		)
		{
			return error;
		}

	ErrorCode lz77_error = LZ77::encode ( input_file_name, temp_file_name, mode );

	if 
		(
			lz77_error != NO_ERROR
		)
		{
			return lz77_error;
		}

	try
		{
			temp_lz77_file_size = size_file ( temp_file_name );
		}
	catch
		(
			ErrorCode error
		)
		{
			return error;
		}

	ErrorCode huff_error = NO_ERROR;

	if 
		(
			original_file_size < temp_lz77_file_size
		)
		{
			huff_error = HuffmanCode::encode ( 
											   input_file_name, 
											   output_file_name,
											   ONLY_HUFFMAN
											  );	
		}
	else
		{
			huff_error = HuffmanCode::encode ( 
												temp_file_name, 
												output_file_name, 
												LZ77_HUFFMAN 
											  );
		}

	if 
		(
			huff_error != NO_ERROR
		)
		{
			return huff_error;
		}

	if 
		( 
			remove( temp_file_name.c_str() ) != 0 
		)
		{
    		return INVALID_OPERATION;
		}

 	return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode LZ77Harchiver::decode
(
	std::string &  input_file_name,
	std::string & output_file_name
)
{
	std::string temp_file_name = "." + output_file_name + ".lz77h.unzip.temp";
	bool only_huffman = false;
	ErrorCode huff_error;

	if 
		(	
			check ( input_file_name ) != NO_ERROR
		)
		{
			return INVALID_FILE;
		}

	only_huffman = is_only_huffman ( input_file_name );

	if 
		(
			only_huffman
		)
		{
			huff_error = HuffmanCode::decode ( input_file_name, output_file_name );
			return huff_error;			
		}
	else
		{
			huff_error = HuffmanCode::decode ( input_file_name,   temp_file_name );
		}

	if 
		(
			huff_error != NO_ERROR
		)
		{
			return huff_error;
		}

	ErrorCode lz77_error = LZ77::decode ( temp_file_name, output_file_name );

	if 
		(
			lz77_error != NO_ERROR
		)
		{
			return lz77_error;
		}

	if 
		( 
			remove( temp_file_name.c_str() ) != 0 
		)
		{
    		return INVALID_OPERATION;
		}

	return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode LZ77Harchiver::check
(
	std::string & file_name
)
{
	std::ios_base::openmode i_file_mode = std::ios::in  | std::ios::binary;
	std::ifstream input_file ( file_name.c_str(), i_file_mode );

	if
		(
			!input_file.is_open()
		)
		{
			return ERROR_OPEN_FILE;
		}

	HuffmanCodeCompress mode;
	input_file.read ( ( char * ) & mode, sizeof ( HuffmanCodeCompress ) );

	if 
		(
			!(   
					mode == ONLY_HUFFMAN
				|| mode == LZ77_HUFFMAN
			)
		)
		{
			return INVALID_FILE;
		}

	char check_file[check_len+1];
	input_file.read ( check_file, check_len * sizeof ( char ) );

	if 
		(
			strncmp( check_file, check_str, check_len )
		)
		{
			input_file.close();
			
			return INVALID_FILE;
		}

	input_file.close();

	return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
int LZ77Harchiver::size_file
(
	std::string & file_name
)
{
	std::fstream file ( file_name.c_str() );
    int size = 0;

    if 
    	(
    		!file.is_open()
    	)
    	{
    		throw ERROR_OPEN_FILE;
    	}

    file.seekg ( 0, std::ios::end );
    size = file.tellg();

    file.close();

    if 
    	(
    		size < 0
    	)
    	{
    		throw INVALID_FILE;
    	}

    return size;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool LZ77Harchiver::is_only_huffman
(
	std::string & file_name
)
{
	std::ios_base::openmode i_file_mode = std::ios::in | std::ios::binary;
	std::ifstream input_file ( file_name.c_str(), i_file_mode );

	HuffmanCodeCompress mode;
	input_file.read ( ( char * ) & mode, sizeof ( HuffmanCodeCompress ) );

	return ( mode == ONLY_HUFFMAN );
}
/////////////////////////////////////////////////////////////////////////////////////////
