/*
 * main.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: tom
 */

#include "Day1/Day1.h"
#include "Day2/PartOne.h"
#include "Day2/PartTwo.h"
#include "Day3/PartOne.h"
#include "Day4/PartOne.h"

int main( int argc, char *argv[] )
{
	Day01::Day1_Test();
	Day02_01::Day2_Part1_Test();
	Day02_02::Day2_Part2_Test();

	Day3_Test();
	Day4_Test();

	Day4_Part1( argc, argv );
	return Day4_Part2( argc, argv );
}
