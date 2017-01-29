/*
 * Day32.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: tom
 */

#include "Monorail.h"
#include <iostream>

static
void Process( const char* input[] )
{
	auto p = input;
	while( *p ){

		auto o = Parse( *p );

		if( o == nullptr )
			std::cout << "Parsing failed: " << *p << '\n';
		else
			ops.push_back( o );

		++p;
	}
}

static void Print()
{
	std::cout << "BEGIN\n";

	for( auto o : ops ){
		std::cout << "OP: " << o->Token << " " << o->Args << '\n';
	}

	std::cout << "END\n\n";
}

static
void Execute()
{
	Print();

	curOp = ops.begin();
	while( curOp != ops.end() ){
		(*curOp)->Execute();

		//Print();
	}
}

int Day23_Test()
{

	const char *test[] = { "cpy 2 a", "tgl a", "tgl a", "tgl a", "cpy 1 a", "dec a", "dec a", nullptr };

	Reset();
	Process( test );

	Execute();

	std::cout << "Register a contains :" << registers["a"].Value << '\n';

	return registers["a"].Value;
}

int Day23_Part1( int argc, char* argv[] )
{
	const char * input[] = { "cpy a b", "dec b", "cpy a d", "cpy 0 a", "cpy b c", "inc a", "dec c", "jnz c -2", "dec d",
			"jnz d -5", "dec b", "cpy b c", "cpy c d", "dec d", "inc c", "jnz d -2", "tgl c", "cpy -16 c", "jnz 1 c",
			"cpy 94 c", "jnz 82 d", "inc a", "inc d", "jnz d -2", "inc c", "jnz c -5", nullptr };

	Reset();
	Process( input );

	registers["a"].Value = 7;

	Execute();

	std::cout << "Part 1 : Register a contains :" << registers["a"].Value << '\n';

	return registers["a"].Value;
}

int Day23_Part2( int argc, char* argv[] )
{
	const char * input[] = { "cpy a b", "dec b", "cpy a d", "cpy 0 a", "cpy b c", "inc a", "dec c", "jnz c -2", "dec d",
			"jnz d -5", "dec b", "cpy b c", "cpy c d", "dec d", "inc c", "jnz d -2", "tgl c", "cpy -16 c", "jnz 1 c",
			"cpy 94 c", "jnz 82 d", "inc a", "inc d", "jnz d -2", "inc c", "jnz c -5", nullptr };

	Reset();
	Process( input );

	registers["a"].Value = 12;

	Execute();

	std::cout << "Part 2 : Register a contains :" << registers["a"].Value << '\n';

	return registers["a"].Value;
}

