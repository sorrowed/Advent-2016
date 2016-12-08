/*
 * Day8.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: tom
 */
#include "Day8.h"
#include "Common.h"

#include <sstream>
#include <cstring>
#include <cassert>

struct Screen
{
	const static int Width = 50;
	const static int Height = 6;

	char pixels[ Height ][ Width ];

	Screen()
	{
		Clear();
	}

	void Clear( void )
	{
		memset( pixels, 0, sizeof pixels );
	}

	int Count( void ) const
	{
		int count = 0;
		for( int r = 0; r < Height; ++r ) {
			for( int c = 0; c < Width; ++c ) {
				if( pixels[ r ][ c ] )
					++count;
			}
		}
		return count;
	}

	void Letter( char dst[ Height ][ 5 ], int ix )
	{
		for( int r = 0; r < Height; ++r ) {
			for( int c = 0; c < 5; ++c ) {

				dst[ r ][ c ] = pixels[ r ][ ix * 5 + c ];
			}
		}
	}
};

class Oper
{
public:

	virtual ~Oper(){};

	virtual void Process( Screen* screen ) = 0;

};

class Rect: public Oper
{
public:
	int Width;
	int Height;

	Rect( int width, int height ) :
		Width( width ), Height( height )
	{

	}

	void Process( Screen* screen )
	{
		for( int r = 0; r < Height; ++r ) {
			for( int c = 0; c < Width; ++c )
				screen->pixels[ r ][ c ] = '1';
		}
	}
};

class RotateColumn: public Oper
{
public:
	int Column;
	int Count;

	RotateColumn( int column, int count ) :
		Column( column ), Count( count )
	{

	}

	void Process( Screen* screen )
	{
		for( int m = 0; m < Count; ++m ) {

			auto l = screen->pixels[ screen->Height - 1 ][ Column ];
			for( int r = screen->Height - 1; r > 0; --r ) {
				screen->pixels[ r ][ Column ] = screen->pixels[ r - 1 ][ Column ];
			}
			screen->pixels[ 0 ][ Column ] = l;
		}
	}
};

class RotateRow: public Oper
{
public:
	int Row;
	int Count;

	RotateRow( int row, int count ) :
		Row( row ), Count( count )
	{

	}

	void Process( Screen* screen )
	{
		for( int m = 0; m < Count; ++m ) {

			auto l = screen->pixels[ Row ][ screen->Width - 1 ];
			for( int c = screen->Width - 1; c > 0; --c ) {
				screen->pixels[ Row ][ c ] = screen->pixels[ Row ][ c - 1 ];
			}
			screen->pixels[ Row ][ 0 ] = l;
		}
	}
};

Oper* ParseRect( std::stringstream& str )
{
	int w, h;
	str >> w;
	str.ignore( 1 );
	str >> h;

	return new Rect( w, h );
}

Oper* ParseRotateRow( std::stringstream& str )
{
	int r, count;

	str.ignore( 128, '=' );
	str >> r;
	str.ignore( 128, ' ' );
	str.ignore( 128, ' ' );
	str >> count;

	return new RotateRow( r, count );
}

Oper* ParseRotateColumn( std::stringstream& str )
{
	int c, count;

	str.ignore( 128, '=' );
	str >> c;
	str.ignore( 128, ' ' );
	str.ignore( 128, ' ' );
	str >> count;

	return new RotateColumn( c, count );
}

void Process( Screen* screen, const std::vector<std::string> lines )
{
	for( const std::string& line : lines ) {
		Oper* oper = nullptr;

		std::stringstream str( line );

		std::string s;
		str >> s;
		if( s == "rect" )
			oper = ParseRect( str );
		else if( s == "rotate" ) {
			str >> s;
			if( s == "row" ) {
				oper = ParseRotateRow( str );
			} else if( s == "column" ) {
				oper = ParseRotateColumn( str );
			}
		}

		if( oper )
			oper->Process( screen );

		delete oper;
	}
}

int Day8_Part1( int argc, char* argv[] )
{
	Screen screen;

	std::vector<std::string> lines;
	Read( "input.txt", &lines );

	Process( &screen, lines );

	return screen.Count();
}

int Day8_Part2( int argc, char* argv[] )
{
	Screen screen;

	std::vector<std::string> lines;
	Read( "input.txt", &lines );

	Process( &screen, lines );

	char letter[ Screen::Height ][ 5 ];

	for( int i = 0; i < 10; ++i ) {
		screen.Letter( letter, i );
	}
	return -1;
}

int Day8_Test( void )
{
	Screen screen;

	Rect o1( 3, 2 );
	o1.Process( &screen );
	RotateColumn o2( 1, 1 );
	o2.Process( &screen );
	RotateRow o3( 0, 4 );
	o3.Process( &screen );
	o2.Process( &screen );

	assert( screen.Count() == 6 );

	std::stringstream str( "rotate row y=1 by 20" );
	std::string dummy;
	str >> dummy;
	str >> dummy;
	Oper* op1 = ParseRotateRow( str );
	delete op1;

	return -1;
}
