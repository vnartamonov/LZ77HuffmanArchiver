#include "lz77.h"
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode LZ77::encode
(
	std::string &  input_file_name,
	std::string & output_file_name,
	LZ77Mode mode
)
{
	std::ios_base::openmode i_file_mode = std::ios::in  | std::ios::binary;
	std::ios_base::openmode o_file_mode = std::ios::out | std::ios::binary;
	std::ifstream     input_file (  input_file_name.c_str(), i_file_mode );
	std::ofstream    output_file ( output_file_name.c_str(), o_file_mode );

	if
		(
			    !input_file.is_open()
			|| !output_file.is_open()
		)
		{
			return ERROR_OPEN_FILE;
		}

    SuffixTree * dictionary = new SuffixTree();

    if
    	(
    		dictionary == NULL
    	)
    	{
    		return NO_MEMORY;
    	}

	output_file.write( ( char * ) & mode, sizeof( LZ77Mode ) );
	unsigned size_dictionary = set_dictionary_size ( mode );

	if
		(
			START_DEBUG_LZ77
		)
		{
			std::cout << "LZ77 ENCODE" << std::endl;		
		}

	bool eof = false;

	while
		(
			!eof
		)
		{
			LZ77Triplet current_triplet;

			try
				{
					current_triplet = get_next_triplet ( input_file, dictionary );
				}
			catch
				(
					bool exp
				)
				{
					eof = exp;
				}

			if 
				(
					eof == true
				)
				{
					break;
				}

			if 
				(
					dictionary-> size() > size_dictionary
				)
				{
					delete dictionary;
					dictionary = new SuffixTree();

					if
						(
							dictionary == NULL
						)
						{
							return NO_MEMORY;
						}
				}
			
			put_unsigned ( output_file, current_triplet. shift );
			put_unsigned ( output_file, current_triplet.length );
			put_char     ( output_file, current_triplet.  data );

			if
				(
					START_DEBUG_LZ77
				)
				{
					std::cout << "--------------------"               << std::endl
							  << "shift : " << current_triplet.shift  << std::endl
							  << "length: " << current_triplet.length << std::endl
							  << "char  : " << current_triplet.data   << std::endl
							  << "dic sz: " << dictionary-> size() 	  << std::endl;
				}
		}

	input_file .close();
	output_file.close();

	delete dictionary;

	return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode LZ77::decode
(
	std::string &  input_file_name,
	std::string & output_file_name
)
{
	std::ios_base::openmode i_file_mode = std::ios::in  | std::ios::binary;
	std::ios_base::openmode o_file_mode = std::ios::out | std::ios::binary;
	std::ifstream     input_file (  input_file_name.c_str(), i_file_mode );
	std::ofstream    output_file ( output_file_name.c_str(), o_file_mode );

	if
		(
			    !input_file.is_open()
			|| !output_file.is_open()
		)
		{
			return ERROR_OPEN_FILE;
		}

	std::vector < char > dictionary;

    LZ77Mode mode;
    input_file.read ( ( char * ) & mode, sizeof ( mode ) );
    unsigned size_dictionary = set_dictionary_size ( mode );

    LZ77Triplet current_triplet;

    if
    	(
    		START_DEBUG_LZ77
    	)
    	{
    		std::cout << std::endl 
    				  << std::endl 
    				  << "LZ77 DECODE" 
    				  << std::endl;		
    	}
	
    while
    	(
    		!input_file.eof()
    	)
    	{
    		current_triplet.shift = get_unsigned ( input_file );

    		if
    			(
    				input_file.eof()
    			)
    			{
    				break;
    			}

    		current_triplet.length = get_unsigned ( input_file );
    		current_triplet.data   = get_char     ( input_file );

    		if
    			(
    				START_DEBUG_LZ77
    			)
    			{
					std::cout << "--------------------" << std::endl
							  << "dictionary: " 		<< std::endl;

					for( unsigned i = 0; i < dictionary.size(); ++i )
						std::cout << dictionary[i];

					std::cout << std::endl
							  << "shift : " << current_triplet.shift  << std::endl
							  << "length: " << current_triplet.length << std::endl
							  << "char  : " << current_triplet.data   << std::endl;
    			}

    		unsigned size_buffer = dictionary.size();

    		for
    			(
    				unsigned count = 0;
    				count < current_triplet.length;
    				++count

    			)
    			{
    				char temp = dictionary[ size_buffer - current_triplet.shift + count ];
    				dictionary.push_back ( temp );
    				put_char ( output_file, temp );
    			}

    		dictionary.push_back  ( current_triplet.data );
    		put_char ( output_file, current_triplet.data );
    	}

	input_file .close();
	output_file.close();

    return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned LZ77::set_dictionary_size
(
	LZ77Mode mode
)
{
	switch
		(
			mode
		)
		{
			case NORMAL_MODE: 
				return NORMAL_MODE_SIZE;

			case GOOD_MODE: 
				return GOOD_MODE_SIZE;

			case MAXIMUM_MODE: 
				return MAXIMUM_MODE_SIZE;
		}
	
	throw INVALID_OPERATION;
}
/////////////////////////////////////////////////////////////////////////////////////////
LZ77Triplet LZ77::get_next_triplet
(
	std::ifstream & input_file,
	SuffixTree    * dictionary
)
{
	std::vector <char> buffer;
	LZ77Triplet        output;
	char                 temp;
	int     current_shift = 0;
	int        last_shift = 0;

	do
		{
			temp = get_char( input_file );

			if
				(
					input_file.eof()
				)
				{
					if
						(
							buffer.size() > 0
						)
						{
							current_shift = 0;
							break;
						}
					else
						{
							throw true;
						}
				}

			buffer.push_back ( temp );
			last_shift    = current_shift;
			current_shift = dictionary-> search ( buffer );
		}
	while
		(
			current_shift != 0
		);

	if
		(
			last_shift    == 0
		)
		{
			output.shift   = 0;
			output.length  = 0;
		}
	else
		{
			output.shift  = ( unsigned ) dictionary-> size() - last_shift + 1;
			output.length = buffer.size() - 1;
		}

	output.data = buffer[ buffer.size() - 1 ];

	for
		(
			unsigned count    = 0;
			count < buffer.size();
			++count
		)
		{
			dictionary-> add_char ( buffer[ count ] );
		}

	return output;
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned LZ77::get_unsigned
(
	std::ifstream & input_file
)
{
	unsigned data;
	void * point = &data;

	for
		(
			unsigned char count = 0; 
			count < 3;
			++count
		)
		{
			char temp;
			input_file.read ( ( char * ) & temp, sizeof ( char ) );
			( * ( ( (char * ) point ) + count ) ) = temp;
		}

	( * ( ( (char * ) point ) + 3 ) ) = ( char ) 0;

	return data;
}
/////////////////////////////////////////////////////////////////////////////////////////
char LZ77::get_char
(
	std::ifstream & input_file
)
{
	char temp;

	input_file.read ( ( char * ) & temp, sizeof ( char ) );

	return temp;
}
/////////////////////////////////////////////////////////////////////////////////////////
void LZ77::put_unsigned 
(
	std::ofstream & output_file,
	unsigned data
)
{
	void * point = &data;

	for
		(
			unsigned char count = 0; 
			count < 3;
			++count
		)
		{
			char temp =  * ( ( (char * ) point ) + count );
			output_file.write ( ( char * ) & temp, sizeof ( char ) );
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void LZ77::put_char
(
	std::ofstream & output_file,
	char data
)
{
	output_file.write ( ( char* ) & data, sizeof ( char ) );	
}
/////////////////////////////////////////////////////////////////////////////////////////
