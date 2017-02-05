/*
 * Day25.cpp
 *
 *  Created on: Feb 1, 2017
 *      Author: tom
 */
#include "Day25.h"
#include "Monorail.h"

#include <iostream>

int Day25_Test( void )
{
	return -1;
}

/*
*/
/*!
 * This is the translated version of the input to C, with the multiplication loop optimized by hand.
 * The rest has too many jumps to even think about optimizing. I tried and failed.
 * The input:
	"cpy a d",
	"cpy 9 c",
	"cpy 282 b",
	"inc d",
	"dec b",
	"jnz b -2",
	"dec c",
	"jnz c -5",
	"cpy d a",
	"jnz 0 0",
	"cpy a b",
	"cpy 0 a",
	"cpy 2 c",
	"jnz b 2",
	"jnz 1 6",
	"dec b",
	"dec c",
	"jnz c -4",
	"inc a",
	"jnz 1 -7",
	"cpy 2 b",
	"jnz c 2",
	"jnz 1 4",
	"dec b",
	"dec c",
	"jnz 1 -4",
	"jnz 0 0",
	"out b",
	"jnz a -19",
	"jnz 1 -21",
 *
 */
int Day25_Part1_Translated( int start, int finish )
{
	int a, b = 0, c = 0, d = start, out = 1, count = 0;

	c = 9;
	b = 282;

	d = start + ( 9 * 282 );

	do {
		a = d;
		do {
			b = a;
			a = 0;
label2:
			c = 2;
			do {
				if( b )
					goto label1;
				goto label5;
label1:
				--b;
				--c;
			}
			while( c );

			++a;
			goto label2;

label5:
			b = 2;
label4:
			if( c )
				goto label3;
			goto label8;

label3:
			--b;
			--c;

			goto label4;

label8:
			++count;
			if( b == out || count > finish )
				return count;

			out = b;

		}
		while( a );
	}
	while( true );
}

int Day25_Part1( int argc, char* argv[] )
{
	int start = 0;
	int c = 0;
	int finish = 10000000;

	do {
		std::cout << "Start with start=" << start << " : ";

		c = Day25_Part1_Translated( start++, finish );

		std::cout << " toggles : " << c << std::endl;

	}while( c < finish );

	// Should yield 192
	return start;
}

int Day25_Part2( int argc, char* argv[] )
{
	int win = 25;

	return win;
}




