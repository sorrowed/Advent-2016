/*
 * main.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: tom
 */

#include "Day1/Day1.h"
#include "Day2/Day2_1.h"
#include "Day2/Day2_2.h"
#include "Day3/Day3.h"
#include "Day4/Day4.h"

int main( int argc, char *argv[] )
{
	Day1::Day1_Test();
	Day2_1::Day2_Part1_Test();
	Day2_2::Day2_Part2_Test();

	Day3_Test();
	Day4_Test();

	Day4_Part1( argc, argv );
	return Day4_Part2( argc, argv );
}
