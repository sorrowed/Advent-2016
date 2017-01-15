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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <climits>
#include <queue>

using std::unordered_set;
using std::unordered_map;
using std::string;
using std::vector;
using std::stringstream;
using std::tuple;

/*
 To do this, you'd like to count the number of viable pairs of nodes. A viable pair is any two nodes (A,B), regardless of whether they are directly connected, such that:

 Node A is not empty (its Used is not zero).
 Nodes A and B are not the same node.
 The data on node A (its Used) would fit on node B (its Avail).

 How many viable pairs of nodes are there?
 */
int make_hash( int x, int y ){
	return x << 16 | y;
}

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

	bool fits( const Node& n ) const
	{
		return n.avail() >= used;
	}

	static Node fromString( const string& src )
	{
		#define match_node	"x([[:digit:]]+)-y([[:digit:]]+)"
		#define match_size	"[[:space:]]*([[:digit:]]+)T"
		#define match_perc	"[[:space:]]*([[:digit:]]+)%"

		std::regex r( match_node match_size match_size match_size match_perc );

		std::smatch sm;
		std::regex_search( src, sm, r );

		Node n;

		stringstream x( sm[1] );
		x >> n.x;
		stringstream y( sm[2] );
		y >> n.y;
		stringstream s( sm[3] );
		s >> n.size;
		stringstream u( sm[4] );
		u >> n.used;

		return n;
	}

	int Hash() const
	{
		return make_hash( x, y );
	}
};

bool is_viable( const Node& a, const Node& b )
{
	return a != b && a.used != 0 && b.avail() >= a.used;
}

typedef std::unordered_map<int, Node> node_map;
typedef std::vector<Node> neighbor_list;

neighbor_list neighbors( const Node& node, node_map& nodes, int x_size, int y_size )
{
	neighbor_list r;

	if( node.x > 0 ){
		auto& n = nodes[make_hash( node.x - 1, node.y )];
		if( n.fits( node ) )
			r.push_back( n );
	}

	if( node.y > 0 ){
		auto& n = nodes[make_hash( node.x, node.y - 1 )];
		if( n.fits( node ) )
			r.push_back( n );
	}

	if( node.x < x_size - 1 ){
		auto& n = nodes[make_hash( node.x + 1, node.y )];
		if( n.fits( node ) )
			r.push_back( n );
	}

	if( node.y < y_size - 1 ){
		auto& n = nodes[make_hash( node.x, node.y + 1 )];
		if( n.fits( node ) )
			r.push_back( n );
	}

	return r;
}

class Hash
{
public:
	int operator()( const Node& node )
	{
		return node.Hash();
	}
};
/*
 * Very much obliged : http://www.redblobgames.com/pathfinding/a-star/implementation.html
 */
std::vector<Node> BreadthFirst( const Node& start, const Node& end, node_map& nodes, int xMax, int yMax )
{
	std::unordered_map<int, Node> came_from;

	std::queue<Node> frontier;
	frontier.push( start );
	came_from[ start.Hash() ] = start;

	while( !frontier.empty() ) {
		auto current = frontier.front();
		frontier.pop();

		if( current == end )
			break;

		auto n = neighbors( current, nodes, xMax, yMax );
		for( auto next : n ) {

			if( !came_from.count( next.Hash() ) ) {

				frontier.push( next );
				came_from[ next.Hash() ] = current;
			}
		}
	}

	std::vector<Node> path;
	auto current = end;
	path.push_back( current );
	while( current != start ) {
		current = came_from[ current.Hash() ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );

	return path;
}

int Day22_Test( void )
{
	string src = "/dev/grid/node-x99-y100     91T   66T    25T   72%";

	Node n = Node::fromString( src );

	return -1;
}

int Day22_Part1( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "input.txt", &lines );

	vector<Node> nodes;
	for( const auto& line : lines ){
		if( line[0] == '#' )
			continue;

		auto n = Node::fromString( line );
		nodes.push_back( n );

	}

	int count = 0;
	for( auto& node : nodes ){
		count += std::count_if( nodes.begin(), nodes.end(), [ &node ](const Node& n){return is_viable( node, n );} );
	}

	// Should yield 1045
	return count;
}

int Day22_Part2( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "input.txt", &lines );

	node_map nodes;
	for( const auto& line : lines ){
		if( line[0] == '#' )
			continue;

		auto n = Node::fromString( line );
		nodes[ n.Hash() ] = n;

	}

	int xMax = INT_MIN;
	for( auto& n : nodes ){
		if( n.second.x > xMax ){
			xMax = n.second.x;
		}
	}
	return -1;
}

