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
#include <utility>
#include <cassert>
#include <algorithm>
#include <cinttypes>

using std::array;
using std::vector;
using std::unordered_set;

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

bool is_generator( uint32_t state )
{
	return (state & item_type::TypeMask) == item_type::Generator;
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

		dangerous |= is_generator( state );
		shielded &= is_shielded( state );

		state >>= 2;
	}

	return dangerous && !shielded;
}

void move( uint32_t* from, uint32_t* to, uint32_t modules )
{
	if( ( *from & modules ) != modules )
		return;

	*from &= ~( modules );
	*to |= modules;
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
 * Determine possible next floor state/lift position
 */
unordered_set<building_state, Hash> neighbors( const building_state& s )
{
	unordered_set<building_state, Hash> n;

	vector<uint32_t> combinations;
	for( uint32_t id = Strontium; id < LastId; ++id ) {
		combinations.push_back( make_state( (item_id) id, item_type::Generator ) );
		combinations.push_back( make_state( (item_id) id, item_type::MicroChip ) );
	}
	std::sort( combinations.begin(), combinations.end() );

	// Move lift up
	if( s.lift < 3 ) {
		for( uint32_t c : combinations ) {

			building_state current = s, next = s;
			++next.lift;

			move( &next.floors[ current.lift ], &next.floors[ next.lift ], c );

			if( !is_dangerous( current.floors[ current.lift ] ) && !is_dangerous( next.floors[ next.lift ] ) )
				n.insert( next );
		}
	}

	return n;
}

void init( building_state& s )
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

int Day11_Test( void )
{
	building_state building;

	init( building );

	assert( has_state( building.floors[ 0 ], item_id::Strontium, item_type::Generator ) );
	assert( has_state( building.floors[ 0 ], item_id::Strontium, item_type::MicroChip ) );
	assert( has_state( building.floors[ 0 ], item_id::Plutonium, item_type::Generator ) );
	assert( has_state( building.floors[ 0 ], item_id::Plutonium, item_type::MicroChip ) );

	assert( has_state( building.floors[ 1 ], item_id::Thulium, item_type::Generator ) );
	assert( has_state( building.floors[ 1 ], item_id::Ruthenium, item_type::Generator ) );
	assert( has_state( building.floors[ 1 ], item_id::Ruthenium, item_type::MicroChip ) );
	assert( has_state( building.floors[ 1 ], item_id::Curium, item_type::Generator ) );
	assert( has_state( building.floors[ 1 ], item_id::Curium, item_type::MicroChip ) );

	assert( has_state( building.floors[ 2 ], item_id::Thulium, item_type::MicroChip ) );

	assert( !is_dangerous( building.floors[ 0 ] ) );
	assert( !is_dangerous( building.floors[ 1 ] ) );
	assert( !is_dangerous( building.floors[ 2 ] ) );
	assert( !is_dangerous( building.floors[ 3 ] ) );

	return -1;
}

int Day11_Part1( int argc, char* argv[] )
{
	return -1;
}

int Day11_Part2( int argc, char* argv[] )
{
	return -1;
}

