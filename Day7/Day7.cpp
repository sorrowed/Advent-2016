/*
 * Day7.cpp
 *
 *  Created on: Dec 7, 2016
 *      Author: tom
 */
#include "Common.h"
#include <string>
#include <algorithm>
#include <cassert>

bool SupportsTLS( const std::string& ip );
bool SupportsSSL( const std::string& ip );

static inline
bool HasBrackets( const std::string& s ){

	return s.find_first_of( "[]" ) != std::string::npos;
}

bool SupportsTLS( const std::string& ip )
{
	bool abba = false;
	bool in_brackets = false;

	for( std::string::size_type i = 0; i < ip.length() - 3; ++i ) {
		auto pair1 = ip.substr( i, 2 );
		if( pair1[ 0 ] == '[' ) {
			in_brackets = true;
		} else if( pair1[ 0 ] == ']' ) {
			in_brackets = false;
		}

		if( HasBrackets( pair1 ) )
			continue;

		auto pair2 = ip.substr( i + 2, 2 );

		if( HasBrackets( pair2 ) )
			continue;

		if( pair1[ 0 ] == pair2[ 1 ] && pair1[ 1 ] == pair2[ 0 ] && pair1[ 0 ] != pair1[ 1 ] ) {
			abba = !in_brackets;
			if( in_brackets )
				break;
		}

	}
	return abba;
}

bool SupportsSSL( const std::string& ip )
{
	bool in_brackets = false;

	std::vector<std::string> inside;
	std::vector<std::string> outside;

	for( std::string::size_type i = 0; i < ip.length() - 2; ++i ) {
		auto triplet = ip.substr( i, 3 );
		if( triplet[ 0 ] == '[' ) {
			in_brackets = true;
		} else if( triplet[ 0 ] == ']' ) {
			in_brackets = false;
		}

		if( HasBrackets( triplet ) )
			continue;

		if( triplet[ 0 ] != triplet[ 1 ] && triplet[ 0 ] == triplet[ 2 ] ) {
			if( in_brackets ) {
				inside.push_back( triplet );
			} else {
				outside.push_back( triplet );
			}
		}
	}

	bool ssl = false;

	for( auto t : inside ) {
		ssl |= (std::find_if( outside.begin(), outside.end(),
			[&t]( const std::string& s ) {return t[0] == s[1] && t[1] == s[0];} ) != outside.end());

	}
	return ssl;
}

int Day7_Part1( int argc, char* argv[] )
{
	std::vector<std::string> addresses;
	Read( "input.txt", &addresses );

	int count = std::count_if( addresses.begin(), addresses.end(), SupportsTLS );
	//Should be 110

	return count;
}

int Day7_Part2( int argc, char* argv[] )
{
	std::vector<std::string> addresses;
	Read( "input.txt", &addresses );

	int count = std::count_if( addresses.begin(), addresses.end(), SupportsSSL );

	//Should be 242

	return count;
}

int Day7_Test( void )
{
	assert( SupportsTLS( "abba[mnop]qrst" ) );
	assert( !SupportsTLS( "abcd[bddb]xyyx" ) );
	assert( !SupportsTLS( "aaaa[qwer]tyui" ) );
	assert( SupportsTLS( "ioxxoj[asdfgh]zxcvbn" ) );

	assert( SupportsSSL( "aba[bab]xyz" ) );
	assert( !SupportsSSL( "xyx[xyx]xyx" ) );
	assert( SupportsSSL( "aaa[kek]eke" ) );
	assert( SupportsSSL( "zazbz[bzb]cdb" ) );

	return -1;
}
