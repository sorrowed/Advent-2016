/*
 * Day13.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: tom
 */
#include "cinttypes"
#include <map>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <cassert>

static inline uint64_t Demo( int x, int y )
{
	return 10 + (x * x) + (3 * x) + (2 * x * y) + y + (y * y);
}

static inline uint64_t Own( int x, int y )
{
	return 1358 + (x * x) + (3 * x) + (2 * x * y) + y + (y * y);
}

static inline
bool IsOpenSpace( int x, int y, uint64_t (*fn)( int, int ) )
{
	auto code = fn( x, y );

	int count = __builtin_popcount( code );

	return count % 2 == 0;
}

typedef std::tuple<int, int> Location;
typedef std::tuple<int, int, int> LocationEx;

std::map<Location, std::vector<Location>> neighbors;

void CreateNeighbors( int width, int height, uint64_t (*fn)( int, int ) )
{
	neighbors.clear();

	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {

			auto l = std::make_tuple( x, y );
			if( x < width - 1 && IsOpenSpace( x + 1, y, fn ) )
				neighbors[ l ].push_back( std::make_tuple( x + 1, y ) );

			if( y < height - 1 && IsOpenSpace( x, y + 1, fn ) )
				neighbors[ l ].push_back( std::make_tuple( x, y + 1 ) );

			if( x > 0 && IsOpenSpace( x - 1, y, fn ) )
				neighbors[ l ].push_back( std::make_tuple( x - 1, y ) );

			if( y > 0 && IsOpenSpace( x, y - 1, fn ) )
				neighbors[ l ].push_back( std::make_tuple( x, y - 1 ) );
		}
	}
}

class Hash
{
public:
	size_t operator()( const Location& l ) const
	{
		return std::get<0>( l ) << 16 || std::get<1>( l );
	}
};

class HashEx
{
public:
	size_t operator()( const LocationEx& l ) const
	{
		return std::get<0>( l ) << 16 || std::get<1>( l );
	}
};

/*
 * Very much obliged : http://www.redblobgames.com/pathfinding/a-star/implementation.html
 */
std::vector<Location> BreadthFirst( const Location& start, const Location& end,
	std::map<Location, std::vector<Location>>& neighbors )
{
	std::queue<Location> frontier;
	frontier.push( start );

	std::unordered_map<Location, Location, Hash> came_from;
	came_from[ start ] = start;

	while( !frontier.empty() ) {
		auto current = frontier.front();
		frontier.pop();

		if( current == end )
			break;

		for( auto next : neighbors[ current ] ) {

			if( !came_from.count( next ) ) {
				frontier.push( next );

				came_from[ next ] = current;
			}
		}
	}

	std::vector<Location> path;
	auto current = end;
	path.push_back( current );
	while( current != start ) {
		current = came_from[ current ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );

	return path;
}

int Day13_Test( void )
{
	CreateNeighbors( 10, 10, Demo );

	std::vector<Location> path = BreadthFirst( std::make_tuple( 1, 1 ), std::make_tuple( 7, 4 ), neighbors );

	assert( path.size() - 1 == 11 );

	return path.size() - 1;
}

int Day13_Part1( int argc, char* argv[] )
{
	CreateNeighbors( 50, 50, Own );

	std::vector<Location> path = BreadthFirst( std::make_tuple( 1, 1 ), std::make_tuple( 31, 39 ), neighbors );

	return path.size() - 1; // Should yield 96
}

void Traverse( std::unordered_set<LocationEx, HashEx>& unique, const Location& start,
	std::map<Location, std::vector<Location>>& neighbors, int maxdepth )
{
	if( maxdepth == 0 )
		return;

	--maxdepth;

	for( auto& l : neighbors[ start ] ) {

		auto le = LocationEx { std::get<0>( l ), std::get<1>( l ), maxdepth };

		if( unique.count( le ) != 0 )
			continue;

		unique.insert( le );

		Traverse( unique, l, neighbors, maxdepth );
	}
}

#include <iostream>
void Print( int w, int h, std::unordered_set<LocationEx, HashEx>& marked )
{
	for( int y = 0; y < h; ++y ) {
		for( int x = 0; x < w; ++x ) {

			auto le = LocationEx { x, y, 0 };

			if( marked.count( le ) > 0 )
				std::cout << std::get<2>( *marked.find( le ) );
			else
			if( IsOpenSpace( x, y, Own ) )
				std::cout << ' ';
			else
				std::cout << 'X';
		}
		std::cout << '\n';
	}
}

int Day13_Part2( int argc, char* argv[] )
{
	CreateNeighbors( 200, 200, Own );

	auto start = LocationEx { 1, 1, 0 };
	std::unordered_set<LocationEx, HashEx> unique;

	unique.insert( start );
	Traverse( unique, Location { std::get<0>(start), std::get<1>(start) }, neighbors, 50 );

	Print( 50, 50, unique );

	return -1;
}

