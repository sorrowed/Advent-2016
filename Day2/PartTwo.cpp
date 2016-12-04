/*
 * PartTwo.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: tom
 */

#include "PartTwo.h"
#include "Location.h"

#include <cstring>
#include <cassert>

class Object2
{
public:
	Location_t Location;

	Object2()
	{
		Reset();
	}

	void Reset()
	{
		Location.X = 0;
		Location.Y = 2;
	}

	void Move( char dir )
	{
		auto l = &Location;

		switch( dir )
		{
		case 'U':
			if( l->Y == 0 || l->Y == 1 )
				++l->Y;
			else if( l->Y == 2 && l->X > 0 && l->X < 4 )
				++l->Y;
			else if( l->Y == 3 && l->X == 2 )
				++l->Y;
			break;

		case 'R':
			if( l->X == 0 || l->X == 1 )
				++l->X;
			else if( l->X == 2 && l->Y > 0 && l->Y < 4 )
				++l->X;
			else if( l->X == 3 && l->Y == 2 )
				++l->X;
			break;

		case 'D':
			if( l->Y == 3 || l->Y == 4 )
				--l->Y;
			else if( l->Y == 2 && l->X > 0 && l->X < 4 )
				--l->Y;
			else if( l->Y == 1 && l->X == 2 )
				--l->Y;
			break;

		case 'L':
			if( l->X == 3 || l->X == 4 )
				--l->X;
			else if( l->X == 2 && l->Y > 0 && l->Y < 4 )
				--l->X;
			else if( l->X == 1 && l->Y == 2 )
				--l->X;
			break;
		}
	}
};

static void Process( Object2* obj, Location_t locations[], const char* movements[], int count );
static void CreateCode( char code[], Location_t locations[], int count );

int Day2_Part2( int argc, char* argv[] )
{
	const char* input[] = {
		"RLRLLLULULULUUDUULULRDDLURURDDLDUUDDLRDDUUUDDRUDLRRDDUDUUDULUDRDULRUDRULRDRUDLDDULRRDLDRLUDDLLDRDDDUDDLUDUDULDRLLDRLULRLURDLULRUUUDRULLUUDLRDLDDUDRRRLDLRUUURRLDDRRRURLLULDUULLDRLRDLLDURDLDDULLDDLDLUURRRURLRURLLRRDURLDUDDLULUUULULLLDRRRRRLULRDUDURURLULRURRRLLUURDURULRRUULDRDLULDLLUDLUDRLUDLRRLDLLDLDUDDLULLDRULRLRULDURRDLDLLUDRLLDRRDLDUDUURUURDUUDDDLDLDDRDLUDLDUUUUDLDRLRURDLURURDLLLUURURDRDLUDLLRUDULLLDLULLULLDLDDRDRRRUDDDUDDDDRULLLLRLDDLLRDRLLLRRLDRRUDRUUURLLLRULRRDURDLDRLDDUUDUUURRLRRUDLDLDDRUDLULLUUDUUUDLUDDRUULLLURUDDDDLRUDDLLLRUR",
		"LDLRLDDDLUDRDRRUDUURLRULLUDDRLURLUULDLLRLLUDLRLRUDLULRLRRLRURLDDDURUDUUURDRLDDLUUUDRUDUDDDLLURLLULRUULLUDRULUDDULDUDUDULLDRUUUULRDUUDLUDURDLLRLLRLUUDUUDRLLLRULUURUDLDRLLDUDLDDRULDULDURRLDDDUDUDDRUDUDRDURLLLLLULDRDDLLUDULLLUDRURLDLDLDULLDDRURRLUDDRLURLULRLDDDUUUURLRDLRURDDURLDLRRLLRLRLUURRLLDDLDRLRDUDDLLDDDURUUDURLRRDUULRRDDRRUULDRLRUDRRLDDRLDRULLDLDURRULDURRRDLRRLRLLLRLDRLLULRRLLLLLDLDDULDLLDLLDUUDDRLURUUUUULRDDLRDLRDRDRDLUDDLDDRULLUDDRLDLLUDRLUURRLUDURURLLRURRURRLRLLRLURURDDDDRRLURDUULLUU",
		"LLRRDURRDLDULRDUDLRDRDRURULDURUDRRURDDDRLDLDRDRDRDRULDUURLULDDUURUULUDULLDUDLLLLDLLLDRLUUULLULDDRRUDDULLLULRDRULDDULDUDRDDLUUURULDLLUDUUUUURUDLLDRDULLRULLDURDRLLDLDRDDURUULUDURRRUULLDUUDDURDURLDLRRLLDURDDLRRRUDLRRRDLDRLUDLUDRDRLDDLLLRLLRURDLRDUUUURRLULDDLDLLLUDRDRLRRDURDDLURDLDDDULLLRRLDDDRULDDDLRRDULUUUDRRULDDLLLURDRRLLLUULDRRRUURRDDLULDRLULDDDLDULDRRRULRULLURLURULLLLRUDRRRDRDRDLDULURLRRRRLRUDDRRRUURUURLLRURURUURRURRDLDLLUDRRRDUDDRDURLLRLRRULD",
		"DULRRDRLRLUDLLURURLLRLRDLLDLLDRDUURLRUUUDLLDUUDDUULDUULLRUDRURLUDRDLRUDDDLULUDLLDRULULLLDRRULDLLUURLRRRLDRDLDRURRRRDLRUUDULLRLLLDLRUDLDUUDRLDLRDRLRDLDDDUDLRUDLDDLLLDRLLRRUUDRDDUUURURRRUUDLRRDDRUDLDDULULDLRRLRDDUDRUURRUULURLURUDRRURRRULDDDDURDLUUULUULULRDLRRRRRURURRLRUULDUUURRDRRDLDUUUULLULLLLUDLUUDUURRDLDLRRRLUUURULDULDLDRLLURDRUULLLLLULLLDRURURRUDRRRRUDUDUDRUDUDRDRULUUDRURDDUUDLDLDUURUDURLRLRRDRDRDLLDUDDULLRDLDDRLLDLRDURDDULLLDLLLULDLUUUDLDRDLURUURDDLRDLLLLLRLURDLLLULLRRLU",
		"DUULULUUDUDLLRLRURULLDLRRLURDLLDUDUDDRURRLUDULULDRRDRLUULUDDLUURURDLDDDRDRUDURLDDLUDUURULRRUUDRLURRLRLDURRRULRLDDDRUDDDDDUDDULLLRRLLDULDRULUDLRRDLLUDRDLDULRLLLUULLRULRLLLLUDDRRDRLULDLDLURDDRUDDLDLDLDRULDLLDDUUDULUULULLURDURRLLUDRULLRDUDRDRURDRDRDURUUDULDDRURUDLLUUDUUDURDLRDRURUDRUURLUUURLRLUDRUDRUURLLUDRLURDDURRUDRDRLRRLDDDRDDLUUUDDLULDUURUDUDLLDRURDURRDULRLURRDLDDRLUDRLDLRLDDUURRULDDLDUDDLRDULLDDDLDUUUUDLRUDUDLDRDLRDDLDLRLLUDDRRLUDLDUUULLDDRLRRDLRRRRUDDLRLLULRLRDURDUDDRRULLDDLDLRRDLLULDURURDDURLRLULULURRUDUDRDLURULDUDLUULDUUURLLRUDLLRDLRUDRLULDUDRRDUUDUUULUUUDDRUD"
	};

	Object2 obj;
	Location_t locations[ 5 ];
	char code[ 5 ];

	Process(&obj, locations, input, 5);

	CreateCode(code, locations, 5);

	return 0;
}

int Day2_Part2_Test1( int argc, char* argv[] )
{
	const char* input[] = {
		"ULL",
		"RRDDD",
		"LURDL",
		"UUUUD"
	};

	Object2 obj;
	Location_t locations[ 4 ];
	char code[ 4 ];

	Process(&obj, locations, input, 4);

	CreateCode(code, locations, 4);

	assert(!strncmp(code, "5DB3", 4));

	return 0;
}

void Process( Object2* obj, Location_t locations[], const char* movements[], int count )
{
	for( int i = 0; i < count; ++i ){
		auto m = movements[ i ];

		int len = strlen(m);
		for( int j = 0; j < len; ++j )
			obj->Move(m[ j ]);

		locations[ i ] = obj->Location;
	}
}

void CreateCode( char code[], Location_t locations[], int count )
{
	for( int i = 0; i < count; ++i ){
		auto l = &locations[ i ];

		int ix = ( 5 * l->Y ) + l->X;

		code[ i ] = "nnDnnnABCn56789n234nnn1nn"[ ix ];
	}
}
