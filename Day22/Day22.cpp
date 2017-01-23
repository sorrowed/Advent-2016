/*
 * Day22.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: tom
 */
#include "Day22.h"
#include "Common.h"
#include <regex>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

/*
 To do this, you'd like to count the number of viable pairs of nodes. A viable pair is any two nodes (A,B), regardless of whether they are directly connected, such that:

 Node A is not empty (its Used is not zero).
 Nodes A and B are not the same node.
 The data on node A (its Used) would fit on node B (its Avail).

 How many viable pairs of nodes are there?
 */
struct Node
{
	int x;
	int y;

	int size;
	int used;
	int avail() const
	{
		return size - used;
	}

	bool operator==( const Node& n ) const
	{
		return x == n.x && y == n.y;
	}

	bool operator!=( const Node& n ) const
	{
		return !(*this == n);
	}

	bool isViable( const Node& n ) const
	{
		return *this != n && used != 0 && n.avail() >= used;
	}

	static Node fromString( const string& src )
	{
		#define match_node	"x([[:digit:]]+)-y([[:digit:]]+)"
		#define match_size	"[[:space:]]*([[:digit:]]+)T"
		#define match_perc	"[[:space:]]*([[:digit:]]+)%"

		std::regex r( match_node match_size match_size match_size match_perc );

		std::smatch sm;
		std::regex_search (src,sm,r);

		Node n;

		stringstream x( sm[ 1 ] ); x >> n.x;
		stringstream y( sm[ 2 ] ); y >> n.y;
		stringstream s( sm[ 3 ] ); s >> n.size;
		stringstream u( sm[ 4 ] ); u >> n.used;

		return n;
	}
};

int Day22_Test( void )
{
	string src = "/dev/grid/node-x99-y100     91T   66T    25T   72%";

	Node n = Node::fromString( src );

	return -1;
}

int Day22_Part1( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day22/input.txt", &lines );

	vector<Node> nodes;
	for( const auto& line : lines ) {
		if( line[ 0 ] == '#' )
			continue;

		auto n = Node::fromString( line );
		nodes.push_back( n );

	}

	int count = 0;
	for( auto& node : nodes ) {
		count += std::count_if( nodes.begin(), nodes.end(), [ &node ](const Node& n) { return node.isViable( n ); } );
	}

	// Should yield 1045
	return count;
}

int Day22_Part2( int argc, char* argv[] )
{
	return -1;
}

