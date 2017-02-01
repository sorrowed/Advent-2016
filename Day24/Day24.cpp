/*
 * Day24.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: tom
 */
#include "Day24.h"
#include <map>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <iostream>

#include "PriorityQueue.h"

using std::unordered_map;

struct Location
{
	int x;
	int y;

	char nr;

	bool operator==( const Location& l ) const
	{
		return x == l.x && y == l.y;
	}

	bool operator!=( const Location& l ) const
	{
		return !( *this == l );
	}

	bool operator<(const Location& l ) const
	{
		return x < l.x && y < l.y;
	}
};

int hash( int x, int y )
{
	return ( x << 16 ) | y;
}

struct Hash
{
	int operator()( const Location& l ) const
	{
		return hash( l.x, l.y );
	}
};

std::vector<Location> neighbors( const Location& l, const std::map<int,Location>& locations ){

	std::vector<Location> r;

	int h = hash( l.x - 1, l.y );
	if( locations.count( h ) ) {
		r.push_back( locations.at( h ) );
	}

	h = hash( l.x, l.y - 1 );
	if( locations.count( h ) ) {
		r.push_back( locations.at( h ) );
	}

	h = hash( l.x + 1, l.y );
	if( locations.count( h ) ) {
		r.push_back( locations.at( h ) );
	}

	h = hash( l.x, l.y + 1 );
	if( locations.count( h ) ) {
		r.push_back( locations.at( h ) );
	}

	return r;
}

double cost( const Location& current, const Location& next )
{
	return 1;
}

double heuristic( const Location& current, const Location& end )
{
	return std::abs( end.x - current.x ) + std::abs( end.y - current.y );
}

typedef unordered_map<Location, Location, Hash> location_map;
typedef unordered_map<Location, double, Hash> location_cost_map;

std::vector<Location> Astar( const Location& start, const Location& end, const std::map<int,Location>& locations )
{
	location_map came_from;
	location_cost_map cost_so_far;

	PriorityQueue<Location, double> frontier;
	frontier.put( start, 0 );

	came_from[ start ] = start;
	cost_so_far[ start ] = 0;

	while( !frontier.empty() ) {
		auto current = frontier.get();

		if( current == end ) {
			break;
		}

		auto n = neighbors( current, locations );
		for( auto next : n ) {
			double new_cost = cost_so_far[ current ] + cost( current, next );
			if( !cost_so_far.count( next ) || new_cost < cost_so_far[ next ] ) {
				cost_so_far[ next ] = new_cost;
				double priority = new_cost + heuristic( next, end );
				frontier.put( next, priority );
				came_from[ next ] = current;
			}
		}
	}

	vector<Location> path;
	Location current = end;
	path.push_back( current );
	while( current != start ) {
		current = came_from[ current ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );

	return path;
}


std::map<int,Location> parse( const char* lines[] ){
	std::map<int,Location> locations;

	int x = 0, y = 0;

	auto line = lines;

	while( *line ) {

		x = 0;
		char c = (*line)[ x ];
		while( c ) {
			if( c != '#' ){

				locations[ hash( x, y ) ] = Location { x, y, c };
			}

			c = (*line)[ ++x ];
		}

		++y;

		++line;
	}

	return locations;
}

std::vector<Location> find_numbers( std::map<int,Location>& locations )
{
	std::vector<Location> numbers;
	for( auto kvp : locations ){
		auto v = kvp.second;
		if( v.nr != '.' )
			numbers.push_back( v );
	}

	return numbers;
}

bool numbers_less( const Location& a, const Location& b )
{
	return a.nr < b.nr;
}

/*!
 * Just try all permutations.
 */
std::vector<std::vector<Location>> find_paths( std::map<int,Location>& locations, std::vector<Location>& numbers, const Location& start )
{
	std::sort( numbers.begin(), numbers.end(), numbers_less );

	std::vector<std::vector<Location>> paths;
	do
	{
		std::vector<Location> path = Astar( start, numbers[ 0 ], locations );
		path.erase( path.begin() );

		for( int i = 0; i < numbers.size() - 1; ++i )
		{
			auto piece = Astar( numbers[ i ], numbers[ i + 1 ], locations );

			path.insert( path.end(), piece.begin() + 1, piece.end() );
		}

		paths.push_back( path );
	}
	while( std::next_permutation( numbers.begin(), numbers.end(), numbers_less ) );

	return paths;
}

void Print( const char* lines[], const std::vector<Location>& path )
{
	int x = 0, y = 0;

	auto line = lines;

	while( *line ) {

		x = 0;
		char c = (*line)[ x ];
		while( c ) {

			char nr = 0;
			for( auto& l : path ){
				if( l.x == x && l.y == y )
					nr = 'X';
			}

			if( c >= '0' && c <= '9' )
				std::cout << c;
			else
			if( nr != 0  )
				std::cout << nr;
			else
				std::cout << c;
			c = (*line)[ ++x ];
		}

		++y;

		++line;
		std::cout << '\n';
	}
}

int Day24_Test( void )
{
	const char* lines[] = {
		"###########",
		"#0.1.....2#",
		"#.#######.#",
		"#4.......3#",
		"###########",
		nullptr
	};

	auto locations = parse( lines );

	auto numbers = find_numbers( locations );
	auto zerop = std::find_if( numbers.begin(), numbers.end(), []( const Location& l) { return l.nr == '0'; } );
	auto zero = *zerop;
	numbers.erase( zerop );

	auto paths = find_paths( locations, numbers, zero );

	return paths.size();
}

static const char * input [] =
{
	"###################################################################################################################################################################################",
	"#.....#.#.....#...#....4#.....#.#...#.........#...#...............#...................#...#.#...........#.#...........#.#.#.#.........#.#.......#...#...........#.....#...#7..#.#.#",
	"###.#.#.###.#.#.###.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.###.#.###.#.###.#.#.#.###.###.#.#####.###.#.#.###.#.#.#.#.#.#.#.#.#.#.#.#.###.#####.#.#.#.#.#####.#.#.#.###.#.#.#.#.#####.#.#.#.#",
	"#.#.....#.#.#...#.........#.....#.....#.......#.#.#.............#.#.#.#.....#.#.......#.....#.........#...#.......#.....#.#.#.............#...........#.#.....#.#.....#.......#.#.#",
	"#.#.#.#.#.#.#.#.#.#.#####.#####.###.###.#.###.#.###.###.#.#####.#.#.#.#.#.###.#.#.###.#.#.#.#.###.#########.###########.#.#.###.#.#.###.###.#.###.###.#.#.#####.#.###.#.#####.#.###",
	"#...........#...#...#.....#.....#...#.#...#.#.....#.........#...#...#.....#.....#.#.#...#...#...#...#.....#.......#...#...#...............#...#...#.............#.....#.#.....#...#",
	"###.#.#.###.#.#.#.#.###.#.###.#####.#.#.#.#.#.###.###.###.#.#.#.###.#.#.#.#.###.#.#.#.###.#####.#########.#.#.#.#.#.###.#.#.#.#.#####.#.#.#.#.###.#.#.#.#.#.#.#.#####.#.###.#.#.#.#",
	"#3#...#.#.#.#.........#...............#...#.#.....#...#.....#...#.......#...#.....#.#.#...#.....#...#.....#.#.#.....#.....#...........#.#.#.#.....#.#.........#.#...#.#.#.#...#...#",
	"#.###.###.#######.###.#.###.#.#.#.###.###.#######.###.#.#####.#####.#.#.#.#.#######.###.###.###.###.###.#.#########.#.#.#.#.#.#####.###.#.###.#.###.#.#####.###.###.###.#.#.#.###.#",
	"#.#...#.....#.#.............#.....#.#.....#.#.....#.#.#.....#.....#.......#.....#.................#...........#...#.#.....#...#.....#...#.......#.#.....#...#...#.#.#...#...#...#.#",
	"#.###.###.###.#.#.#.#####.#.###.#.#.###.#.#.#.#.#.#.#.#.#.#####.#####.#.#.#.#.#.#.###########.#.#.#.#.#.###.#.#.#.#.#.#.#.#.#.###.#.#.#####.#####.#.###.#.#.#.#.#.#.#####.#.###.#.#",
	"#.....#.......#.#.#.#.#...............#...#.#.#.#...#...........#.....#.#...#.................#...#.#.#...#.............#...#.........#...............#...#.#.#.....#.....#.....#.#",
	"#####.#.#######.#.###.#.#.#.#.###.#.#.#.###.###.###.#.#.#.#.#.#.###.#.#.#.#.#######.###.#.###.#.#.#.###.#.#.###.###.#.#.#.#.#####.#####.#.###.#####.###.#.#.#####.#.#.#####.#.#.#.#",
	"#.#...#.........#...#.#...#.......#...#.#.......#...#.#.........#.#.#...#.#.#.#.........#.#.#.......#...#...#...#.#...#.......#...#.....#...#...#.#...#...#...#...........#...#.#.#",
	"#.#####.#.###.#.#.#######.#.###.#.#.#.#########.#.#.#.#.#####.#.#.#######.#.#.###########.#.#########.###.#.#.#.#.###.#.#.###.#########.#.#.#.###.#.#.###.#.#.###.#####.#.###.#.#.#",
	"#.......#.......#...#.#.#...#...#.....#.#...#...#.#.#.#.#...#.....#.#...#...#.............#.......#.......#...#.#.............#.......#.....#...#...#.#.....#.............#...#.#.#",
	"#.#####.###.#####.#.#.#.#.#.#.#.#.#.#.#.#.###.###.#.###.###.#.#.###.#.#.#.#.###.#.###.#.#.#.#.#.#.#.#######.#.#.###.#.#.#.#.###.#.###.###.#####.#.#.#.#.#####.###.#.###.#####.###.#",
	"#..6#...#...#...#...#.#.....#...#.#.#...#...........#.#.#...#.#.#.....#.....#.#.#.....#.......#.................#.#.....#.#.........#...#...#...........#.#2....#.#.......#.#.#.#.#",
	"#.###.###.#.###.#####.#####.#.###.###.#.###.#.#####.#.#.#.#.#.#.###.#.#.#.#.#.#.#.#.#.#.###.#######.#.#.#.#.#####.#.#.#######.###.#####.###.#####.#####.#.#####.###.#######.###.###",
	"#.#.....#...#...#...........#.#.......#.#...#.#.............#...#...#.....#...#.....#.......#.......#.......#...#...#.......#...#.......#.#...#...#.........#...#...#...#.......#.#",
	"#.#.###.#.#.#.#.###.#######.#.#.###.###.#####.###.#.###.#######.#####.#####.#.#####.#.###.#.#.#.#.#####.###.#.#.#.#.#.#.#.#.#############.###.#.#.#.###.#.#.###.#.#.#.#.#####.#.#.#",
	"#...#.........#.....#...#.#...#.....#...#...#.......#.....#...#...#...#...#.............#.#...#.............#.....#...#.#.#.......#.....#.....#.....#...........#...#...#.....#...#",
	"#.#######.#.#.###.#.#.#.#.#.###.#.#.#.###.#.###.#.#.#.#####.#.#.#.#.#.#.#.#.#####.#####.#####.#.#######.###.#.#.###.#.###.#.#.#.#.#.###.#.#.###.#.#.#######.###.#.###.#.#.#.#.###.#",
	"#.....#.......#...#.#...#.....#...#.#...........#.....#.....#.#.#...#.....#.................#.........#.#.......#...........#...#...#.......#0#...#.....#.......#.#...........#...#",
	"#.#.#.#.#.###.#.#.#.###.###.#.#.###.#.#.#####.#######.#.#.#.#.#.###.###.###.#.#####.###.#####.#.#.###.###.###.###.#####.###.#.#.#.#.#.###.#.#.#.#.#.###.#.###.#.#.#.#.#.#.#####.###",
	"#.#.#...#...#.#.......#.............#...........................#.......#...........#.#...#...#.#...#.....#...#.#.#.#.#.#.......#.#...#...#...#...............#.......#.....#.....#",
	"#.#.###.#.#.#.#.#.#####.#.#####.#.#.###.#.#.#.#.#############.#.###.#.#.#.#.#####.#.#.###.#.###.#.#.#######.###.#.#.#.#.#.###.#.#####.#.###.###.#######.#.###.#####.#.#.#.#######.#",
	"#...#.......#.....#...#...#...#.....#5....#...#.......#.#.#...#...........#.#.......#.#...#.#.......#.#.#...#...#.....#.............#...#...#.....#.................#.....#.#...#.#",
	"#######.#.#.#######.#####.###.#.#.#######.#.#.#.#.#.#.#.#.#.###.#.###.#.#.#.###.###.#.#.#.###.#.###.#.#.###.#.###.#####.###.#######.#.#.#.#.#.#.#.#########.###.#.#.#.#.#.#.#.#.###",
	"#.#.........#...........#.........#.........#.#.#...........#...#.....#...................#...........#...#...#...#.#.......#...#.....#.#.#.....#.#.............#.........#.#...#.#",
	"#.#.#.###.#.###.#.###.#.###.#.#######.#.###.#.#.#.#########.#.###.#.#####.###.#.#.###.#.#.#.###.#.#####.###.#.###.#.#.###.#.#.#.#.#.#.#.#.###.#.#.###.#.#####.#.#.#######.#.#####.#",
	"#.........#.#.....#.....#...#...#.......#.....#.................#...#...#.....#...#...#.#.#.#...#...........#.#.....#.#.....#...#.#...#.......#.........#.....#.....#.......#...#.#",
	"#.#####.#.#.#.#.#.#.#####.###.###.#.#####.###.#####.###.#.#.#.#.#.###.#.#.#.#.#.#####.###.###.#.#.#.#.#.###.#.#.#.#.#.#.#.#####.#.#.#.#.#.#########.#.#.#.###.#.###.#.#.#.#.#.#.###",
	"#.......#...#...#.....#.#...#...#...#.#.............#.....#.............#.#.......#.......#...#...#...#.....#.......#...#...........#.#...#.#.......#...........#.#.....#.....#...#",
	"#.#.#.#.#.###.#.#.#.#.#.#.#.#.#.#.#.#.#.###.#.#.#####.#.###.#.#.#####.#.#.#.#####.#.#.###.###.#.#.#.#.#.#.#.#####.#.#.#####.###.###.###.###.#.#.#.#.#.#.#########.#####.#.#.#.#.#.#",
	"#.#.#.#.............#...#...#.#.....#...........#.........#...#.#.#...#.#.........#.........#.........#.....#.........#...#...#...#..1#.....#.#.#...#.#.....#...#...........#.....#",
	"###################################################################################################################################################################################",
	nullptr
};

int Day24_Part1( int argc, char* argv[] )
{
	auto locations = parse( input );

	auto numbers = find_numbers( locations );
	auto zerop = std::find_if( numbers.begin(), numbers.end(), []( const Location& l) { return l.nr == '0'; } );
	auto zero = *zerop;
	numbers.erase( zerop );

	auto paths = find_paths( locations, numbers, zero );

	int shortest = INT32_MAX;
	for( auto& p : paths )
		shortest = std::min( shortest, (int)p.size() );

	std::cout << "\nShortest path was " << shortest << " steps\n";

	// Should yield 460
	return shortest;
}

int Day24_Part2( int argc, char* argv[] )
{
	auto locations = parse( input );

	auto numbers = find_numbers( locations );
	auto zerop = std::find_if( numbers.begin(), numbers.end(), []( const Location& l) { return l.nr == '0'; } );
	auto zero = *zerop;
	numbers.erase( zerop );

	auto paths = find_paths( locations, numbers, zero );


	int shortest = INT32_MAX;
	for( auto& p : paths )
	{
		auto end = p[ p.size() - 1 ];

		auto piece = Astar( end, zero, locations );
		p.insert( p.end(), piece.begin() + 1, piece.end() );

		shortest = std::min( shortest, (int)p.size() );
	}

	std::cout << "\nShortest path was " << shortest << " steps\n";

	// Should yield 668
	return shortest;
}




