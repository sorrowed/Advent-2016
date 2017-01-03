/*
 * Day18.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: tom
 */
#include "Day18.h"

#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

struct Tile
{
	bool trap;

	static const Tile Safe;

	static Tile fromTiles( const Tile& left, const Tile& center, const Tile& right )
	{
		bool trap = ( left.trap && center.trap && !right.trap ) ||
					( !left.trap && center.trap && right.trap ) ||
					( left.trap && !center.trap && !right.trap ) ||
					( !left.trap && !center.trap && right.trap );

		return Tile { trap };
	}
};

const Tile Tile::Safe { false };

struct Row
{
	vector<Tile> tiles;

	int length() const {
		return tiles.size();
	}

	void add( const Tile& tile )
	{
		tiles.push_back( tile );
	}

	static Row fromRow( const Row& row )
	{
		Row r;

		r.add( Tile::fromTiles( Tile::Safe, row.tiles[ 0 ], row.tiles[ 1 ] ) );
		for( int t = 1; t < row.length() - 1; ++t )
			r.add( Tile::fromTiles( row.tiles[ t - 1 ], row.tiles[ t ], row.tiles[ t + 1 ] ) );
		r.add( Tile::fromTiles( row.tiles[ row.length() - 2 ], row.tiles[ row.length() - 1 ], Tile::Safe ) );

		return r;
	}

	static Row fromString( const string& src )
	{
		Row r;

		for( char c : src )
		{
			if( c == '.' )
				r.add( Tile { false } );
			else
			if( c == '^' )
				r.add( Tile { true } );
		}

		return r;
	}
};

int Day18_Test( void )
{
	const string input = ".^^.^.^^^^";

	vector<Row> rows;

	rows.push_back( Row::fromString( input ) );
	while( rows.size() != 10 )
		rows.push_back( Row::fromRow( rows[ rows.size() - 1 ] ));

	int count = 0;
	for( const auto& row : rows )
		count += std::count_if( row.tiles.begin(), row.tiles.end(), []( const Tile& tile ){ return !tile.trap; } );

	return count;
}

int Day18_Part1( int argc, char* argv[] )
{
	const string input = ".^.^..^......^^^^^...^^^...^...^....^^.^...^.^^^^....^...^^.^^^...^^^^.^^.^.^^..^.^^^..^^^^^^.^^^..^";

	vector<Row> rows;

	rows.push_back( Row::fromString( input ) );
	while( rows.size() != 40 )
		rows.push_back( Row::fromRow( rows[ rows.size() - 1 ] ));

	int count = 0;
	for( const auto& row : rows )
		count += std::count_if( row.tiles.begin(), row.tiles.end(), []( const Tile& tile ){ return !tile.trap; } );

	// Should yield 1987
	return count;
}

int Day18_Part2( int argc, char* argv[] )
{
	const string input = ".^.^..^......^^^^^...^^^...^...^....^^.^...^.^^^^....^...^^.^^^...^^^^.^^.^.^^..^.^^^..^^^^^^.^^^..^";

	vector<Row> rows;

	rows.push_back( Row::fromString( input ) );
	while( rows.size() != 400000 )
		rows.push_back( Row::fromRow( rows[ rows.size() - 1 ] ));

	int count = 0;
	for( const auto& row : rows )
		count += std::count_if( row.tiles.begin(), row.tiles.end(), []( const Tile& tile ){ return !tile.trap; } );

	// Should yield 19984714
	return count;
}


