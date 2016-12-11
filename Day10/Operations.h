/*
 * Operations.h
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#ifndef DAY10_OPERATIONS_H_
#define DAY10_OPERATIONS_H_

#include "skip.h"

using std::string;
using std::stringstream;
using std::getline;

struct Op
{
	virtual bool Execute() = 0;
};

struct ExchangeOp: Op
{
	int Id;

	string LowType;
	int LowDst;

	string HighType;
	int HighDst;

	void Parse( stringstream& str )
	{
		str >> Id;
		skip( str, ' ', 4 );

		getline( str, LowType, ' ' );
		str >> LowDst;
		skip( str, ' ', 4 );

		getline( str, HighType, ' ' );
		str >> HighDst;
	}

	bool Execute();
};

struct AssignOp: Op
{
	int Id;
	int Value;

	void Parse( stringstream& str )
	{
		str >> Value;
		skip( str, ' ', 4 );

		str >> Id;
	}

	bool Execute();
};



#endif /* DAY10_OPERATIONS_H_ */
