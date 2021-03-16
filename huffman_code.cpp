#include "huffman_code.h"
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode HuffmanCode::encode
(
	std::string &  input_file_name,
	std::string & output_file_name,
	HuffmanCodeCompress       mode
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

	output_file.write ( ( char * ) & mode, sizeof ( HuffmanCodeCompress ) );
	output_file.write ( check_str, check_len );

	unsigned * probability;

	try
		{
			probability = creat_probability_table ( input_file_name, input_file );		
		}
	catch
		(
			ErrorCode error
		)
		{
			return error;
		}

	BitMap * bit_stream  = new BitMap( input_file, output_file );
	HuffmanTreeNode * nodes = NULL;
	HuffmanTreeNode * root  = create_tree ( probability, &nodes );
	std::vector < std::vector < Bit > > * code = create_static_code ( root, size_alphabet );

	if
		(
			   bit_stream == NULL
			|| root       == NULL
			|| code       == NULL
		)
		{
			return NO_MEMORY;
		}

	output_file.write ( ( char * ) probability, sizeof( unsigned ) * size_alphabet );
	
	if 
		(
			START_DEBUG_HUFFMAN == true
		)
		{
			std::cout << "HuffmanCode ENCODE" << std::endl;
		}

	while
		(
			true
		)
		{
			unsigned char temp;

			input_file.read ( ( char * ) & temp, sizeof ( char ) );

			if
				(
					input_file.eof() == true
				)
				{
					break;
				}

			if 
				(
					START_DEBUG_HUFFMAN == true
				)
				{
					std::vector <Bit> v = encode_data ( temp, code );

					std::cout << ( unsigned ) temp << " : ";

					for 
						(
							unsigned i = 0;
							i < v.size();
							++i
						)
						{
							std::cout << v[i] << " ";
						}

					std::cout << std::endl;
				}

			bit_stream-> putbits ( encode_data ( temp, code ) );
		}

	bit_stream-> putbits ( encode_data ( _EOF, code ) );
	bit_stream-> flush();

	input_file .close();
	output_file.close();

	delete_huffman_tree ( root );
	delete 			  bit_stream;
	delete                  code;
	delete []              nodes;
	delete []        probability;

    return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
ErrorCode HuffmanCode::decode
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

	HuffmanCodeCompress     mode;
	input_file.read ( ( char * ) & mode, sizeof ( HuffmanCodeCompress ) );

	char check_file[check_len+1];
	input_file.read ( check_file, check_len * sizeof ( char ) );

	if 
		( 
			strncmp( check_file, check_str, check_len )
		)
		{
			input_file .close();
			output_file.close();
			
			return INVALID_FILE;
		}

	unsigned probability[size_alphabet];
	memset ( probability, 0, size_alphabet * sizeof( unsigned ) );
	input_file.read ( ( char * ) probability, sizeof( unsigned ) * size_alphabet );

	BitMap * bit_stream = new BitMap( input_file, output_file );
	HuffmanTreeNode * nodes = NULL;
	HuffmanTreeNode * root  = create_tree ( probability, &nodes );

	if
		(
			   bit_stream == NULL
			|| root       == NULL
		)
		{
			return NO_MEMORY;
		}

	bool end = false;

	if 
		(
			START_DEBUG_HUFFMAN == true
		)
		{
			std::cout << std::endl
					  << "HuffmanCode DECODE" 
					  << std::endl;
		}

	while
		(
			!end
		)
		{
			HuffmanTreeNode * point = root;
			bool check = true;

			if 
				(
					START_DEBUG_HUFFMAN == true
				)
				{
					std::cout << "code: ";
				}

			while
				(
					check
				)
				{
					Bit current_bit = bit_stream-> getbit();

					if
						(
							current_bit == false
						)
						{
							point = point-> left;

							if 
								(
									START_DEBUG_HUFFMAN == true
								)
								{
									std::cout << "0 ";
								}
						}

					else
						{
							point = point->right;

							if 
								(
									START_DEBUG_HUFFMAN == true
								)
								{
									std::cout << "0 ";
								}
						}

					check = point->data == NULL;
				}

			if 
				(
					START_DEBUG_HUFFMAN == true
				)
				{
					std::cout << std::endl 
							  << "data: " 
							  << *(point->data) 
							  << std::endl;
				}

			if
				(
					*(point->data) == _EOF
				)
				{
					end = true;
				}
			else
				{
					char data = ( char ) * ( point->data );
					output_file.write ( & data, sizeof ( char ) );
				}
		}

	input_file .close();
	output_file.close();

	delete_huffman_tree ( root );
	delete 			  bit_stream;
	delete []              nodes;

	return NO_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned * HuffmanCode::creat_probability_table
(
	std::string   & input_file_name,
	std::ifstream & input_file
)
{
	unsigned * probability = new unsigned [size_alphabet];
	unsigned char buffer;

	if
		(
			probability == NULL
		)
		{
			throw    NO_MEMORY;
		}

	memset ( probability, 0, size_alphabet * sizeof( unsigned ) );

	while
		(
			!input_file.eof()
		)
 		{
 			input_file.read ( ( char * ) & buffer, sizeof( char ) );
 			probability[buffer]++;
 		}

	probability[buffer]--;
 	probability[_EOF] = 1;

 	input_file.close();
 	input_file.open ( input_file_name.c_str(), std::ios::in | std::ios::binary );

 	if
 		(
 			!input_file.is_open()
 		)
 		{ 
 			delete [] probability;
 			throw ERROR_OPEN_FILE;
 		}

 	return probability;
}
/////////////////////////////////////////////////////////////////////////////////////////
std::vector < Bit > & HuffmanCode::encode_data
(
	unsigned data,
	std::vector < std::vector < Bit > > * code
)
{
	return (*code)[data];
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned HuffmanCode::decode_data
(
	std::vector < Bit > & data,
	HuffmanTreeNode *     root,
	std::vector < std::vector < Bit > > * code
)
{
	HuffmanTreeNode * point = root;
	unsigned count;

	for
		(
			count =  0;
			   count < data.size ()
			&& point !=       NULL;
			++count
		)
		{
			if
				(
					data[count] == false
				)
				{
					point = point->left;
				}

			else
				{
					point = point->right;
				}
		}

	if
		(
			   count == data.size ()
			&& point !=         NULL
		)
		{
			return *(point->data);
		}

	else
		{
			throw true;
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
HuffmanTreeNode * HuffmanCode::create_tree
(
	unsigned *        probability,
	HuffmanTreeNode ** temp_nodes
)
{
	HuffmanTreeNode * nodes = new HuffmanTreeNode[size_alphabet];
	std::vector < HuffmanTreeNode * > link_nodes;

	for
		(
			unsigned count =    0; 
			count < size_alphabet;
			++count
		)
		{
			nodes[count].   data =  new unsigned ( count );		
			nodes[count]. weight =      probability[count];
			nodes[count].   left =                    NULL;
			nodes[count].  right =                    NULL;
			nodes[count]. parent =                    NULL;

			link_nodes.push_back( &nodes[count] );
		}

	while 
		( 
			link_nodes.size () > 1 
		)
		{
			std::sort ( link_nodes.begin(), link_nodes.end(), tree_node_compare );

			unsigned size_nodes    =   link_nodes.size();
			HuffmanTreeNode * temp = new HuffmanTreeNode;

			temp->weight = link_nodes[size_nodes-2]->weight + 
						   link_nodes[size_nodes-1]->weight;
			temp->  left = link_nodes[size_nodes-2];
			temp-> right = link_nodes[size_nodes-1];
			temp->  data =          		   NULL;
			temp->parent =       			   NULL;

			link_nodes[size_nodes-1]->parent = temp;
			link_nodes[size_nodes-2]->parent = temp;

			link_nodes.pop_back        ();
			link_nodes.pop_back        ();
			link_nodes.push_back ( temp );
		}

	(*temp_nodes) = nodes;

	return link_nodes[0];
}
/////////////////////////////////////////////////////////////////////////////////////////
bool HuffmanCode::tree_node_compare
(
	const HuffmanTreeNode * a,
	const HuffmanTreeNode * b
)
{
	return ( a->weight > b->weight );
}
/////////////////////////////////////////////////////////////////////////////////////////
std::vector < std::vector < Bit > > * HuffmanCode::create_static_code
(
	HuffmanTreeNode * root,
	unsigned size_alphabet
)
{
	std::vector < std::vector < Bit > > * code = 
		new std::vector < std::vector < Bit > > ( size_alphabet );
	std::vector <Bit> current_code;

	lcr_huffman_tree(root, code, current_code);

	return code;
}
/////////////////////////////////////////////////////////////////////////////////////////
void HuffmanCode::lcr_huffman_tree
(
	HuffmanTreeNode * point,
	std::vector < std::vector < Bit > > * code,
	std::vector <Bit> & current_code
)
{
	if
		(
			   point->left  == NULL 
			&& point->right == NULL
		)
		{
			( *code )[ *(point->data) ] = current_code;
			return;
		}

	if
		(
			point->left != NULL
		)
		{
			current_code.push_back ( false );
			lcr_huffman_tree ( point->left, code, current_code );
			current_code.pop_back ();
		}

	if
		(
			point->right != NULL
		)
		{
			current_code.push_back ( true );
			lcr_huffman_tree ( point->right, code, current_code );
			current_code.pop_back ();
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void HuffmanCode::delete_huffman_tree
(
	HuffmanTreeNode * point
)
{
	if 
		(
			   point->left  == NULL
			&& point->right == NULL
		)
		{
			delete point->data;
			return;
		}

	if
		(
			point->left != NULL
		)
		{
			delete_huffman_tree ( point->left );
		}

	if
		(
			point->right != NULL
		)
		{
			delete_huffman_tree ( point->right );
		}

	delete point;
}
/////////////////////////////////////////////////////////////////////////////////////////
