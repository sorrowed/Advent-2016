/*
 * Movement.h
 *
 *  Created on: Dec 4, 2016
 *      Author: tom
 */

#ifndef DAY1_MOVEMENT_H_
#define DAY1_MOVEMENT_H_

#include <string>

enum Rotation_e
{
	Left, Right
};

struct Movement
{
	Rotation_e rotation;
	int distance;

	void Parse( const std::string& s )
	{
		std::string r = s.substr( 0, 1 );
		if( r == "R" || r == "r" )
			rotation = Rotation_e::Right;
		else if( r == "L" || r == "l" )
			rotation = Rotation_e::Left;

		std::string d = s.substr( 1 );
		distance = (int) strtol( d.c_str(), 0, 10 );
	}
};

#endif /* DAY1_MOVEMENT_H_ */
