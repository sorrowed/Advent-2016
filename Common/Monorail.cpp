/*
 * Monorail.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: tom
 */
#include "Monorail.h"

OpQue ops;
OpQue::iterator curOp;
RegMap registers;

std::shared_ptr<Op> Parse( const char* input )
{
	std::shared_ptr<Op> r = nullptr;

	std::stringstream p( input );
	std::string token;
	p >> token;
	if( token == "cpy" ){
		p.ignore( 1 );
		r = std::make_shared<Copy>( token, p );
	}else if( token == "jnz" ){
		p.ignore( 1 );
		r = std::make_shared<Jnz>( token, p );
	}else if( token == "inc" )
		r = std::make_shared<Inc>( token, p );
	else if( token == "dec" )
		r = std::make_shared<Dec>( token, p );
	else if( token == "tgl" )
		r = std::make_shared<Tgl>( token, p );

	return r;
}

