/*
 * Day14.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: tom
 */
#include "md5.h"

#include <map>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <cstring>

using std::map;
using std::string;
using std::stringstream;
using std::array;
using std::vector;

typedef array<uint8_t,16> digest_t;

static inline
digest_t hash( const string& salt, int index, int stretch, map<int,digest_t>& pre )
{
	if( pre.count( index ) > 0 )
		return pre[ index ];

	char buffer[ 33 ] = { 0 };
	snprintf( buffer, 33, "%s%d", salt.c_str(), index );

	digest_t digest;

	for( int i = 0; i < stretch; ++i ){

		md5((uint8_t*) buffer, strlen(buffer), digest.begin());

		snprintf( buffer, 33, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				(int) digest[0], (int) digest[1], (int) digest[2], (int) digest[3],
				(int) digest[4], (int) digest[5], (int) digest[6], (int) digest[7],
				(int) digest[8], (int) digest[9], (int) digest[10], (int) digest[11],
				(int) digest[12], (int) digest[13], (int) digest[14], (int) digest[15] );
	}

	return pre[ index ] = digest;
}

static inline
uint8_t high_nibble( uint8_t v )
{
	return v >> 4;
}

static inline
uint8_t low_nibble( uint8_t v )
{
	return v & 0x0F;
}


static inline
uint8_t has_triplet( const digest_t& digest )
{
	uint8_t r = 0xFF;

	for( int i = 0; r == 0xFF && i < 16 - 1; ++i ){
		if( high_nibble( digest[i] ) == low_nibble( digest[i] ) &&
				high_nibble( digest[i + 1] ) == low_nibble( digest[i] ) ){
			r = low_nibble( digest[i] );
		}
		else
		if( low_nibble( digest[i] ) == high_nibble( digest[i + 1] ) &&
				low_nibble( digest[i] ) == low_nibble( digest[i + 1] ) ){
			r = low_nibble( digest[i] );
		}
	}
	return r;
}

static inline
bool has_quintet( const digest_t& digest, uint8_t c )
{
	bool r = false;

	for( int i = 0; !r && i < 16 - 2; ++i ){

		r = ( c == high_nibble( digest[i] ) &&
			c == low_nibble( digest[i] ) &&
			c == high_nibble( digest[i+1] ) &&
			c == low_nibble( digest[i+1] ) &&
			c == high_nibble( digest[i+2] ) ) ||

			( c == low_nibble( digest[i] ) &&
			c == high_nibble( digest[i+1] ) &&
			c == low_nibble( digest[i+1] ) &&
			c == high_nibble( digest[i+2] ) &&
			c == low_nibble( digest[i+2] ) );

	}
	return r;
}

int HashOrama( vector<digest_t>& hashes, const string& salt, size_t count, int stretch )
{
	map<int, digest_t> pre;

	int index = 0;
	while( hashes.size() < count ){
		auto h = hash( salt, index, stretch, pre );

		auto c = has_triplet( h );

		if( c != 0xFF ){

			for( int i = 0; i < 1000; ++i ){
				auto s = hash( salt, index + 1 + i, stretch, pre );

				if( has_quintet( s, c ) ){
					hashes.push_back( h );
					break;
				}
			}
		}

		++index;
	}

	return index - 1;
}

int Day14_Test( void )
{
	vector<digest_t> hashes;

	int r = HashOrama( hashes, "abc", 64, 1 );


	// Should yield 22728
	return r;
}

int Day14_Part1( int argc, char* argv[] )
{
	vector<digest_t> hashes;

	int r = HashOrama( hashes, "jlmsuwbz", 64, 1 );

	// Should yield 35186
	return r;
}

int Day14_Part2( int argc, char* argv[] )
{
	//pre.clear();
	//volatile digest_t stretch = hash( salt, 0, 2017, pre ); // should yield a107ff634856bb300138cac6568c0f24

	vector<digest_t> hashes;

	int r = HashOrama( hashes, "jlmsuwbz", 64, 2017 );

	// Should yield 22429
	return r;
}
