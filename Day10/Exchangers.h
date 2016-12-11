/*
 * Exhangers.h
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#ifndef DAY10_EXCHANGERS_H_
#define DAY10_EXCHANGERS_H_

#include "Exchanger.h"

#include <map>
#include <memory>

using std::map;
using std::shared_ptr;

typedef map<int, shared_ptr<Exchanger>> ExchangeMap;

extern ExchangeMap Bots;
extern ExchangeMap Outputs;

shared_ptr<Exchanger> GetExchanger( ExchangeMap &m, int id );

#endif /* DAY10_EXCHANGERS_H_ */
