/*
 * Day16.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: tom
 */
#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;
using std::cout;

void enlarge( vector<bool>* a )
{
	auto b = *a;
	std::reverse( b.begin(), b.end() );

	for( vector<bool>::size_type i = 0; i < b.size(); ++i )
		b[i] = !b[i];

	a->push_back( 0 );
	a->insert( a->end(), b.begin(), b.end());
}

vector<bool> checksum( const vector<bool>& src )
{
	vector<bool> r;

	for( vector<bool>::size_type i = 0; i < src.size(); i += 2 ){
		r.push_back( src[i] == src[i + 1] );
	}

	return r.size() % 2 == 0 ? checksum( r ) : r;
}

int Day16_Test( void )
{
	vector<bool> k1 = { 1,1,1,1,1 };
	enlarge(&k1);

	vector<bool> k2 = { 1,1,1,1,0,0,0,0,1,0,1,0 };
	enlarge(&k2);

	vector<bool> k3 = { 1,0,0,0,0 };
	while( k3.size() < 20 )
		enlarge( &k3 );
	k3.resize( 20 );

	auto k4 = checksum( k3 );

	return -1;
}

std::ostream& operator<<( std::ostream& str, const vector<bool>& src )
{
	for( auto v : src )
		str << ( v ? '1' : '0' );

	return str;
}

int Day16_Part1( int argc, char* argv[] )
{
	vector<bool> input = { 1,1,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0 };
	while( input.size() < 272 )
		enlarge( &input );
	input.resize( 272 );

	auto chk = checksum( input );

	std::cout << "Checksum for part 1: " << chk << '\n';

	// Should yield 10100011010101011

	return 0;
}

int Day16_Part2( int argc, char* argv[] )
{
	vector<bool> input = { 1,1,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0 };
	while( input.size() < 35651584 )
		enlarge( &input );
	input.resize( 35651584 );

	auto chk = checksum( input );

	std::cout << "Checksum for part 2: " << chk << '\n';

	// Should yield 01010001101011001

	return 0;
}


