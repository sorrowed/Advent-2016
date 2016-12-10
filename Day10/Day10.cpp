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
	int Id;
	int Low;
	int High;

	bool CanAssign( void ) const
	{
		return Low == -1 || High == -1;
	}

	bool CanExchange( void ) const
	{
		return Low != -1 && High != -1;
	}

	Exchanger( int id ) :
			Id( id ), Low( -1 ), High( -1 )
	{
	}

	bool Assign( int value )
	{
		if( !CanAssign() )
			return false;

		if( High == -1 )
			High = value;
		else
			Low = value;

		if( High < Low )
			std::swap( High, Low );

		return true;
	}

	bool Exchange( shared_ptr<Exchanger> low, shared_ptr<Exchanger> high )
	{
		if( !CanExchange() )
			return false;

		GiveLow( low );
		GiveHigh( high );

		return true;
	}

private:
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
	while( str && c ){
		if( str.get() == v )
			--c;
	}
	return str;
}

map<int, shared_ptr<Exchanger>> Bots;
map<int, shared_ptr<Exchanger>> Outputs;

static inline shared_ptr<Exchanger> GetExchanger( map<int, shared_ptr<Exchanger>> &m, int id )
{
	if( m.count( id ) == 0 )
		m[id] = make_shared<Exchanger>( id );

	return m[id];
}

struct Op
{
	virtual bool Execute() = 0;
};

struct ExchangeOp: Op
{
	int Id;

	string LowType;
	int LowDst;

	string HighType;
	int HighDst;

	void Parse( stringstream& str )
	{
		str >> Id;
		skip( str, ' ', 4 );

		getline( str, LowType, ' ' );
		str >> LowDst;
		skip( str, ' ', 4 );

		getline( str, HighType, ' ' );
		str >> HighDst;
	}

	bool Execute()
	{
		auto s = GetExchanger( Bots, Id );

		shared_ptr<Exchanger> low = nullptr;
		if( LowType == "bot" )
			low = GetExchanger( Bots, LowDst );
		else if( LowType == "output" )
			low = GetExchanger( Outputs, LowDst );

		shared_ptr<Exchanger> high = nullptr;
		if( HighType == "bot" )
			high = GetExchanger( Bots, HighDst );
		else if( HighType == "output" )
			high = GetExchanger( Outputs, HighDst );

		return s->Exchange( low, high );
	}
};

struct AssignOp: Op
{
	int Id;
	int Value;

	void Parse( stringstream& str )
	{
		str >> Value;
		skip( str, ' ', 4 );

		str >> Id;
	}

	bool Execute()
	{
		auto s = GetExchanger( Bots, Id );

		return s->Assign( Value );
	}
};

int Day10_Test( void )
{
	auto bot = GetExchanger( Bots, 2 );

	AssignOp o;
	o.Value = 3;
	o.Id = 1;
	o.Execute();

	o.Value = 2;
	o.Id = 2;
	o.Execute();

	o.Value = 5;
	o.Id = 2;
	o.Execute();

	assert( bot->Low == 2 && bot->High == 5 );

	ExchangeOp e;
	e.Id = 2;
	e.LowDst = 1;
	e.LowType = "bot";
	e.HighDst = 0;
	e.HighType = "bot";
	e.Execute();

	e.Id = 1;
	e.LowDst = 0;
	e.LowType = "output";
	e.HighDst = 0;
	e.HighType = "bot";
	e.Execute();

	e.Id = 0;
	e.LowDst = 2;
	e.LowType = "output";
	e.HighDst = 0;
	e.HighType = "output";
	e.Execute();




	//string line = "bot 88 gives low to bot 51 and high to bot 42";
	string line = "value 7 goes to bot 40";

	stringstream str( line );
	string op;
	getline( str, op, ' ' );
	if( op == "bot" ){
		ExchangeOp o;
		o.Parse( str );

		o.Execute();
	}else if( op == "value" ){
		AssignOp o;
		o.Parse( str );

		o.Execute();
	}

	return -1;
}

void ReadOps( const vector<string>& lines, vector<pair<bool, shared_ptr<Op> > > ops )
{
	for( auto& line : lines ){
		stringstream str( line );
		string op;
		getline( str, op, ' ' );
		if( op == "bot" ){
			auto o = make_shared<ExchangeOp>();
			o->Parse( str );
			ops.push_back( make_pair( false, o ) );
		}else if( op == "value" ){
			auto o = make_shared<AssignOp>();
			o->Parse( str );
			ops.push_back( make_pair( false, o ) );
		}
	}
}

int Day10_Part1( int argc, char* argv[] )
{
	Bots.clear();
	Outputs.clear();

	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day10/input.txt", &lines );

	vector<pair<bool, shared_ptr<Op> > > ops;

	ReadOps( lines, ops );

	while( true ){
		for( auto& op : ops ){
			op.second->Execute();
		}
	}

	return -1;
}

int Day10_Part2( int argc, char* argv[] )
{
	return -1;
}
