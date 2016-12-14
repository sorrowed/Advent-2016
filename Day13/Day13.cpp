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

	int count =  __builtin_popcount( code );

	return count % 2 == 0;
}

typedef std::tuple<int, int> Location;

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

/*
 * Very much obliged : http://www.redblobgames.com/pathfinding/a-star/implementation.html
 */
void BreadthFirst( std::vector<Location>& path, const Location& start, const Location& end )
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

				//++std::get<2>( current );

				came_from[ next ] = current;
			}
		}
	}

	auto current = end;
	path.push_back( current );
	while( current != start ) {
		current = came_from[ current ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );
}

int Day13_Test( void )
{
	CreateNeighbors( 10, 10, Demo );

	std::vector<Location> path;

	BreadthFirst( path, std::make_tuple( 1, 1 ), std::make_tuple( 7, 4 ) );

	assert( path.size() - 1 == 11 );

	return path.size() - 1;
}

int Day13_Part1( int argc, char* argv[] )
{
	CreateNeighbors( 50, 50, Own );

	std::vector<Location> path;

	BreadthFirst( path, std::make_tuple( 1, 1 ), std::make_tuple( 31, 39 ) );

	return path.size() - 1; // Should yield 96
}

void Traverse( std::unordered_set<Location,Hash>& unique, const std::vector<Location>& locations, int maxdepth )
{
	if( maxdepth == 0 )
		return;

	--maxdepth;

	for( auto& l : locations )
	{
		if( unique.count( l ) != 0 )
			continue;

		unique.insert( l );

		Traverse( unique, neighbors[ l ], maxdepth );
	}
}

int Day13_Part2( int argc, char* argv[] )
{
	CreateNeighbors( 100, 100, Own );

	auto start = std::make_tuple( 1, 1 );
	std::unordered_set<Location, Hash> unique;

	unique.insert( start );
	Traverse( unique, neighbors[ start ], 50 );

	return -1;
}


