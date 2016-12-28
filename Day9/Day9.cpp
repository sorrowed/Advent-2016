/*
 * Day9.cpp
 *
 *  Created on: Dec 9, 2016
 *      Author: tom
 */
#include "Common.h"

#include <string>
#include <sstream>
#include <vector>
#include <cassert>

using std::string;
using std::stringstream;
using std::vector;

string Read( stringstream& str, int len )
{
	std::vector<char> field( len );
	str.read( &field[ 0 ], len );

	return string( field.begin(), field.end() );
}

string Decompress1( const string& input )
{
	stringstream in( input );
	stringstream out;

	char s = in.get();

	while( in ) {
		if( s == '(' ) {
			stringstream marker;

			while( (s = in.get()) != ')' )
				marker << s;

			int len, count;
			marker >> len;
			marker.ignore( 1 );
			marker >> count;

			string field = Read( in, len );

			for( int i = 0; i < count; ++i )
				out << field;
		} else
			out << s;

		s = in.get();
	}

	return out.str();
}

std::string::size_type Decompress2( const string& input )
{
	std::string::size_type l = 0;

	if( input.find( "(" ) == std::string::npos )
		l = input.length();
	else {
		stringstream in( input );

		char s = in.get();

		while( in ) {
			if( s == '(' ) {
				stringstream marker;

				while( (s = in.get()) != ')' )
					marker << s;

				int len, count;
				marker >> len;
				marker.ignore( 1 );
				marker >> count;

				string field = Read( in, len );

				l += count * Decompress2( field );

			} else
				++l;

			s = in.get();
		}
	}

	return l;
}

int Day9_Part1( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day9/input.txt", &lines );

	int len = 0;
	for( auto& line : lines ) {
		auto s = Decompress1( line );
		len += s.length();
	}

	return len;
}

int Day9_Part2( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day9/input.txt", &lines );

	stringstream out;

	long len = 0;
	for( auto& line : lines )
		len += Decompress2( line );

	return len;
}

int Day9_Test( void )
{
	assert( Decompress1( "ADVENT" ) == "ADVENT" );

	assert( Decompress1( "A(1x5)BC" ) == "ABBBBBC" );
	assert( Decompress1( "(3x3)XYZ" ) == "XYZXYZXYZ" );
	assert( Decompress1( "A(2x2)BCD(2x2)EFG" ) == "ABCBCDEFEFG" );
	assert( Decompress1( "(6x1)(1x3)A" ) == "(1x3)A" );
	assert( Decompress1( "X(8x2)(3x3)ABCY" ) == "X(3x3)ABC(3x3)ABCY" );

	assert( Decompress2( "(3x3)XYZ" ) == string( "XYZXYZXYZ" ).length() );
	assert( Decompress2( "X(8x2)(3x3)ABCY" ) == string( "XABCABCABCABCABCABCY" ).length() );
	assert( Decompress2( "(27x12)(20x12)(13x14)(7x10)(1x12)A" ) == 241920L );
	assert( Decompress2( "(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN" ) == 445 );

	return -1;
}

