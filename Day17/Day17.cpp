/*
 * Day17.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: tom
 */
#include "Common.h"
#include "md5.h"

#include <string>
#include <array>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cassert>

using std::string;
using std::array;
using std::queue;

typedef array<uint8_t,16> digest_t;

bool is_open( uint8_t c ){
	return ( c & 0x0F ) > 0x0A && ( c & 0x0F ) <= 0x0F;
}

struct Location
{
	int x;
	int y;

	bool operator==( const Location& l ) const
	{
		return x == l.x && y == l.y;
	}

	bool operator!=( const Location& l ) const
	{
		return !( *this == l );
	}
};

typedef std::pair<string,Location> path_t;

struct Hash
{
	int operator()( const path_t& path ) const
	{
		return path.second.x << 8 || path.second.y;
	}
};

struct invalid_move : public std::exception
{

};

enum Direction
{
	Up = 0,
	Down,
	Left,
	Right
};

std::unordered_map<int, path_t> neighbors( const path_t& path )
{
	std::unordered_map<int, path_t> n;

	digest_t digest;
	md5( (const uint8_t*)path.first.c_str(), path.first.length(), digest.begin() );

	auto& current = path.second;
	if( current.y > 0 && is_open( digest[ 0 ] >> 4 ) )
		n[ Direction::Up ] = path_t { path.first + "U", Location{ current.x, current.y - 1 } };

	if( current.y < 3 && is_open( digest[ 0 ] & 0x0F ) )
		n[ Direction::Down ] = path_t { path.first + "D", Location{ current.x, current.y + 1 } };

	if( current.x > 0 && is_open( digest[ 1 ] >> 4 ) )
		n[ Direction::Left ] = path_t { path.first + "L", Location{ current.x - 1, current.y } };

	if( current.x < 3 && is_open( digest[ 1 ] & 0x0F ) )
		n[ Direction::Right ] = path_t { path.first + "R", Location{ current.x + 1, current.y } };

	return n;
}

void move( path_t* path, Direction d, std::unordered_map<int, path_t>& n )
{
	if( n.count( d ) == 0 )
		throw invalid_move();

	*path = n[ d ];
}

path_t BreadthFirst( const std::string& key, const Location& start, const Location& end, bool longest = false )
{
	std::unordered_map<path_t, path_t, Hash> came_from;

	std::queue<path_t> frontier;

	path_t s = std::make_pair( key, start );

	frontier.push( s );
	came_from[ s ] = s;

	path_t e;

	while( !frontier.empty() ) {
		auto current = frontier.front();
		frontier.pop();

		if( current.second == end )
		{
			if( !longest || current.first.length() > e.first.length() )
				e = current;

			if( !longest )
				break;
		}
		else
		{
			auto n = neighbors( current );
			for( auto next : n ) {

				frontier.push( next.second );
				came_from[ next.second ] = current;
			}
		}
	}

	return e;
}

int Day17_Test( void )
{
	path_t path = { "hijkl", { 0, 0 } };
	auto n = neighbors( path );
	move( &path, Direction::Down, n );
	n = neighbors( path );
	move( &path, Direction::Up, n );
	n = neighbors( path );
	move( &path, Direction::Right, n );
	n = neighbors( path );

	auto p1 = BreadthFirst( "ihgpwlah", Location { 0, 0 }, Location { 3, 3 } );
	assert( p1.first == "ihgpwlahDDRRRD" );

	auto p2 = BreadthFirst( "kglvqrro", Location { 0, 0 }, Location { 3, 3 } );
	assert( p2.first == "kglvqrroDDUDRLRRUDRD" );

	auto p3 = BreadthFirst( "ulqzkmiv", Location { 0, 0 }, Location { 3, 3 } );
	assert( p3.first == "ulqzkmivDRURDRUDDLLDLUURRDULRLDUUDDDRR" );

	//auto p4 = BreadthFirst( "ihgpwlah", Location { 0, 0 }, Location { 3, 3 }, true );
	//assert( p4.first.length() - 8 == 370 );


	return -1;
}

int Day17_Part1( int argc, char* argv[] )
{
	const string key = "pxxbnzuo";

	auto path = BreadthFirst( key, Location { 0, 0 }, Location { 3, 3 } );

	std::cout << "Reached vault after " << path.first.substr( key.length() ) << '\n';

	// Should yield RDULRDDRRD
	return path.first.length();
}

int Day17_Part2( int argc, char* argv[] )
{
	const string key = "pxxbnzuo";

	auto path = BreadthFirst( key, Location { 0, 0 }, Location { 3, 3 }, true );

	std::cout << "Reached vault after longest path of " << path.first.length() - key.length() << " steps \n";

	// Should yield 752

	return path.first.length();
}


