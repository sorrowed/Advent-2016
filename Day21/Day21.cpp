/*
 * Day21.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: tom
 */
#include "Day21.h"
#include "Common.h"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <memory>

using std::vector;
using std::string;
using std::stringstream;
using std::shared_ptr;

struct Op
{
	virtual void Parse( stringstream& str ) = 0;
	virtual string& Apply( string& src ) = 0;
	virtual string& Revert( string& src ) = 0;
};

struct SwapOp: public Op
{
	int a;
	int b;

	void Parse( stringstream& str )
	{
		str >> std::ws;
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Apply( string& src )
	{
		std::swap( src[ a ], src[ b ] );

		return src;
	}

	string& Revert( string& src )
	{
		return Apply( src );
	}
};

struct SwapLetterOp: public Op
{
	char a;
	char b;

	void Parse( stringstream& str )
	{
		str >> std::ws;
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Apply( string& src )
	{
		auto s1 = src.find( a );
		auto s2 = src.find( b );

		std::swap( src[ s1 ], src[ s2 ] );

		return src;
	}

	string& Revert( string& src )
	{
		return Apply( src );
	}
};

struct ReverseOp: public Op
{
	int from;
	int to;

	void Parse( stringstream& str )
	{
		str >> std::ws;
		str.ignore( 256, ' ' );
		str >> from;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> to;
	}

	string& Apply( string& src )
	{
		std::reverse( src.begin() + from, src.begin() + to + 1 );

		return src;
	}

	string& Revert( string& src )
	{
		return Apply( src );
	}
};

struct RotateLeftOp: public Op
{
	int count;

	void Parse( stringstream& str )
	{
		str >> std::ws >> count;
	}

	string& Apply( string& src )
	{
		std::rotate( src.begin(), src.begin() + count, src.end() );

		return src;
	}

	string& Revert( string& src )
	{
		std::rotate( src.begin(), src.end() - count, src.end() );

		return src;
	}

};

struct RotateRightOp: public Op
{
	int count;

	void Parse( stringstream& str )
	{
		str >> std::ws >> count;
	}

	string& Apply( string& src )
	{
		std::rotate( src.begin(), src.end() - count, src.end() );

		return src;
	}

	string& Revert( string& src )
	{
		std::rotate( src.begin(), src.begin() + count, src.end() );

		return src;
	}

};

struct MoveOp: public Op
{
	int a;
	int b;

	void Parse( stringstream& str )
	{
		str >> std::ws;
		str.ignore( 256, ' ' );
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Apply( string& src )
	{
		auto s = src[ a ];
		src.erase( src.begin() + a );
		src.insert( src.begin() + b, s );
		return src;
	}

	string& Revert( string& src )
	{
		auto s = src[ b ];
		src.erase( src.begin() + b );
		src.insert( src.begin() + a, s );
		return src;
	}
};

#include <array>

struct RotateBasedOp: public Op
{
	char letter;

	void Parse( stringstream& str )
	{
		str >> std::ws;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> letter;
	}

	string& Apply( string& src )
	{
		int index = src.find( letter );
		if( index >= 4 )
			++index;
		++index;

		index %= src.length();

		std::rotate( src.begin(), src.end() - index, src.end() );

		return src;
	}

	string& Revert( string& src )
	{
		auto t = src;
		for( int i = 0; i < t.length() - 1; ++i )
		{
			std::rotate( t.begin(), t.begin() + 1, t.end() );
			auto t2 = t;
			Apply( t2 );

			if( t2 == src )
			{
				src = t;
				break;
			}
		}

		return src;
	}
};


int Day21_Test( void )
{
	string s = "abcde";
	SwapOp op1;
	stringstream str1( "4 with position 0" );
	op1.Parse( str1 );
	op1.Apply( s );
	op1.Revert( s );

	SwapLetterOp op7;
	stringstream str7( "a with letter b" );
	op7.Parse( str7 );
	op7.Apply( s );
	op7.Revert( s );

	ReverseOp op2;
	stringstream str2( "positions 1 through 3" );
	op2.Parse( str2 );
	op2.Apply( s );
	op2.Revert( s );

	RotateLeftOp op3;
	stringstream str3( "1 step" );
	op3.Parse( str3 );
	op3.Apply( s );
	op3.Revert( s );

	RotateRightOp op4;
	stringstream str4( "1 step" );
	op4.Parse( str4 );
	op4.Apply( s );
	op4.Revert( s );

	MoveOp op5;
	stringstream str5( "position 1 to position 3" );
	op5.Parse( str5 );
	op5.Apply( s );
	op5.Revert( s );

	RotateBasedOp op6;
	stringstream str6( "on position of letter c" );
	op6.Parse( str6 );
	op6.Apply( s );
	op6.Revert( s );


	RotateBasedOp op8;
	stringstream str8( "on position of letter d" );
	op8.Parse( str8 );
	s = "ecabd";
	op8.Apply( s );
	op8.Revert( s );

	return -1;
}

vector<shared_ptr<Op>> parse( const vector<string> lines )
{
	vector<shared_ptr<Op>> ops;

	for( auto& line : lines )
	{
		stringstream str( line );

		string token;
		str >> token;

		shared_ptr<Op> t = nullptr;

		if( token == "swap" ) {
			str >> token;
			if( token == "position" )
				t = std::make_shared<SwapOp>();
			else if( token == "letter" )
				t = std::make_shared<SwapLetterOp>();
		}
		else if( token == "reverse" )
			t = std::make_shared<ReverseOp>();
		else if( token == "rotate" ) {
			str >> token;
			if( token == "based" )
				t = std::make_shared<RotateBasedOp>();
			else if( token == "left" )
				t = std::make_shared<RotateLeftOp>();
			else if( token == "right" )
				t = std::make_shared<RotateRightOp>();
		}
		else if( token == "move" ) {
			t = std::make_shared<MoveOp>();
		}
		else
			throw std::exception();


		if( t != nullptr ) {
			t->Parse( str );
			ops.push_back( t );
		}
	}

	return ops;
}

int Day21_Part1( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day21/input.txt", &lines );

	auto ops = parse( lines );

	string input = "abcdefgh";
	for( auto& op : ops )
		op->Apply( input );

	// Should yield bdfhgeca
	return 0;
}

int Day21_Part2( int argc, char* argv[] )
{
	vector<string> lines;
	Read( "/home/tom/Projects/Advent-2016/Day21/input.txt", &lines );

	auto ops = parse( lines );
	std::reverse( ops.begin(), ops.end() );

	string input = "fbgdceah";
	for( auto& op : ops )
		op->Revert( input );

	// Should yield gdfcabeh
	return 0;
}

