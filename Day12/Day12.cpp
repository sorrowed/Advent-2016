/*
 * Day12.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: tom
 */
#include "Day12.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include <iostream>

class Op;

const char* input[] = { "cpy 1 a", "cpy 1 b", "cpy 26 d", "jnz c 2", "jnz 1 5", "cpy 7 c", "inc d", "dec c", "jnz c -2",
	"cpy a c", "inc a", "dec b", "jnz b -2", "cpy c b", "dec d", "jnz d -6", "cpy 18 c", "cpy 11 d", "inc a", "dec d",
	"jnz d -2", "dec c", "jnz c -5" };

struct Register
{
	char Id;
	long Value;
};

typedef std::vector<std::shared_ptr<Op>> OpQue;
typedef std::map<std::string, Register> RegMap;

OpQue ops;
OpQue::iterator curOp;
RegMap registers;

struct Op
{
	virtual void Parse( std::stringstream& str ) = 0;
	virtual void Execute() = 0;
};

struct Copy1: public Op
{
	int srcVal;
	std::string dstReg;

	Copy1( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcVal;
		str >> dstReg;
	}

	void Execute()
	{
		registers[ dstReg ].Value = srcVal;
		curOp++;
	}
};

struct Copy2: public Op
{
	std::string srcReg;
	std::string dstReg;

	Copy2( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		str >> dstReg;
	}

	void Execute()
	{
		registers[ dstReg ].Value = registers[ srcReg ].Value;
		curOp++;
	}
};

struct JmpNz1: public Op
{
	std::string srcReg;
	int Offs;

	JmpNz1( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		str >> Offs;
	}

	void Execute()
	{
		if( registers[ srcReg ].Value )
			curOp += Offs;
		else
			curOp++;
	}
};

struct JmpNz2: public Op
{
	int srcVal;
	int Offs;

	JmpNz2( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcVal;
		str >> Offs;
	}

	void Execute()
	{
		if( srcVal )
			curOp += Offs;
		else
			curOp++;
	}
};

struct Inc: public Op
{
	std::string srcReg;
	int Offs;

	Inc( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		str >> Offs;
	}

	void Execute()
	{
		registers[ srcReg ].Value++;
		curOp++;
	}
};

struct Dec: public Op
{
	std::string srcReg;
	int Offs;

	Dec( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		str >> Offs;
	}

	void Execute()
	{
		registers[ srcReg ].Value--;
		curOp++;
	}
};

static inline
bool IsReg( const std::string& token )
{
	const char* regs = "abcdefghijklmnopqrstuvwxyz";

	return token.find_first_of( regs, 0 ) != std::string::npos;
}

void Process()
{
	for( auto& op : input ) {
		std::stringstream p( op );
		std::string token;
		p >> token;
		if( token == "cpy" ) {
			p.ignore( 1 );
			token = p.peek();
			if( IsReg( token ) )
				ops.push_back( std::make_shared < Copy2 > (p) );
			else
				ops.push_back( std::make_shared < Copy1 > (p) );
		} else if( token == "jnz" ) {
			p.ignore( 1 );
			token = p.peek();
			if( IsReg( token ) )
				ops.push_back( std::make_shared < JmpNz1 > (p) );
			else
				ops.push_back( std::make_shared < JmpNz2 > (p) );
		} else if( token == "inc" )
			ops.push_back( std::make_shared < Inc > (p) );
		else if( token == "dec" )
			ops.push_back( std::make_shared < Dec > (p) );
	}
}

void Execute()
{
	curOp = ops.begin();
	while( curOp != ops.end() )
		(*curOp)->Execute();
}

void Reset()
{
	registers.clear();
	ops.clear();
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
