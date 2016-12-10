/*
 * Day10.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: tom
 */
#include "Common.h"

#include <utility>
#include <map>
#include <memory>
#include <cassert>
#include <string>
#include <sstream>

using std::map;
using std::shared_ptr;
using std::pair;
using std::make_shared;
using std::string;
using std::stringstream;
using std::vector;

struct Exchanger
{
	Exchanger( int id ) :
		Id( id ), Low( -1 ), High( -1 )
	{
	}

	int Id;
	int Low;
	int High;

	pair<int, int> Compares;

	void Assign( int value )
	{
		if( High != -1 && Low != -1 )
			; //Error
		else if( High == -1 )
			High = value;
		else
			Low = value;

		if( High < Low )
			std::swap( High, Low );

		if( High != -1 && Low != -1 )
			Compares.first = Low, Compares.second = High;
	}

	void GiveLow( shared_ptr<Exchanger> target )
	{
		if( Low == -1 )
			return;

		target->Assign( Low );

		Low = -1;
	}

	void GiveHigh( shared_ptr<Exchanger> target )
	{
		if( High == -1 )
			return;

		target->Assign( High );

		High = -1;
	}
};

stringstream& skip( stringstream& str, char v, int c )
{
	while( str && c ) {
		if( str.get() == v )
			--c;
	}
	return str;
}

map<int, shared_ptr<Exchanger>> Bots;
map<int, shared_ptr<Exchanger>> Outputs;

static inline
shared_ptr<Exchanger> WhenExists( map<int, shared_ptr<Exchanger>> &m, int id )
{
	return m.count( id ) > 0 ? m[ id ] : nullptr;
}

class Op
{
public:
	virtual void Execute( void )=0;
};

struct ExchangeOp: Op
{
	int srcId;

	string typeLow;
	int dstLow;

	string typeHigh;
	int dstHigh;

	void Parse( stringstream& str )
	{
		str >> srcId;
		skip( str, ' ', 4 );

		getline( str, typeLow, ' ' );
		str >> dstLow;
		skip( str, ' ', 4 );

		getline( str, typeHigh, ' ' );
		str >> dstHigh;
	}

	void Execute()
	{
		auto s = WhenExists( Bots, srcId );
		if( s == nullptr )
			return;

		shared_ptr<Exchanger> low = nullptr;
		if( typeLow == "bot" )
			low = WhenExists( Bots, dstLow );
		else if( typeLow == "output" )
			low = WhenExists( Outputs, dstLow );

		if( low != nullptr )
			s->GiveLow( low );

		shared_ptr<Exchanger> high = nullptr;
		if( typeHigh == "bot" )
			high = WhenExists( Bots, dstHigh );
		else if( typeHigh == "output" )
			high = WhenExists( Outputs, dstHigh );
		if( high != nullptr )
			s->GiveHigh( high );
	}
};

struct AssignOp: Op
{
	int value;
	int dst;

	void Parse( stringstream& str )
	{
		str >> value;
		skip( str, ' ', 4 );

		str >> dst;
	}

	void Execute()
	{
		if( Bots.count( dst ) == 0 )
			Bots[ dst ] = make_shared<Exchanger>( dst );

		auto s = Bots[ dst ];
		s->Assign( value );
	}
};

int Day10_Test( void )
{
	AssignOp o;
	o.value = 3;
	o.dst = 1;
	o.Execute();

	o.value = 2;
	o.dst = 2;
	o.Execute();

	o.value = 5;
	o.dst = 2;
	o.Execute();

	ExchangeOp e;
	e.srcId = 2;
	e.dstLow = 1;
	e.typeLow = "bot";
	e.dstHigh = 0;
	e.typeHigh = "bot";
	e.Execute();

	e.srcId = 1;
	e.dstLow = 0;
	e.typeLow = "output";
	e.dstHigh = 0;
	e.typeHigh = "bot";
	e.Execute();

	e.srcId = 0;
	e.dstLow = 2;
	e.typeLow = "output";
	e.dstHigh = 0;
	e.typeHigh = "output";
	e.Execute();

	volatile pair<int, int> cmp = Bots[ 2 ]->Compares;
	assert( cmp.first == 2 && cmp.second == 5 );

	//string line = "bot 88 gives low to bot 51 and high to bot 42";
	string line = "value 7 goes to bot 40";

	stringstream str( line );
	string op;
	getline( str, op, ' ' );
	if( op == "bot" ) {
		ExchangeOp o;
		o.Parse( str );

		o.Execute();
	} else if( op == "value" ) {
		AssignOp o;
		o.Parse( str );

		o.Execute();
	}

	return -1;
}

int Day10_Part1( int argc, char* argv[] )
{
	Bots.clear();
	Outputs.clear();

	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day10/input.txt", &lines );

	vector<shared_ptr<Op> > ops;

	for( auto& line : lines ) {

		stringstream str( line );
		string op;
		getline( str, op, ' ' );
		if( op == "bot" ) {
			auto o = make_shared<ExchangeOp>();
			o->Parse( str );
			ops.push_back( o );
		} else if( op == "value" ) {
			auto o = make_shared<AssignOp>();
			o->Parse( str );
			ops.push_back( o );
		}
	}

	for( auto& op : ops )
		op->Execute();

	shared_ptr<Exchanger> bot = nullptr;
	for( auto& kvp : Bots ) {
		auto& b = kvp.second;

		if( b->Low == 17 && b->High == 61 ) {
			bot = b;
			break;
		}
	}

	return bot != nullptr ? bot->Id : -1;
}

int Day10_Part2( int argc, char* argv[] )
{
	return -1;
}
