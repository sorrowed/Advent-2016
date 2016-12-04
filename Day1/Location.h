/*
 * Location.h
 *
 *  Created on: Dec 4, 2016
 *      Author: tom
 */

#ifndef DAY1_LOCATION_H_
#define DAY1_LOCATION_H_

#include <string>
#include <cstring>
struct Location_t
{
	int X;
	int Y;

	std::string toString()
	{
		char buffer[ 16 + 1 ] = { 0 };
		std::sprintf( buffer, "%08X%08X", X, Y );

		return std::string( buffer );
	}
};

#endif /* DAY1_LOCATION_H_ */
