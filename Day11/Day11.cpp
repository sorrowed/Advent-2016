/*
 * Day11.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#include <array>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <utility>
#include <cassert>
#include <algorithm>
#include <cinttypes>
#include <iostream>

using std::array;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::ostream;
using std::queue;

class aoc_exception: public std::exception
{
public:
	std::string Message;

	aoc_exception( const std::string& msg ) :
			Message( msg )
	{

	}
};

enum item_type
{
	Generator = 0x01,
	MicroChip = 0x02,
	TypeMask = (Generator | MicroChip),
};

enum item_id
{
	Strontium = 0,
	Plutonium = 1,
	Thulium = 2,
	Ruthenium = 3,
	Curium = 4,
	LastId
};

uint32_t make_state( item_id id, item_type t )
{
	return t << (id * 2);
}

bool has_state( uint32_t state, item_id id, item_type t )
{
	auto s = make_state( id, t );

	return (state & s) == s;
}

bool has_generator( uint32_t state )
{
	return (state & item_type::Generator) == item_type::Generator;
}

bool is_shielded( uint32_t state )
{
	auto s = (state & item_type::TypeMask);

	return  s != item_type::MicroChip;
}

bool is_dangerous( uint32_t state )
{
	bool dangerous = false;
	bool shielded = true;

	while( state ) {

		dangerous |= has_generator( state );
		shielded &= is_shielded( state );

		state >>= 2;
	}

	return dangerous && !shielded;
}

bool move( uint32_t* from, uint32_t* to, uint32_t modules )
{
	if( ( *from & modules ) != modules )
		return false;

	*from &= ~( modules );
	*to |= modules;

	return true;
}

struct building_state
{
	int lift;
	array<uint32_t, 4> floors;

	bool operator==(const building_state& state ) const
	{
		return lift == state.lift &&
				floors[ 0 ] == state.floors[ 0 ] &&
				floors[ 1 ] == state.floors[ 1 ] &&
				floors[ 2 ] == state.floors[ 2 ] &&
				floors[ 3 ] == state.floors[ 3 ];
	}

	bool operator!=(const building_state& state ) const
	{
		return !( *this == state );
	}

	int Hash() const
	{
		return lift ^ floors[ 0 ] ^ floors[ 1 ] ^ floors[ 2 ] ^ floors[ 3 ];
	}
};

struct Hash{
	int operator()( const building_state& state ) const{
		return state.Hash();
	}
};

/*!
 * Create a set with all non-dangerous combinations and single module( generator/microchip ) items
 * These are all possible (combinations of) items that could be taken into the lift.
 * Because the lift won't move if not at least one item is taken, there is no "nothing combination"
 */
unordered_set<uint32_t> combinations()
{
	vector<uint32_t> c;
	unordered_set<uint32_t> r;

	for( uint32_t id = Strontium; id < LastId; ++id ) {
		c.push_back( make_state( (item_id) id, item_type::Generator ) );
		c.push_back( make_state( (item_id) id, item_type::MicroChip ) );

		r.insert( make_state( (item_id) id, item_type::Generator ) );
		r.insert( make_state( (item_id) id, item_type::MicroChip ) );
	}

	// Add non-dangerous combinations
	std::sort( c.begin(), c.end() );
	do {
		uint32_t m = c[ 0 ] | c[ 1 ];
		if( !is_dangerous( m ) )
			r.insert( m );
	}
	while( std::next_permutation( c.begin(), c.end() ) );

	return r;
}

/*!
 * Determine possible next floor state/lift position
 */
unordered_set<building_state, Hash> neighbors( const building_state& s )
{
	unordered_set<building_state, Hash> n;

	static auto comb = combinations();

	// Move lift up
	if( s.lift < 3 ) {
		for( uint32_t c : comb ) {

			building_state next = s;

			if( !move( &next.floors[ next.lift ], &next.floors[ next.lift + 1 ], c ) )
				continue;

			if( is_dangerous( next.floors[ next.lift ] ) )
				continue;
			++next.lift;
			if( is_dangerous( next.floors[ next.lift ] ) )
				continue;

			n.insert( next );
		}
	}

	// Move lift down
	if( s.lift > 0 ) {
		for( uint32_t c : comb ) {

			building_state next = s;

			if( !move( &next.floors[ next.lift ], &next.floors[ next.lift - 1 ], c ) )
				continue;

			if( is_dangerous( next.floors[ next.lift ] ) )
				continue;
			--next.lift;
			if( is_dangerous( next.floors[ next.lift ] ) )
				continue;

			n.insert( next );
		}
	}

	return n;
}

/*
 * Very much obliged : http://www.redblobgames.com/pathfinding/a-star/implementation.html
 */
std::vector<building_state> BreadthFirst( const building_state& start, const building_state& end )
{
	std::unordered_map<building_state, building_state, Hash> came_from;

	std::queue<building_state> frontier;
	frontier.push( start );
	came_from[ start ] = start;

	while( !frontier.empty() ) {
		auto current = frontier.front();
		frontier.pop();

		if( current == end )
			break;

		auto n = neighbors( current );
		for( auto next : n ) {

			if( !came_from.count( next ) ) {

				frontier.push( next );
				came_from[ next ] = current;
			}
		}
	}

	std::vector<building_state> path;
	auto current = end;
	path.push_back( current );
	while( current != start ) {
		current = came_from[ current ];
		path.push_back( current );
	}
	std::reverse( path.begin(), path.end() );

	return path;
}

void init_building_start( building_state& s )
{
	s.floors[ 0 ] = make_state( Strontium, Generator ) |
					make_state( Strontium, MicroChip ) |
					make_state( Plutonium, Generator ) |
					make_state( Plutonium, MicroChip );

	s.floors[ 1 ] = make_state( Thulium, Generator ) |
					make_state( Ruthenium, Generator ) |
					make_state( Ruthenium, MicroChip ) |
					make_state( Curium, Generator ) |
					make_state( Curium, MicroChip );

	s.floors[ 2 ] = make_state( Thulium, MicroChip );

	s.floors[ 3 ] = 0;

	s.lift = 0;
}

void init_building_end( building_state& s )
{
	s.floors[ 0 ] = s.floors[ 1 ] = s.floors[ 2 ] = 0;

	s.floors[ 3 ] = make_state( Strontium, Generator )	|
					make_state( Strontium, MicroChip )	|
					make_state( Plutonium, Generator )	|
					make_state( Plutonium, MicroChip )	|
					make_state( Thulium, Generator )	|
					make_state( Ruthenium, Generator )	|
					make_state( Ruthenium, MicroChip )	|
					make_state( Curium, Generator )		|
					make_state( Curium, MicroChip )		|
					make_state( Thulium, MicroChip );

	s.lift = 3;
}

std::ostream& print( std::ostream& str, item_id id )
{
	switch( id )
	{
		case item_id::Curium:
			str << "Curium";
		break;
		case item_id::Plutonium:
			str << "Plutonium";
		break;
		case item_id::Ruthenium:
			str << "Ruthenium";
		break;
		case item_id::Strontium:
			str << "Strontium";
		break;
		case item_id::Thulium:
			str << "Thulium";
		break;
		default:
			throw aoc_exception("Invalid item_id");
	}
	return str;
}

std::ostream& print( std::ostream& str, uint32_t modules )
{
	for( uint32_t id = Strontium; id < LastId; ++id ) {

		if( has_state( modules, (item_id) id, item_type::Generator ) )
			print( str, (item_id)id ) << " generator ";
		if( has_state( modules, (item_id) id, item_type::MicroChip ) )
			print( str, (item_id)id ) << " microchip ";
	}
	return str;
}

std::ostream& print( std::ostream& str, unordered_set<uint32_t>& combinations )
{
	for( auto m : combinations ) {
		print( str, m ) << '\n';
	}
	return str;
}

int Day11_Test( void )
{
	building_state building_start;
	init_building_start( building_start );

	assert( has_state( building_start.floors[ 0 ], item_id::Strontium, item_type::Generator ) );
	assert( has_state( building_start.floors[ 0 ], item_id::Strontium, item_type::MicroChip ) );
	assert( has_state( building_start.floors[ 0 ], item_id::Plutonium, item_type::Generator ) );
	assert( has_state( building_start.floors[ 0 ], item_id::Plutonium, item_type::MicroChip ) );

	assert( has_state( building_start.floors[ 1 ], item_id::Thulium, item_type::Generator ) );
	assert( has_state( building_start.floors[ 1 ], item_id::Ruthenium, item_type::Generator ) );
	assert( has_state( building_start.floors[ 1 ], item_id::Ruthenium, item_type::MicroChip ) );
	assert( has_state( building_start.floors[ 1 ], item_id::Curium, item_type::Generator ) );
	assert( has_state( building_start.floors[ 1 ], item_id::Curium, item_type::MicroChip ) );

	assert( has_state( building_start.floors[ 2 ], item_id::Thulium, item_type::MicroChip ) );

	assert( !is_dangerous( building_start.floors[ 0 ] ) );
	assert( !is_dangerous( building_start.floors[ 1 ] ) );
	assert( !is_dangerous( building_start.floors[ 2 ] ) );
	assert( !is_dangerous( building_start.floors[ 3 ] ) );

	auto comb = combinations();
	print( std::cout, comb );

	return -1;
}

int Day11_Part1( int argc, char* argv[] )
{
	building_state building_start;
	init_building_start( building_start );

	building_state building_end;
	init_building_end( building_end );

	auto path = BreadthFirst( building_start, building_end );

	// Should yield 37

	return path.size() - 1;
}

int Day11_Part2( int argc, char* argv[] )
{
	return -1;
}

