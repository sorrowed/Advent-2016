/*
 * Day24.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: tom
 */
#include "Day24.h"
#include <map>
#include <vector>

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
};

int hash( int x, int y )
{
	return ( x << 16 ) | y;
}

std::vector<Location> neighbors( const Location& l, const std::map<int,Location>& locations ){

	std::vector<Location> r;

	int h;

	if( l.x > 0 ) {
		h = hash( l.x - 1, l.y );
		if( locations.count( h ) ) {
			r.push_back( locations.at( h ) );
		}
	}

	if( l.y > 0 ) {
		h = hash( l.x, l.y - 1 );
		if( locations.count( h ) ) {
			r.push_back( locations.at( h ) );
		}
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

std::map<int,Location> parse( const char* lines[] ){
	std::map<int,Location> locations;

	int x = 0, y = 0;

	const char* line = lines[ 0 ];

	while( line ) {

		x = 0;
		char c = line[ x ];
		while( c ) {
			if( c != '#' ){

				locations[ hash( x, y ) ] = Location { x, y, c };
			}

			c = line[ ++x ];
		}

		++y;

		++line;
	}

	return locations;
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

	return -1;
}

int Day24_Part1( int argc, char* argv[] )
{
	const char * lines [] =
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
	return -1;
}

int Day24_Part2( int argc, char* argv[] )
{
	return -1;
}




