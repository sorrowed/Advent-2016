/*
 * Day12.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: tom
 */
#include "Day12.h"
#include "Monorail.h"

#include <cstring>
#include <iostream>

const char* input[] = { "cpy 1 a", "cpy 1 b", "cpy 26 d", "jnz c 2", "jnz 1 5", "cpy 7 c", "inc d", "dec c", "jnz c -2",
	"cpy a c", "inc a", "dec b", "jnz b -2", "cpy c b", "dec d", "jnz d -6", "cpy 18 c", "cpy 11 d", "inc a", "dec d",
	"jnz d -2", "dec c", "jnz c -5" };


static
void Process()
{
	for( auto& op : input ) {

		auto o = Parse( op );

		ops.push_back( o );
	}
}

static
void Execute()
{
	curOp = ops.begin();
	while( curOp != ops.end() )
		(*curOp)->Execute();
}

int Day12_Part1( int argc, char* argv[] )
{
	Reset();
	Process();
	Execute();

	std::cout << "Register a contains :" << registers[ "a" ].Value << '\n';

	return -1;
}

int Day12_Part2( int argc, char* argv[] )
{
	Reset();
	Process();
	registers[ "c" ].Value = 1;
	Execute();

	std::cout << "Register a contains :" << registers[ "a" ].Value << '\n';

	return -1;
}

int Day12_Test( void )
{
	return -1;
}
