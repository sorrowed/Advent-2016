/*
 * Operations.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */
#include "Operations.h"
#include "Exchangers.h"

bool ExchangeOp::Execute()
{
	auto s = GetExchanger( Bots, Id );

	shared_ptr<Exchanger> low = nullptr;
	if( LowType == "bot" )
		low = GetExchanger( Bots, LowDst );
	else if( LowType == "output" )
		low = GetExchanger( Outputs, LowDst );

	shared_ptr<Exchanger> high = nullptr;
	if( HighType == "bot" )
		high = GetExchanger( Bots, HighDst );
	else if( HighType == "output" )
		high = GetExchanger( Outputs, HighDst );

	return s->Exchange( low, high );
}

bool AssignOp::Execute()
{
	auto s = GetExchanger( Bots, Id );

	return s->Assign( Value );
}
