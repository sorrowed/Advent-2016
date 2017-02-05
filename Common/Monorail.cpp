/*
 * Monorail.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: tom
 */
#include "Monorail.h"
#include <iostream>

OpQue ops;
OpQue::iterator curOp;
RegMap registers;

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

int out;
int fault;

void Execute()
{
	//Print();

	curOp = ops.begin();
	while( curOp != ops.end() && !fault ){
		(*curOp)->Execute();
	}
}

std::shared_ptr<Op> Parse( const char* input )
{
	std::shared_ptr<Op> r = nullptr;

	std::stringstream p( input );
	std::string token;
	p >> token;
	if( token == "cpy" ) {
		p.ignore( 1 );
		r = std::make_shared<Copy>( token, p );
	}
	else if( token == "jnz" ) {
		p.ignore( 1 );
		r = std::make_shared<Jnz>( token, p );
	}
	else if( token == "inc" )
		r = std::make_shared<Inc>( token, p );
	else if( token == "dec" )
		r = std::make_shared<Dec>( token, p );
	else if( token == "tgl" )
		r = std::make_shared<Tgl>( token, p );
	else if( token == "out" )
		r = std::make_shared<Out>( token, p );

	return r;
}

Copy::Copy( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Copy::Parse( const std::string& token, std::stringstream& str )
{
	str >> src;
	str >> dst;

	Token = token;
	Args = src + ' ' + dst;
}

void Copy::Execute()
{
	int value;
	if( IsReg( src ) )
		value = registers[ src ].Value;
	else {
		std::stringstream s( src );
		s >> value;
	}

	if( IsReg( dst ) )
		registers[ dst ].Value = value;

	curOp++;
}

Jnz::Jnz( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Jnz::Parse( const std::string& token, std::stringstream& str )
{
	str >> src;
	str >> Offs;

	std::stringstream a;
	a << src;
	a << ' ';
	a << Offs;

	Token = token;
	Args = a.str();

}

void Jnz::Execute()
{
	int value = 1;
	if( IsReg( src ) )
		value = registers[ src ].Value;
	else {
		std::stringstream s( src );
		s >> value;
	}

	int offs;
	if( IsReg( Offs ) )
		offs = registers[ Offs ].Value;
	else {
		std::stringstream s( Offs );
		s >> offs;
	}

	if( value )
		curOp += offs;
	else
		++curOp;
}

Inc::Inc( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Inc::Parse( const std::string& token, std::stringstream& str )
{
	Token = token;
	str >> srcReg;
	Args = srcReg;
}

void Inc::Execute()
{
	registers[ srcReg ].Value++;
	curOp++;
}

Dec::Dec( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Dec::Parse( const std::string& token, std::stringstream& str )
{
	Token = token;

	str >> srcReg;

	Args = srcReg;
}

void Dec::Execute()
{
	registers[ srcReg ].Value--;
	curOp++;
}

/*
 tgl x toggles the instruction x away (pointing at instructions like jnz does: positive means forward; negative means backward):

 For one-argument instructions, inc becomes dec, and all other one-argument instructions become inc.
 For two-argument instructions, jnz becomes cpy, and all other two-instructions become jnz.
 The arguments of a toggled instruction are not affected.
 If an attempt is made to toggle an instruction outside the program, nothing happens.
 If toggling produces an invalid instruction (like cpy 1 2) and an attempt is later made to execute that instruction, skip it instead.
 If tgl toggles itself (for example, if a is 0, tgl a would target itself and become inc a), the resulting instruction is not executed until the next time it is reached.
 */
Tgl::Tgl( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Tgl::Parse( const std::string& token, std::stringstream& str )
{
	Token = token;

	str >> srcReg;

	Args = srcReg;
}

void Tgl::Execute()
{
	int offset = registers[ srcReg ].Value;

	auto op = (curOp + offset);
	if( op >= ops.begin() && op < ops.end() ) {
		std::stringstream str;

		if( (*op)->Arguments() == 1 ) {

			if( (*op)->Token == "inc" ) {
				str << "dec ";
				str << (*op)->Args;
			}
			else {
				str << "inc ";
				str << (*op)->Args;
			}
		}
		else if( (*op)->Arguments() == 2 ) {
			if( (*op)->Token == "jnz" ) {
				str << "cpy ";
				str << (*op)->Args;
			}
			else {
				str << "jnz ";
				str << (*op)->Args;
			}
		}

		auto o = ::Parse( str.str().c_str() );
		*op = o;

		if( offset != 0 )
			curOp++;
	}
	else
		curOp++;
}

Out::Out( const std::string& token, std::stringstream& str ) :
		Op()
{
	Parse( token, str );
}

void Out::Parse( const std::string& token, std::stringstream& str )
{
	Token = token;

	str >> src;

	Args = src;
}

void Out::Execute()
{
	if( IsReg( src ) )
		Value = registers[ src ].Value;
	else {
		std::stringstream s( src );
		s >> Value;
	}

	if( Value != out )
		out = Value;
	else
		fault = 1;

	std::cout << out;

	curOp++;
}
