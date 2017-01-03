/*
 * Day20.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: tom
 */
#include "Day20.h"
#include "Common.h"

#include <sstream>
#include <deque>
#include <algorithm>

using std::string;
using std::vector;
using std::deque;

struct range_t
{
	long begin;
	long end;

	static range_t fromString( const string& src )
	{
		range_t r;

		std::stringstream str( src );
		str >> r.begin;
		str.ignore( 1 );
		str >> r.end;

		return r;
	}

	bool overlaps( const range_t& r ) const {
		return r.begin - 1 <= end && r.end + 1 >= begin;
	}

	bool meld( const range_t& r )
	{
		if( !overlaps( r ) )
			return false;

		begin = std::min( begin, r.begin );
		end = std::max( end, r.end );

		return true;
	}

	bool operator<( const range_t& r ) const
	{
		return begin < r.begin;
	}
};

typedef deque<range_t> rangelist_t;

void meld( rangelist_t& ranges )
{
	std::sort( ranges.begin(), ranges.end() );

	for( rangelist_t::size_type m = 0; m + 1 < ranges.size(); ++m ){
		while( m + 1 < ranges.size() && ranges[m + 1].meld( ranges[m] ) ){
			ranges.erase( ranges.begin() + m );
		}
	}
}

int Day20_Test( void )
{
	vector<string> lines;
	rangelist_t ranges;

	lines.push_back( "5-8" );
	lines.push_back( "0-2" );
	lines.push_back( "4-7" );

	for( const auto& line : lines )
		ranges.push_back( range_t::fromString( line ) );

	meld( ranges );

	return -1;
}

int Day20_Part1( int argc, char* argv[] )
{
	vector<string> lines;
	deque<range_t> ranges;

	Read( "input.txt", &lines );

	for( const auto& line : lines )
		ranges.push_back( range_t::fromString( line ) );

	meld( ranges );

	// Should be 22887907
	return ranges[ 0 ].end + 1;
}

int Day20_Part2( int argc, char* argv[] )
{
	vector<string> lines;
	rangelist_t ranges;

	Read( "input.txt", &lines );

	for( const auto& line : lines )
		ranges.push_back( range_t::fromString( line ) );

	meld( ranges );

	int count = 0;
	for( rangelist_t::size_type m = 0; m + 1 < ranges.size(); ++m ){

		auto& a = ranges[ m ];
		auto& b = ranges[ m + 1 ];

		count += b.begin - a.end - 1;
	}
	count += ( 4294967295 - ranges[ ranges.size() - 1 ].end );

	// Should yield 109
	return count;
}

