/*
 * Common.h
 *
 *  Created on: Dec 7, 2016
 *      Author: tom
 */

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#include <string>
#include <vector>
#include <fstream>

static inline
void Read( const char* path, std::vector<std::string>* dst )
{
	dst->clear();

	std::ifstream file( path, std::ifstream::in );
	std::string line;

	std::getline( file, line );
	while( file.good() ) {
		dst->push_back( line );
		std::getline( file, line );
	}
}


#endif /* COMMON_COMMON_H_ */
