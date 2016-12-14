/*
 * Day14.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: tom
 */
#include "md5.h"


int Day14_Part1( int argc, char* argv[] )
{
	return -1;
}

int Day14_Part2( int argc, char* argv[] )
{
	return -1;
}

#include <cstdio>
#include <cstring>

int Day14_Test( void )
{
	const char* salt = "abc";

	uint8_t digest[16]= { 0 };
	char src[ 256 ];

	int index = 18;
	sprintf( src, "%s%d", salt, index );
	md5( (uint8_t*)src, strlen( src ), digest );

	return -1;
}


