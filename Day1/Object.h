/*
 * Object.h
 *
 *  Created on: Dec 2, 2016
 *      Author: tom
 */

#ifndef OBJECT_H_
#define OBJECT_H_

enum Heading_e {
	North = 0, West, South, East
};

#include <string>
#include <cstring>
struct Location_t {
	int X;
	int Y;

	std::string toString()
	{
		char buffer[ 16 + 1 ] = {0};
		std::sprintf( buffer, "%08X%08X", X, Y );

		return std::string( buffer );
	}
};

enum Rotation_e {
	Left, Right
};

struct Movement {
	Rotation_e rotation;
	int distance;

	void Parse(const std::string& s) {
		std::string r = s.substr(0, 1);
		if (r == "R" || r == "r")
			rotation = Rotation_e::Right;
		else if (r == "L" || r == "l")
			rotation = Rotation_e::Left;

		std::string d = s.substr(1);
		distance = (int)strtol(d.c_str(), 0, 10);
	}
};

#include <map>

class Object {
public:
	Object() {
		Reset();
	}

	virtual ~Object() {
	}

	Location_t Location;
	Heading_e Heading;

	void Reset() {
		Location.X = Location.Y = 0;
		Heading = Heading_e::North;
		twice = nullptr;
		locations.clear();
		addLocation();
	}

	void Move(const Movement& m) {
		if (m.rotation == Rotation_e::Left) {
			if (Heading != Heading_e::East)
				Heading = Heading_e(Heading + 1);
			else
				Heading = Heading_e::North;

		} else if (m.rotation == Rotation_e::Right) {
			if (Heading != Heading_e::North)
				Heading = Heading_e(Heading - 1);
			else
				Heading = Heading_e::East;

		}

		switch (Heading) {
		case Heading_e::North:
			for (int i = 0; i < m.distance; ++i) {
				++Location.Y;
				addLocation();
			}
			break;
		case Heading_e::West:
			for (int i = 0; i < m.distance; ++i) {
				--Location.X;
				addLocation();
			}
			break;
		case Heading_e::South:
			for (int i = 0; i < m.distance; ++i) {
				--Location.Y;
				addLocation();
			}
			break;
		case Heading_e::East:
			for (int i = 0; i < m.distance; ++i) {
				++Location.X;
				addLocation();
			}
			break;
		}
	}
private:

	void addLocation(){
		std::string key = Location.toString();

		if( locations.find( key ) == locations.end() )
			locations[ key ] = Location;
		else
		if( twice == nullptr )
		{
			twice = new Location_t();
			*twice = Location;
		}
	}

	std::map<std::string, Location_t> locations;
	Location_t* twice;
};

#endif /* OBJECT_H_ */
