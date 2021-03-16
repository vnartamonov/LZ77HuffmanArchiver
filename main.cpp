/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include "lz77h_archiver.h"
/////////////////////////////////////////////////////////////////////////////////////////
const char file_format[] = ".lz77h";
/////////////////////////////////////////////////////////////////////////////////////////
void print_error_code ( ErrorCode error)
{
	switch
	(
		error
	)
	{
		case ERROR_OPEN_FILE:
			std::cout << std::endl << "\tERROR_OPEN_FILE "  <<  std::endl;
		break;

		case FILE_NOT_FOUND:
			std::cout << std::endl << "\tFILE_NOT_FOUND "    << std::endl;
		break;

		case INVALID_OPERATION:
			std::cout << std::endl << "\tINVALID_OPERATION " << std::endl;
		break;

		case INVALID_FILE:
			std::cout << std::endl << "\tINVALID_FILE "      << std::endl;
		break;

		case NO_MEMORY:
			std::cout << std::endl << "\tNO_MEMORY "         << std::endl;
		break;

		case NO_ERROR:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
int main
(
	int     argc,
	char * argv[]
)
{
	ErrorCode error = NO_ERROR;

	for 
		(
			int count  = 0;
			count < argc-1;
			++count
		)
		{

		if 
			(
				!strcmp( argv[count], "-c" )
			)
			{
				std::string  input_file_name ( argv[count+1] );
				std::string output_file_name = input_file_name + file_format;
				error = LZ77Harchiver::encode ( input_file_name, 
												output_file_name, 
												NORMAL_MODE 
											  );
				break;
			} 	

		else if
			(
				!strcmp( argv[count], "-d" )
			)
			{
				std::string input_file_name   ( argv[count+1] );
				unsigned len = input_file_name.size() - strlen ( file_format );
				std::string output_file_name  ( input_file_name, 0, len );
				error = LZ77Harchiver::decode ( input_file_name, 
												output_file_name 
											   );
				break;
			} 

		else if
			(
				!strcmp( argv[count], "-t" )
			)
			{
				std::string  input_file_name ( argv[count+1]   );
				error = LZ77Harchiver::check ( input_file_name );
				break;
			}	 

		else if
			(
				!strcmp( argv[count], "-1" )
			)
			{
				std::string  input_file_name ( argv[count+1] );
				std::string output_file_name = input_file_name + file_format;
				error = LZ77Harchiver::encode ( input_file_name, 
												output_file_name, 
												GOOD_MODE 
											  );
				break;
			} 

		else if
			(
				!strcmp( argv[count], "-9" )
			)
			{
				std::string  input_file_name ( argv[count+1] );
				std::string output_file_name = input_file_name + file_format;
				error = LZ77Harchiver::encode ( input_file_name, 
												output_file_name, 
												MAXIMUM_MODE 
											  );
				break;
			}
		}

	print_error_code ( error );

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
