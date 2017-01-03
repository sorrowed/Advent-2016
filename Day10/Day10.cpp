/*
 * Day10.cpp
 *
 *  Created on: Dec 10, 2016
 *      Author: tom
 */
#include "Common.h"

#include <cassert>
#include <iostream>
#include <map>
#include <utility>
#include <chrono>

#include "Exchangers.h"
#include "Operations.h"

using std::make_pair;
using std::make_shared;

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

void ReadOps( const vector<string>& lines, vector<pair<bool, shared_ptr<Op> > >& ops )
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

	// Read ops from file
	vector<string> lines;
	Read( "input.txt", &lines );

	vector<pair<bool, shared_ptr<Op> > > ops;
	ReadOps( lines, ops );

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Execute them as long as there are executable ops left
	bool busy = false;
	do{
		busy = false;
		for( auto& op : ops ){
			if( !op.first ){
				op.first |= op.second->Execute();

				busy |= op.first;
			}
		}
	}while( busy );

	std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

	shared_ptr<Exchanger> target = nullptr;

	for( auto& kvp : Bots ){
		auto& bot = kvp.second;

		for( auto& p : bot->Exchanges ){
			std::cout << "Bot " << bot->Id << " exchanged " << p.first << " and " << p.second << '\n';

			if( p.first == 17 && p.second == 61 )
				target = bot;
		}
	}

	std::cout << "Answer for part 1 : " << target->Id << " in " << elapsed << "[us]\n";

	return -1;
}

int Day10_Part2( int argc, char* argv[] )
{
	for( auto& kvp : Outputs ){
		auto& out = kvp.second;

		std::cout << "Output " << out->Id << " has " << out->High << '\n';
	}

	int p = Outputs[0]->High * Outputs[1]->High * Outputs[2]->High;

	std::cout << "Answer for part 2 : " << p << '\n';

	return -1;
}
