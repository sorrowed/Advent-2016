/*
 * Object.h
 *
 *  Created on: Dec 2, 2016
 *      Author: tom
 */

#ifndef OBJECT_H_
#define OBJECT_H_

enum Heading_e
{
	North = 0,
	West,
	South,
	East
};

struct Location_t
{
	long X;
	long Y;
};

enum Rotation_e
{
	Left,
	Right
};

#include <string>

struct Movement
{
	Rotation_e rotation;
	long distance;

	void Parse( const std::string& s )
	{
		std::string r = s.substr( 0,1 );
		if( r == "R" || r == "r" )
			rotation = Rotation_e::Right;
		else
		if( r == "L" || r == "l" )
			rotation = Rotation_e::Left;

		std::string d = s.substr( 1 );
		distance = strtol( d.c_str(), 0, 10 );
	}
};

class Object {
public:
	Object(){
		Reset();
	}

	virtual ~Object(){}

	Location_t Location;
	Heading_e	Heading;

	void Reset()
	{
		Location.X = Location.Y = 0;
		Heading = Heading_e::North;
	}

	void Move( const Movement& m )
	{
		if( m.rotation == Rotation_e::Left )
		{
			if( Heading != Heading_e::East )
				Heading = Heading_e( Heading + 1 );
			else
				Heading = Heading_e::North;

		}
		else
		if( m.rotation == Rotation_e::Right )
		{
			if( Heading != Heading_e::North )
				Heading = Heading_e( Heading - 1 );
			else
				Heading = Heading_e::East;

		}

		switch( Heading )
		{
			case Heading_e::North:
				Location.Y += m.distance;
				break;
			case Heading_e::West:
				Location.X -= m.distance;
				break;
			case Heading_e::South:
				Location.Y -= m.distance;
				break;
			case Heading_e::East:
				Location.X += m.distance;
				break;
		}
	}
};

#endif /* OBJECT_H_ */
