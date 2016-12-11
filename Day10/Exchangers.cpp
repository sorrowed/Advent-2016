/*
 * Exchangers.cpp
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */
#include "Exchangers.h"

#include <utility>

using std::make_shared;

ExchangeMap Bots;
ExchangeMap Outputs;

shared_ptr<Exchanger> GetExchanger( ExchangeMap &m, int id )
{
	if( m.count( id ) == 0 )
		m[id] = make_shared<Exchanger>( id );

	return m[id];
}

