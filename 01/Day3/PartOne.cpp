/*
 * PartOne.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: tom
 */

#include "PartOne.h"

class Triangle
{
public:
	int A;
	int B;
	int C;

	Triangle( int a, int b, int c ) :
		A( a ), B( b ), C( c )
	{

	}

	bool IsValid( void ) const
	{
		if( A + B <= C )
			return false;
		if( A + C <= B )
			return false;
		if( B + C <= A )
			return false;
		return true;
	}
};

#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

static std::vector<Triangle> triangles;

void Read1( void )
{
	std::ifstream file( "triangles.txt", std::ifstream::in );
	std::string line;

	std::getline( file, line );

	while( file.good() ){

		std::istringstream iss( line );

		int a, b, c;
		iss >> a >> b >> c;

		triangles.push_back( Triangle( a, b, c ) );

		std::getline( file, line );
	}
}

int Day3_Part1( int argc, char* argv[] )
{
	Read1();

	return std::count_if( triangles.begin(), triangles.end(), [](const Triangle& t){return t.IsValid();} );
}

void Read2( void )
{
	std::ifstream file( "/home/tom/Projects/eclipse-workspace/Advent-2016-01/01/Day3/triangles.txt", std::ifstream::in );
	std::string line1, line2, line3;

	std::getline( file, line1 );
	std::getline( file, line2 );
	std::getline( file, line3 );

	while( file.good() ){

		std::istringstream iss1( line1 ), iss2( line2 ), iss3( line3 );

		for( int i = 0; i < 3; ++i ){

			int a; iss1 >> a;
			int b; iss2 >> b;
			int c; iss3 >> c;

			triangles.push_back( Triangle( a, b, c ) );
		}

		std::getline( file, line1 );
		std::getline( file, line2 );
		std::getline( file, line3 );
	}
}

int Day3_Part2( int argc, char* argv[] )
{
	Read2();

	return std::count_if( triangles.begin(), triangles.end(), [](const Triangle& t){return t.IsValid();} );
}
