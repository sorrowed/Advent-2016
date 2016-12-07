/*
 * Day6.cpp
 *
 *  Created on: Dec 6, 2016
 *      Author: tom
 */
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <fstream>

static inline
bool Forward( const std::pair<char, int>& a, const std::pair<char, int>& b )
{
	return a.second > b.second;
}

static inline
bool Reverse( const std::pair<char, int>& a, const std::pair<char, int>& b )
{
	return a.second < b.second;
}

std::string Decode( std::vector<std::string> input, int len, bool reverse )
{
	std::vector<std::pair<char, int>> counts;

	std::string secret;

	for( int i = 0; i < len; ++i ) {

		counts.clear();

		for( auto s : input ) {
			auto it = std::find_if( counts.begin(), counts.end(),
				[s,i]( const std::pair<char, int>& p) {return p.first == s[ i ];} );

			if( it != counts.end() )
				++it->second;
			else
				counts.push_back( std::make_pair( s[ i ], 1 ) );
		}

		if( reverse )
			std::sort( counts.begin(), counts.end(), Reverse );
		else
			std::sort( counts.begin(), counts.end(), Forward );

		secret += counts[ 0 ].first;
	}

	return secret;
}

void Read( std::vector<std::string>* dst )
{
	dst->clear();

	std::ifstream file( "input.txt", std::ifstream::in );
	std::string line;

	std::getline( file, line );
	while( file.good() ) {
		dst->push_back( line );
		std::getline( file, line );
	}
}

int Day6_Part1( int argc, char* argv[] )
{
	std::vector<std::string> input;

	Read( &input );

	auto secret = Decode( input, 8, false );

	return -1;
}

int Day6_Part2( int argc, char* argv[] )
{
	std::vector<std::string> input;

	Read( &input );

	auto secret = Decode( input, 8, true );

	return -1;
}

int Day6_Test( void )
{
	std::vector<std::string> input = { "eedadn", "drvtee", "eandsr", "raavrd", "atevrs", "tsrnev", "sdttsa", "rasrtv",
		"nssdts", "ntnada", "svetve", "tesnvt", "vntsnd", "vrdear", "dvrsen", "enarar" };

	auto secret = Decode( input, 6, false );

	assert( secret == "easter" );

	return -1;
}

