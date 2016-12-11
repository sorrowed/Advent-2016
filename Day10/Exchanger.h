/*
 * Exchanger.h
 *
 *  Created on: Dec 11, 2016
 *      Author: tom
 */

#ifndef DAY10_EXCHANGER_H_
#define DAY10_EXCHANGER_H_

#include <vector>
#include <utility>
#include <memory>
#include <algorithm>

using std::vector;
using std::pair;
using std::shared_ptr;
using std::swap;
using std::make_pair;

struct Exchanger
{
	int Id;
	int Low;
	int High;

	vector<pair<int, int> > Exchanges;

	bool CanAssign( void ) const
	{
		return Low == -1 || High == -1;
	}

	bool CanExchange( void ) const
	{
		return Low != -1 && High != -1;
	}

	Exchanger( int id ) :
			Id( id ), Low( -1 ), High( -1 )
	{
	}

	bool Assign( int value )
	{
		if( !CanAssign() )
			return false;

		if( High == -1 )
			High = value;
		else
			Low = value;

		if( High < Low )
			swap( High, Low );

		return true;
	}

	bool Exchange( shared_ptr<Exchanger> low, shared_ptr<Exchanger> high )
	{
		if( !CanExchange() )
			return false;

		if( !low->CanAssign() || !high->CanAssign() )
			return false;

		Exchanges.push_back( make_pair( Low, High ) );

		TransferLow( low );
		TransferHigh( high );

		return true;
	}

private:
	void TransferLow( shared_ptr<Exchanger> target )
	{
		target->Assign( Low );

		Low = -1;
	}

	void TransferHigh( shared_ptr<Exchanger> target )
	{
		target->Assign( High );

		High = -1;
	}
};




#endif /* DAY10_EXCHANGER_H_ */
