/*
 * RoomId.h
 *
 *  Created on: Dec 4, 2016
 *      Author: tom
 */

#ifndef DAY4_ROOMID_H_
#define DAY4_ROOMID_H_

#include <string>
#include <vector>
#include <sstream>

class RoomId
{
public:
	std::string code;
	int id;
	std::string chk;

	RoomId( const std::string& id )
	{
		Parse( id );
	}

	virtual ~RoomId()
	{
	}

	void Parse( const std::string& src );

	bool IsValid( void );

	std::string Decrypt( void );

private:
	void Split( const std::string &s, char delim, std::vector<std::string> &elems );
};

#endif /* DAY4_ROOMID_H_ */
