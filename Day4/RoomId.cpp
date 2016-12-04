/*
 * RoomId.cpp
 *
 *  Created on: Dec 4, 2016
 *      Author: tom
 */

#include "RoomId.h"

#include <algorithm>
#include <vector>
#include <utility>

void RoomId::Parse( const std::string& src )
{
	std::vector<std::string> tokens;

	Split( src, '-', tokens );
	for( auto it = tokens.begin(); it != tokens.end() - 1; ++it ) {
		if( code.length() > 0 )
			code += '-';
		code += *it;
	}

	//Last token is id + [checksum]
	auto last = tokens[ tokens.size() - 1 ];

	auto chkpos = last.find_first_of( '[' );

	std::istringstream ss( last.substr( 0, chkpos ) );
	ss >> id;

	chk = last.substr( chkpos + 1, last.length() - chkpos - 2 );
}

void RoomId::Split( const std::string &s, char delim, std::vector<std::string> &elems )
{
	std::istringstream ss( s );

	std::string item;
	while( std::getline( ss, item, delim ) )
		elems.push_back( item );
}

bool Compare( const std::pair<int, char>& a, const std::pair<int, char>& b )
{
	if( a.first != b.first )
		return a.first > b.first;

	return a.second < b.second;
}

bool RoomId::IsValid( void )
{
	const char letters[] = "abcdefghijklmnopqrstuvwxyz";

	std::vector<std::pair<int, char> > results;

	for( auto c : letters ) {
		int count = std::count_if( code.begin(), code.end(), [c]( const char& ch ) {return ch != '-' && ch == c;} );

		if( count > 0 )
			results.push_back( std::make_pair( count, c ) );
	}

	std::stable_sort( results.begin(), results.end(), Compare );

	bool result = true;

	for( int i = 0; i < 5; ++i )
		result &= (chk[ i ] == results[ i ].second);

	return result;
}

std::string RoomId::Decrypt( void )
{
	std::stringstream result;

	int rot = id % 26;

	for( auto c : code ) {
		if( c == '-' )
			result << ' ';
		else {
			int n = c + rot;

			if( n > 'z' )
				n = 'a' + (n - 'z' - 1);

			result << (char) n;
		}
	}

	return result.str();
}
