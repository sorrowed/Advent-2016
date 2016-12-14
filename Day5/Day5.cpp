/*
 * Day5.cpp
 *
 *  Created on: Dec 5, 2016
 *      Author: tom
 */
#include "Day5.h"

#include "md5.h"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cinttypes>


static inline
bool StartsWithZeros( unsigned char hash[ 16 ] )
{
	return !hash[ 0 ] && !hash[ 1 ] && !(hash[ 2 ] >> 4);
}

static inline
void CalculateHash( uint8_t dst[ 16 ], const char* input, int number )
{
	char buffer[ 128 ] = { 0 };

	sprintf( buffer, "%s%d", input, number );

	md5( (uint8_t*) buffer, strlen( buffer ), dst );
}

int Day5_Part1( int argc, char* argv[] )
{
	// "abc" should yield 18f47a30
	// "ugkcyxxp" should yield d4cd2ee1
	unsigned char hash[ 16 ] = { 0 };

	int number = 0, position = 0;
	char password[ 8 ] = { 0 };

	for( number = 0; position < 8; ++number ) {

		CalculateHash( hash, "ugkcyxxp", number );

		if( StartsWithZeros( hash ) ) {
			password[ position++ ] = hash[ 2 ] & 0x0F;
		}
	}

	return -1;
}

int Day5_Part2( int argc, char* argv[] )
{
	// "ugkcyxxp" should yield f2c730e5
	// "abc" should yield 05ace8e3
	unsigned char hash[ 16 ] = { 0 };

	int number = 0, count = 0;
	char password[ 8 ] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	for( number = 0; count < 8; ++number ) {

		CalculateHash( hash, "ugkcyxxp", number );

		if( StartsWithZeros( hash ) ) {

			int p = (hash[ 2 ] & 0x0F);

			if( p >= 0 && p < 8 && password[ p ] == -1 ) {
				password[ p ] = (hash[ 3 ] >> 4);
				++count;
			}
		}
	}

	return -1;
}

int Day5_Test( void )
{
	const char* input[] = { "abc3231929", "abc5017308", "abc5278568" };

	unsigned char hash[ 16 ] = { 0 };

	md5( (uint8_t*) input[ 0 ], strlen( input[ 0 ] ), hash );
	assert( (hash[ 2 ] & 0x0F) == 0x01 );

	md5( (uint8_t*) input[ 1 ], strlen( input[ 1 ] ), hash );
	assert( (hash[ 2 ] & 0x0F) == 0x08 );

	md5( (uint8_t*) input[ 2 ], strlen( input[ 2 ] ), hash );
	assert( (hash[ 2 ] & 0x0F) == 0x0F );

	return -1;
}
