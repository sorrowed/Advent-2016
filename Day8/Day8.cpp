/*
 * Day8.cpp
 *
 *  Created on: Dec 8, 2016
 *      Author: tom
 */
#include "Day8.h"
#include "Common.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

class ScreenBase
{
public:
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual char GetPixel( int x, int y ) const = 0;
	virtual void SetPixel( int x, int y, char v = '#' ) = 0;

	virtual ~ScreenBase()
	{
	}

	void Clear( void )
	{
		for( int y = 0; y < GetHeight(); ++y ) {
			for( int x = 0; x < GetWidth(); ++x ) {
				SetPixel( x, y, ' ' );
			}
		}
	}

	int Count( void ) const
	{
		int count = 0;

		for( int y = 0; y < GetHeight(); ++y ) {
			for( int x = 0; x < GetWidth(); ++x ) {
				if( GetPixel( x, y ) != ' ' )
					++count;
			}
		}

		return count;
	}

	void Print( std::ostream& s )
	{
		for( int y = 0; y < GetHeight(); ++y ) {
			for( int x = 0; x < GetWidth(); ++x ) {
				s << GetPixel( x, y );
			}
			s << '\n';
		}
	}
};

template<int w, int h>
struct Screen: public ScreenBase
{
	char pixels[ h ][ w ];

	const int Width = w;
	const int Height = h;

	int GetWidth() const
	{
		return Width;
	}

	int GetHeight() const
	{
		return Height;
	}

	char GetPixel( int x, int y ) const
	{
		return pixels[ y ][ x ];
	}

	void SetPixel( int x, int y, char v )
	{
		pixels[ y ][ x ] = v;
	}

	Screen() :
		ScreenBase()
	{
		Clear();
	}
};

class Oper
{
public:

	virtual ~Oper()
	{
	}
	;

	virtual void Process( ScreenBase* screen ) = 0;

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

	void Process( ScreenBase* screen )
	{
		for( int y = 0; y < Height; ++y ) {
			for( int x = 0; x < Width; ++x )
				screen->SetPixel( x, y );
		}
	}
};

class RotateColumn: public Oper
{
public:
	int X;
	int Count;

	RotateColumn( int x, int count ) :
		X( x ), Count( count )
	{

	}

	void Process( ScreenBase* screen )
	{
		for( int m = 0; m < Count; ++m ) {

			auto l = screen->GetPixel( X, screen->GetHeight() - 1 );
			for( int y = screen->GetHeight() - 1; y > 0; --y ) {
				screen->SetPixel( X, y, screen->GetPixel( X, y - 1 ) );
			}
			screen->SetPixel( X, 0, l );
		}
	}
};

class RotateRow: public Oper
{
public:
	int Y;
	int Count;

	RotateRow( int y, int count ) :
		Y( y ), Count( count )
	{

	}

	void Process( ScreenBase* screen )
	{
		for( int m = 0; m < Count; ++m ) {

			auto l = screen->GetPixel( screen->GetWidth() - 1, Y );
			for( int x = screen->GetWidth() - 1; x > 0; --x ) {
				screen->SetPixel( x, Y, screen->GetPixel( x - 1, Y ) );
			}
			screen->SetPixel( 0, Y, l );
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

void Process( ScreenBase* screen, const std::vector<std::string>& lines )
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
	Screen<50, 6> screen;

	std::vector<std::string> lines;
	Read( "input.txt", &lines );

	Process( &screen, lines );

	return screen.Count();
}

int Day8_Part2( int argc, char* argv[] )
{
	Screen<50, 6> screen;

	std::vector<std::string> lines;
	Read( "input.txt", &lines );

	Process( &screen, lines );

	screen.Print( std::cout );

	return -1;
}

int Day8_Test( void )
{
	Screen<5, 6> screen;

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
