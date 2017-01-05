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

using std::vector;
using std::string;
using std::stringstream;

struct Op
{
	virtual void Parse( stringstream& str ) = 0;
	virtual string& Execute( string& src ) = 0;
};

struct SwapOp: public Op
{
	int a;
	int b;

	void Parse( stringstream& str )
	{
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Execute( string& src )
	{
		std::swap( src[ a ], src[ b ] );

		return src;
	}
};

struct SwapLetterOp: public Op
{
	char a;
	char b;

	void Parse( stringstream& str )
	{
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Execute( string& src )
	{
		auto s1 = src.find( a );
		auto s2 = src.find( b );

		std::swap( src[ s1 ], src[ s2 ] );

		return src;
	}
};

struct ReverseOp: public Op
{
	int from;
	int to;

	void Parse( stringstream& str )
	{
		str.ignore( 256, ' ' );
		str >> from;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> to;
	}

	string& Execute( string& src )
	{
		std::reverse( src.begin() + from, src.begin() + to + 1 );

		return src;
	}
};

struct RotateLeftRightOp: public Op
{
	string dir;
	int count;

	void Parse( stringstream& str )
	{
		str >> dir;
		str >> count;
	}

	string& Execute( string& src )
	{
		if( dir == "left" ){
			std::rotate( src.begin(), src.begin() + count, src.end() );
		}
		else
		if( dir == "right" ){
			std::rotate( src.begin(), src.end() - count, src.end() );
		}

		return src;
	}
};

struct MoveOp: public Op
{
	int a;
	int b;

	void Parse( stringstream& str )
	{
		str.ignore( 256, ' ' );
		str >> a;
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> b;
	}

	string& Execute( string& src )
	{
		auto s = src[ a ];
		src.erase( src.begin() + a );
		src.insert( src.begin() + b, s );
		return src;
	}
};

struct RotateBasedOp: public Op
{
	char letter;

	void Parse( stringstream& str )
	{
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str.ignore( 256, ' ' );
		str >> letter;
	}

	string& Execute( string& src )
	{
		auto count = src.find( letter );
		if( count >= 4 )
			++count;
		++count;

		std::rotate( src.begin(), src.end() - count, src.end() );

		return src;
	}
};


int Day21_Test( void )
{
	vector<string> lines;
	Read( "input.txt", &lines );

	string s = "abcde";
	SwapOp op1;
	stringstream str1( "4 with position 0" );
	op1.Parse( str1 );
	op1.Execute( s );

	SwapLetterOp op7;
	stringstream str7( "a with letter b" );
	op7.Parse( str7 );
	op7.Execute( s );

	ReverseOp op2;
	stringstream str2( "positions 1 through 3" );
	op2.Parse( str2 );
	op2.Execute( s );

	RotateLeftRightOp op3;
	stringstream str3( "left 1 step" );
	op3.Parse( str3 );
	op3.Execute( s );

	RotateLeftRightOp op4;
	stringstream str4( "right 1 step" );
	op4.Parse( str4 );
	op4.Execute( s );

	MoveOp op5;
	stringstream str5( "position 1 to position 3" );
	op5.Parse( str5 );
	op5.Execute( s );

	RotateBasedOp op6;
	stringstream str6( "based on position of letter c" );
	op6.Parse( str6 );
	op6.Execute( s );

	return -1;
}

int Day21_Part1( int argc, char* argv[] )
{
	return -1;
}

int Day21_Part2( int argc, char* argv[] )
{
	return -1;
}

