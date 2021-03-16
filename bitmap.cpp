#include "bitmap.h"
/////////////////////////////////////////////////////////////////////////////////////////
BitMap::BitMap
(
	std::ifstream &  data_input_file,
	std::ofstream & data_output_file
) 
:   input_file  (  data_input_file ), 
    output_file ( data_output_file )
{
	obuffer = 0;
 	omask   = 128;

 	ibuffer = 0;
 	imask   = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
Bit BitMap::getbit()
{
	if 
		(
			(imask >>= 1) == 0
		) 
 		{
  			input_file.read( (char * ) & ibuffer, sizeof ( char ) );
			imask = 128;
 		}

 	return ( ibuffer & imask ) != 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
char BitMap::getchar()
{
	char result = 0;

 	for 
 		(
 			char count = 0; 
 			count < 8; 
 			++count
 		)
  		{
  			result |= getbit() << count;
  		}

	return result;
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned BitMap::get_unsigned()
{
	unsigned data;
	void * point = & data;

	for
		(
			unsigned char count = 0; 
			count < 3;
			++count
		)
		{
			( * ( ( (char * ) point ) + count ) ) = getchar();
		}

	( * ( ( (char * ) point ) + 3 ) ) = ( char ) 0;

	return data;
}
/////////////////////////////////////////////////////////////////////////////////////////
void BitMap::putbit
(
	Bit bit
)
{
	if 
		(
			bit
		) 
		{
			obuffer |= omask;
		}

	if 
		(
			(omask >>= 1) == 0
		) 
		{
			output_file.write ( (char * ) & obuffer, sizeof( char ) );
			obuffer = 0;
			omask   = (char) 128;
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void BitMap::putbits 
(
	const std::vector < Bit > & data
)
{
	for
		(
			unsigned count =  0;
			count < data.size();
			++count
		)
		{
			putbit( data[count] );
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void BitMap::putchar
(
	char value
)
{
    char mask = 1;

	for 
		(
			char count = 0; 
			count < 8; 
			count++
		)
 		{
 			putbit( (value & mask) != 0 );
  			mask <<= 1;
 		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void BitMap::put_unsigned
(
	unsigned  data
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
			putchar ( * ( ( (char * ) point ) + count ) );
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
void BitMap::flush()
{
	for
		(
			int count = 0;
			count     < 7;
			++count
		)
		{
			putbit ( false );
		}
}
/////////////////////////////////////////////////////////////////////////////////////////
