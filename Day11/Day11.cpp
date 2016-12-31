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
#include "combinations.h"

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
	Elerium = 5,
	Dilithium = 6,
	LastId
};

uint16_t make_state( item_id id, item_type t )
{
	return t << (id * 2);
}

bool has_state( uint16_t state, item_id id, item_type t )
{
	auto s = make_state( id, t );

	return (state & s) == s;
}

bool has_generator( uint16_t state )
{
	return (state & item_type::Generator) == item_type::Generator;
}

bool is_shielded( uint16_t state )
{
	auto s = (state & item_type::TypeMask);

	return  s != item_type::MicroChip;
}

bool is_dangerous( uint16_t state )
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

struct building_state
{
	int lift;
	uint64_t img;

	uint16_t get_floor( int ix ) const {
		return ( img >> ( ix * 16 ) ) & 0xFFFF;
	}

	void set_floor( int ix, uint16_t value ){

		img &= ~( 0xFFFFUL <<  ( ix * 16 ) );
		img |= ( uint64_t( value ) << ( ix * 16 ) );
	}

	bool operator==(const building_state& state ) const
	{
		return lift == state.lift && img == state.img;
	}

	bool operator!=(const building_state& state ) const
	{
		return !( *this == state );
	}

	bool operator<(const building_state& state ) const
	{
		return img < state.img &&
				lift < state.lift;
	}

	int Hash() const
	{
		return lift ^ img;
	}
};

struct Hash{
	int operator()( const building_state& state ) const{
		return state.Hash();
	}
};

bool move( building_state* state, int from, int to, uint16_t modules )
{
	auto f = state->get_floor( from );

	if( ( f & modules ) != modules )
		return false;

	auto t = state->get_floor( to );

	f &= ~( modules );
	t |= modules;

	state->set_floor( from, f );
	state->set_floor( to, t );

	return true;
}

/*!
 * Create a set with all non-dangerous combinations and single module( generator/microchip ) items
 * These are all possible (combinations of) items that could be taken into the lift.
 * Because the lift won't move if not at least one item is taken, there is no "nothing combination"
 */
unordered_set<uint16_t> combinations()
{
	vector<uint16_t> c;
	unordered_set<uint16_t> r;

	for( uint16_t id = Strontium; id < LastId; ++id ) {
		c.push_back( make_state( item_id( id ), item_type::Generator ) );
		c.push_back( make_state( item_id( id ), item_type::MicroChip ) );

		r.insert( make_state( item_id( id ), item_type::Generator ) );
		r.insert( make_state( item_id( id ), item_type::MicroChip ) );
	}

	// Add non-dangerous combinations
	std::sort( c.begin(), c.end() );
	do {
		uint32_t m = c[ 0 ] | c[ 1 ];
		if( !is_dangerous( m ) )
			r.insert( m );
	}
	while( next_combination( c.begin(), c.begin() + 2, c.end() ) );

	return r;
}

/*!
 *	Determine if this combination is a pair
 *	If moving a pair of items (Generator + Microchip) any other pair of items (Generator + Microchip) would be
 *	an equivalent move, so remove all but one.
 */
bool is_pair( uint16_t comb )
{
	return (comb & 0x0003) == 0x0003 ||
			(comb & 0x000C) == 0x000C ||
			(comb & 0x0030) == 0x0030 ||
			(comb & 0x00C0) == 0x00C0 ||
			(comb & 0x0300) == 0x0300 ||
			(comb & 0x0C00) == 0x0C00 ||
			(comb & 0x3000) == 0x3000 ||
			(comb & 0xC000) == 0xC000;
}


unordered_set<building_state, Hash> visited;

/*!
 * Determine possible next floor state/lift position
 */
unordered_set<building_state, Hash> neighbors( const building_state& s )
{
	unordered_set<building_state, Hash> n;

	static auto comb = combinations();

	bool has_pair = false;

	// Move lift up
	if( s.lift < 3 ) {
		for( uint16_t c : comb ) {

			bool p = is_pair( c );
			if( has_pair && p )
				continue;

			building_state next = s;

			if( !move( &next, next.lift, next.lift + 1, c ) )
				continue;

			if( is_dangerous( next.get_floor( next.lift ) ) )
				continue;
			++next.lift;
			if( is_dangerous( next.get_floor( next.lift ) ) )
				continue;

			if( visited.count( next ) != 0 )
				continue;

			has_pair = p;

			n.insert( next );

			visited.insert( next );
		}
	}

	has_pair = false;

	// Move lift down
	if( s.lift > 0 ) {
		for( uint16_t c : comb ) {

			bool p = is_pair( c );
			if( has_pair && p )
				continue;

			building_state next = s;

			if( !move( &next, next.lift, next.lift - 1, c ) )
				continue;

			if( is_dangerous( next.get_floor( next.lift ) ) )
				continue;
			--next.lift;
			if( is_dangerous( next.get_floor( next.lift ) ) )
				continue;

			if( visited.count( next ) != 0 )
				continue;

			has_pair = p;

			n.insert( next );

			visited.insert( next );
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

	visited.clear();

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

double cost( const building_state& current, const building_state& next )
{
	return 1;
}

double heuristic( const building_state& next, const building_state& end )
{

	return __builtin_popcount( next.get_floor( 3 ) ) * 1 +
			__builtin_popcount( next.get_floor( 2 ) ) * 2 +
			__builtin_popcount( next.get_floor( 1 ) ) * 3 +
			__builtin_popcount( next.get_floor( 0 ) ) * 4;
}

using std::pair;
using std::priority_queue;
template<typename T, typename priority_t>
struct PriorityQueue
{
	typedef pair<priority_t, T> PQElement;
	priority_queue<PQElement, vector<PQElement>, std::greater<PQElement>> elements;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void put( T item, priority_t priority )
	{
		elements.emplace( priority, item );
	}

	inline T get()
	{
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

typedef unordered_map<building_state, building_state, Hash> building_state_map;
typedef unordered_map<building_state, double, Hash> building_cost_map;

std::vector<building_state> Astar( const building_state& start, const building_state& end )
{
	building_state_map came_from;
	building_cost_map cost_so_far;

	PriorityQueue<building_state, double> frontier;
	frontier.put( start, 0 );

	came_from[ start ] = start;
	cost_so_far[ start ] = 0;

	while( !frontier.empty() ) {
		auto current = frontier.get();

		if( current == end ) {
			break;
		}

		auto n = neighbors( current );
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

	vector<building_state> path;
	building_state current = end;
	path.push_back(current);
	while (current != start) {
	current = came_from[current];
	path.push_back(current);
	}
	std::reverse(path.begin(), path.end());

	return path;

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
		case item_id::Elerium:
			str << "Elerium";
		break;
		case item_id::Dilithium:
			str << "Dilithium";
		break;
		default:
			throw aoc_exception("Invalid item_id");
	}
	return str;
}

std::ostream& print( std::ostream& str, uint16_t modules )
{
	for( uint16_t id = Strontium; id < LastId; ++id ) {

		if( has_state( modules, (item_id) id, item_type::Generator ) )
			print( str, (item_id)id ) << " generator ";
		if( has_state( modules, (item_id) id, item_type::MicroChip ) )
			print( str, (item_id)id ) << " microchip ";
	}
	return str;
}

std::ostream& print( std::ostream& str, unordered_set<uint16_t>& combinations )
{
	for( auto m : combinations ) {
		print( str, m ) << '\n';
	}
	return str;
}

void init_building_start_pt1( building_state& s )
{
	s.set_floor( 0, make_state( Strontium, Generator ) |
					make_state( Strontium, MicroChip ) |
					make_state( Plutonium, Generator ) |
					make_state( Plutonium, MicroChip ) );

	s.set_floor( 1, make_state( Thulium, Generator ) |
					make_state( Ruthenium, Generator ) |
					make_state( Ruthenium, MicroChip ) |
					make_state( Curium, Generator ) |
					make_state( Curium, MicroChip ) );

	s.set_floor( 2, make_state( Thulium, MicroChip ) );

	s.set_floor( 3, 0 );

	s.lift = 0;
}

void init_building_end_pt1( building_state& s )
{
	s.set_floor( 0, 0 );
	s.set_floor( 1, 0 );
	s.set_floor( 2, 0 );

	s.set_floor( 3, make_state( Strontium, Generator )	|
					make_state( Strontium, MicroChip )	|
					make_state( Plutonium, Generator )	|
					make_state( Plutonium, MicroChip )	|
					make_state( Thulium, Generator )	|
					make_state( Ruthenium, Generator )	|
					make_state( Ruthenium, MicroChip )	|
					make_state( Curium, Generator )		|
					make_state( Curium, MicroChip )		|
					make_state( Thulium, MicroChip ) );

	s.lift = 3;
}

void init_building_start_pt2( building_state& s )
{
	s.set_floor( 0, make_state( Strontium, Generator )	|
					make_state( Strontium, MicroChip )	|
					make_state( Plutonium, Generator )	|
					make_state( Plutonium, MicroChip )	|
					make_state( Elerium, Generator ) 	|
					make_state( Elerium, MicroChip )	|
					make_state( Dilithium, Generator )	|
					make_state( Dilithium, MicroChip ) );

	s.set_floor( 1, make_state( Thulium, Generator )	|
					make_state( Ruthenium, Generator )	|
					make_state( Ruthenium, MicroChip )	|
					make_state( Curium, Generator )		|
					make_state( Curium, MicroChip ) );

	s.set_floor( 2, make_state( Thulium, MicroChip ) );

	s.set_floor( 3, 0 );

	s.lift = 0;
}

void init_building_end_pt2( building_state& s )
{
	s.set_floor( 0, 0 );
	s.set_floor( 1, 0 );
	s.set_floor( 2, 0 );

	s.set_floor( 3, make_state( Strontium, Generator )	|
					make_state( Strontium, MicroChip )	|
					make_state( Plutonium, Generator )	|
					make_state( Plutonium, MicroChip )	|
					make_state( Thulium, Generator )	|
					make_state( Ruthenium, Generator )	|
					make_state( Ruthenium, MicroChip )	|
					make_state( Curium, Generator )		|
					make_state( Curium, MicroChip )		|
					make_state( Thulium, MicroChip )	|
					make_state( Elerium, Generator )	|
					make_state( Elerium, MicroChip )	|
					make_state( Dilithium, Generator )	|
					make_state( Dilithium, MicroChip ) );

	s.lift = 3;
}
int Day11_Test( void )
{
	building_state building_start;
	init_building_start_pt1( building_start );

	assert( has_state( building_start.get_floor(0), item_id::Strontium, item_type::Generator ) );
	assert( has_state( building_start.get_floor(0), item_id::Strontium, item_type::MicroChip ) );
	assert( has_state( building_start.get_floor(0), item_id::Plutonium, item_type::Generator ) );
	assert( has_state( building_start.get_floor(0), item_id::Plutonium, item_type::MicroChip ) );

	assert( has_state( building_start.get_floor(1), item_id::Thulium, item_type::Generator ) );
	assert( has_state( building_start.get_floor(1), item_id::Ruthenium, item_type::Generator ) );
	assert( has_state( building_start.get_floor(1), item_id::Ruthenium, item_type::MicroChip ) );
	assert( has_state( building_start.get_floor(1), item_id::Curium, item_type::Generator ) );
	assert( has_state( building_start.get_floor(1), item_id::Curium, item_type::MicroChip ) );

	assert( has_state( building_start.get_floor(2), item_id::Thulium, item_type::MicroChip ) );

	assert( !is_dangerous( building_start.get_floor(0) ) );
	assert( !is_dangerous( building_start.get_floor(1) ) );
	assert( !is_dangerous( building_start.get_floor(2) ) );
	assert( !is_dangerous( building_start.get_floor(3) ) );

	auto comb = combinations();
	print( std::cout, comb );

	return -1;
}

int Day11_Part1( int argc, char* argv[] )
{
	building_state building_start;
	init_building_start_pt1( building_start );

	building_state building_end;
	init_building_end_pt1( building_end );

	auto path = Astar( building_start, building_end );

	//auto path = BreadthFirst( building_start, building_end );

	// Should yield 37

	std::cout << "Part 1 completed in " << path.size() - 1 << " steps\n";

	return path.size() - 1;
}

int Day11_Part2( int argc, char* argv[] )
{
	building_state building_start;
	init_building_start_pt2( building_start );

	building_state building_end;
	init_building_end_pt2( building_end );

	//auto path = BreadthFirst( building_start, building_end );
	auto path = Astar( building_start, building_end );

	// Should yield 61
	std::cout << "Part 2 completed in " << path.size() - 1 << " steps\n";

	return path.size() - 1;
}

