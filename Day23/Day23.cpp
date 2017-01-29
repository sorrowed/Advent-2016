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
	while( *p ) {

		auto o = Parse( *p );

		if( o == nullptr )
			std::cout << "Parsing failed: " << *p << '\n';
		else
		{
			std::cout << "OP: " << o->Token << " " << o->Args << '\n';
			ops.push_back( o );
		}

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

		Print();
	}
}


int Day23_Test(){

	const char *test[] = {
			"cpy 2 a",
			"tgl a",
			"tgl a",
			"tgl a",
			"cpy 1 a",
			"dec a",
			"dec a",
			nullptr
	};

	Reset();
	Process( test );

	Execute();

	std::cout << "Register a contains :" << registers[ "a" ].Value << '\n';

	return -1;
}


int Day23_Part1( int argc, char* argv[] ){ return -1;}
int Day23_Part2( int argc, char* argv[] ){ return -1;}

