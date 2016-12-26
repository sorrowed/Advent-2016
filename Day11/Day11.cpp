/*
 * Day11.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#include <array>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>
#include <cinttypes>

using std::array;
using std::vector;

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
	*from &= ~( modules );
	*to |= modules;
}

vector<array<uint32_t, 4> > neighbors( array<uint32_t, 4> floors )
{

}

void init( array<uint32_t,4>& floors )
{
	floors[ 0 ] = make_state( Strontium, Generator ) |
					make_state( Strontium, MicroChip ) |
					make_state( Plutonium, Generator ) |
					make_state( Plutonium, MicroChip );

	floors[ 1 ] = make_state( Thulium, Generator ) |
					make_state( Ruthenium, Generator ) |
					make_state( Ruthenium, MicroChip ) |
					make_state( Curium, Generator ) |
					make_state( Curium, MicroChip );

	floors[ 2 ] = make_state( Thulium, MicroChip );

	floors[ 3 ] = 0;
}

int Day11_Test( void )
{
	array<uint32_t,4> floors;

	init( floors );

	assert( has_state( floors[ 0 ], item_id::Strontium, item_type::Generator ) );
	assert( has_state( floors[ 0 ], item_id::Strontium, item_type::MicroChip ) );
	assert( has_state( floors[ 0 ], item_id::Plutonium, item_type::Generator ) );
	assert( has_state( floors[ 0 ], item_id::Plutonium, item_type::MicroChip ) );

	assert( has_state( floors[ 1 ], item_id::Thulium, item_type::Generator ) );
	assert( has_state( floors[ 1 ], item_id::Ruthenium, item_type::Generator ) );
	assert( has_state( floors[ 1 ], item_id::Ruthenium, item_type::MicroChip ) );
	assert( has_state( floors[ 1 ], item_id::Curium, item_type::Generator ) );
	assert( has_state( floors[ 1 ], item_id::Curium, item_type::MicroChip ) );

	assert( has_state( floors[ 2 ], item_id::Thulium, item_type::MicroChip ) );

	assert( !is_dangerous( floors[ 0 ] ) );
	assert( !is_dangerous( floors[ 1 ] ) );
	assert( !is_dangerous( floors[ 2 ] ) );
	assert( !is_dangerous( floors[ 3 ] ) );

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

