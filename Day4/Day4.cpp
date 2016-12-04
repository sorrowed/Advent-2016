#include "RoomId.h"

#include <cassert>
#include <vector>
#include <fstream>

#include "Day4.h"

static void Read( std::vector<RoomId>* dst );

int Day4_Part1(int argc, char* argv[])
{
	std::vector<RoomId> ids;

	Read( &ids );

	int sum = 0;
	for( auto it = ids.begin(); it != ids.end(); ++it )
	{
		if( it->IsValid( ))
			sum += it->id;
	}

	return sum;
}

int Day4_Part2(int argc, char* argv[])
{
	int id = -1;

	std::vector<RoomId> ids;
	Read( &ids );

	for( auto it = ids.begin(); it != ids.end(); ++it )
	{
		if( !it->IsValid( ))
			continue;

		auto d = it->Decrypt();

		if( d.find( "northpole" ) != std::string::npos )
			id = it->id;
	}

	return id;
}

void Read( std::vector<RoomId>* dst )
{
	dst->clear();

	std::ifstream file( "input.txt", std::ifstream::in );
	std::string line;

	std::getline( file, line );
	while( file.good() ){
		RoomId id( line );
		dst->push_back( id );
		std::getline( file, line );
	}
}

int Day4_Test( void ){

	RoomId r1( "aaaaa-bbb-z-y-x-123[abxyz]" );
	assert( r1.code == "aaaaa-bbb-z-y-x" );
	assert( r1.id == 123 );
	assert( r1.chk == "abxyz" );
	assert( r1.IsValid() );

	RoomId r2( "a-b-c-d-e-f-g-h-987[abcde]" );
	assert( r2.code == "a-b-c-d-e-f-g-h" );
	assert( r2.id == 987 );
	assert( r2.chk == "abcde" );
	assert( r2.IsValid() );

	RoomId r3( "not-a-real-room-404[oarel]" );
	assert( r3.code == "not-a-real-room" );
	assert( r3.id == 404 );
	assert( r3.chk == "oarel" );
	assert( r3.IsValid() );

	RoomId r4( "totally-real-room-200[decoy]" );
	assert( r4.code == "totally-real-room" );
	assert( r4.id == 200 );
	assert( r4.chk == "decoy" );
	assert( !r4.IsValid() );

	RoomId r5( "qzmt-zixmtkozy-ivhz-343[decoy]" );
	assert( r5.code == "qzmt-zixmtkozy-ivhz" );
	assert( r5.id == 343 );

	assert( r5.Decrypt() =="very encrypted name" );

	return -1;
}
