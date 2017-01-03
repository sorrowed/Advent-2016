/*
 * Day19.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: tom
 */
#include "Day19.h"

#include <list>
#include <iostream>
#include <deque>
#include <algorithm>

using std::deque;
using std::list;

struct elve
{
	int index;
	int presents;
};

list<elve>::iterator next_elve( list<elve>& elves, list<elve>::iterator c )
{
	auto r = ++c;
	return r != elves.end() ? r : elves.begin();
}

list<elve>::iterator find_not_zero( list<elve>& elves, list<elve>::iterator c )
{
	auto r = elves.end();

	auto ix = next_elve( elves, c );
	for( auto it = ix; r == elves.end() && ix != c; it = next_elve( elves, it ) ){
		if( it->presents != 0 )
			r = it;
	}
	return r;
}

int Day19_Test( void )
{
	list<elve> elves;
	for( int i = 1; i <= 5; ++i ){
		elves.push_back( elve { i, 1 } );
	}

	auto s = elves.begin();
	auto e = next_elve( elves, s );
	while( s != e )
	{
		s->presents += e->presents;
		e->presents = 0;

		s = find_not_zero( elves, e );
		e = find_not_zero( elves, s );
	}
	return s->index;
}

int Day19_Part1( int argc, char* argv[] )
{
	list<elve> elves;
	for( int i = 1; i <= 3005290; ++i ){
		elves.push_back( elve { i, 1 } );
	}

	auto s = elves.begin();
	auto e = next_elve( elves, s );
	while( s != e )
	{
		s->presents += e->presents;
		e->presents = 0;

		s = find_not_zero( elves, e );
		e = find_not_zero( elves, s );
	}


	std::cout << "Elve at position " << s->index << " gets all presents for part 1\n";

	// Should yield 1816277
	return s->index;
}

deque<elve>::iterator next_elve( deque<elve>& elves, deque<elve>::iterator c )
{
	auto r = ++c;
	return r != elves.end() ? r : elves.begin();
}

deque<elve>::iterator find_across( deque<elve>& elves, const deque<elve>::iterator c )
{
	int s = elves.size() / 2;

	auto r = elves.end() - c;
	if( s < r )
		return c + s;
	else
		return elves.begin() + ( s - r );
}

int Day19_Part2( int argc, char* argv[] )
{
	deque<elve> elves;
	for( int i = 0; i < 10000; ++i ){
		elves.push_back( elve { i + 1, 1 } );
	}

	auto s = elves.begin();
	auto e = find_across( elves, s );
	while( s != e )
	{
		s->presents += e->presents;
		elves.erase( e );
		s = next_elve( elves, s );

		e = find_across( elves, s );

		if( !( elves.size() % 1000 ) )
			std::cout << "Still " << elves.size() << " to go for part 2\n";
	}


	std::cout << "Elve at position " << s->index << " gets all presents for part 2\n";

	// Should yield 1410967
	return s->index;
}

