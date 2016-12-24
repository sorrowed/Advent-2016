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

using std::array;
using std::vector;
using std::shared_ptr;
using std::make_shared;

enum item_type
{
	Generator, MicroChip, LastType
};

enum item_id
{
	Strontium, Plutonium, Thulium, Ruthenium, Curium, LastId
};

struct item_t
{
	item_type Type;
	item_id Id;

	item_t( item_type t, item_id id ) :
			Type( t ), Id( id )
	{
	}
};

typedef shared_ptr<item_t>	itemptr_t;
typedef vector<itemptr_t>	itemptrs_t;

array<itemptrs_t, 4> floors;

bool IsValid( const itemptr_t& a, const itemptr_t& b )
{
	return ( a->Type == MicroChip && b->Type == MicroChip ) || a->Id == b->Id;
}

bool IsValid( const itemptrs_t& items )
{
	return false;
}

void Init( void )
{
	auto f = &floors[ 0 ];
	f->clear();
	f->push_back( make_shared<item_t>( Generator, Strontium ) );
	f->push_back( make_shared<item_t>( MicroChip, Strontium ) );
	f->push_back( make_shared<item_t>( Generator, Plutonium ) );
	f->push_back( make_shared<item_t>( MicroChip, Plutonium ) );

	f = &floors[1];
	f->clear();
	f->push_back( make_shared<item_t>( Generator, Thulium ) );
	f->push_back( make_shared<item_t>( Generator, Ruthenium ) );
	f->push_back( make_shared<item_t>( MicroChip, Ruthenium ) );
	f->push_back( make_shared<item_t>( Generator, Curium ) );
	f->push_back( make_shared<item_t>( MicroChip, Curium ) );

	f = &floors[2];
	f->clear();
	f->push_back( make_shared<item_t>( MicroChip, Thulium ) );

	f = &floors[3];
	f->clear();
}

int Day11_Test( void )
{
	Init();

	assert( IsValid( floors[0][0], floors[0][1] ) );
	assert( IsValid( floors[0][2], floors[0][3] ) );

	assert( IsValid( floors[0] ) );
	assert( IsValid( floors[1] ) );
	assert( IsValid( floors[2] ) );
	assert( IsValid( floors[3] ) );

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

