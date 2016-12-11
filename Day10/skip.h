/*
 * skip.h
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#ifndef DAY10_SKIP_H_
#define DAY10_SKIP_H_

#include <sstream>

using std::stringstream;

static inline
stringstream& skip( stringstream& str, char v, int c )
{
	while( str && c ){
		if( str.get() == v )
			--c;
	}
	return str;
}


#endif /* DAY10_SKIP_H_ */
