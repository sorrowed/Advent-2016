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
#include <iostream>

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

struct Location
{
	int X;
	int Y;
	int L;

	Location() : Location( -1,-1,-1 ){}
	Location( int x, int y, int l )
	{
		X = x;
		Y = y;
		L = l;
	}

	Location( int x, int y )
	{
		X = x;
		Y = y;
		L = -1;
	}

	bool IsValid() const { return X != -1 && Y != -1; }


	static size_t Id( const Location& l ){
		return l.X << 16 | l.Y;
	}
};

bool operator==( const Location& a, const Location& b ) {
	return a.X == b.X && a.Y == b.Y;
}

bool operator!=( const Location& a, const Location& b ) {
	return !( a == b );
}

bool operator<( const Location& a, const Location& b ) {
	return Location::Id( a ) < Location::Id( b );
}

class Hash
{
public:
	size_t operator()( const Location& l ) const
	{
		return Location::Id( l );
	}
};

typedef std::map<Location, std::vector<Location>> neighbor_map;

neighbor_map CreateNeighbors( int width, int height, uint64_t (*fn)( int, int ) )
{
	neighbor_map neighbors;

	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {

			Location l ( x, y );
			if( x < width - 1 && IsOpenSpace( x + 1, y, fn ) )
				neighbors[ l ].push_back( Location( x + 1, y ) );

			if( y < height - 1 && IsOpenSpace( x, y + 1, fn ) )
				neighbors[ l ].push_back( Location( x, y + 1 ) );

			if( x > 0 && IsOpenSpace( x - 1, y, fn ) )
				neighbors[ l ].push_back( Location( x - 1, y ) );

			if( y > 0 && IsOpenSpace( x, y - 1, fn ) )
				neighbors[ l ].push_back( Location( x, y - 1 ) );
		}
	}

	return neighbors;
}

/*
 * Very much obliged : http://www.redblobgames.com/pathfinding/a-star/implementation.html
 */
std::vector<Location> BreadthFirst( const Location& start, const Location& end, neighbor_map& neighbors )
{
	std::unordered_map<Location, Location, Hash> came_from;

	Location s = start;
	Location e = end;

	s.L = 0;

	std::queue<Location> frontier;
	frontier.push( s );
	came_from[ s ] = s;

	while( !frontier.empty() ) {
		auto current = frontier.front();
		frontier.pop();

		if( current == e )
			break;

		for( auto next : neighbors[ current ] ) {
			next.L = current.L + 1;

			if( !came_from.count( next ) ) {

				frontier.push( next );
				came_from[ next ] = current;
			}
		}
	}

	std::vector<Location> path;
	auto current = e;
	path.push_back( current );
	while( current != s && current.IsValid() ) {
		current = came_from[ current ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );

	return path;
}

void Traverse( std::unordered_set<Location, Hash> locations, const Location& start, neighbor_map& neighbors, int max )
{
	for( auto next : neighbors[ start ] ) {
		next.L = start.L + 1;

		Traverse( locations, next, neighbors, max );
	}
}

void Print( int w, int h, std::vector<Location>& path )
{
	int count = 0;

	for( int y = 0; y < h; ++y ) {
		for( int x = 0; x < w; ++x ) {

			auto l = std::find_if( path.begin(), path.end(),
					[x,y]( const Location& i ) {return i.X == x && i.Y == y;} );

			if( l != path.end() ) {
				std::cout << std::hex << '0';//l->L;
				++count;
			}
			else if( IsOpenSpace( x, y, Own ) )
				std::cout << ' ';
			else
				std::cout << 'X';
		}
		std::cout << '\n';
	}

	std::cout << "Marked " << count << " unique locations\n";
}

int Day13_Test( void )
{
	auto neighbors = CreateNeighbors( 10, 10, Demo );
	auto path = BreadthFirst( Location{ 1, 1, 0 }, Location{ 7, 4, 0 }, neighbors );

	//Print( 10, 10, path );

	assert( path.size() - 1 == 11 );

	return path.size() - 1;
}

int Day13_Part1( int argc, char* argv[] )
{
	auto neighbors = CreateNeighbors( 50, 50, Own );
	auto path = BreadthFirst( Location{ 1, 1, 0 }, Location{ 31, 39, 0 }, neighbors );

	//Print( 50, 50, path );

	assert( path.size() - 1 == 96 );

	return path.size() - 1; // Should yield 96
}

int Day13_Part2( int argc, char* argv[] )
{
	auto neighbors = CreateNeighbors( 50, 50, Own );

	std::unordered_set<Location, Hash> locations;

	Traverse( locations, Location{ 1, 1, 0 }, neighbors, 50 );

	//Print( 50, 50, locations );

	return locations.size() - 1;
}

