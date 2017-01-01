/*
 * Day15.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: tom
 */

/*
Disc #1 has 7 positions; at time=0, it is at position 0.
Disc #2 has 13 positions; at time=0, it is at position 0.
Disc #3 has 3 positions; at time=0, it is at position 2.
Disc #4 has 5 positions; at time=0, it is at position 2.
Disc #5 has 17 positions; at time=0, it is at position 0.
Disc #6 has 19 positions; at time=0, it is at position 7.
 */

struct Disc
{
	int slots;
	int start;

	int current;

	void Reset()
	{
		current = start;
	}

	void Advance( int time )
	{
		current = (current + time) % slots;
	}
};

void Reset( Disc discs[], int count, int time )
{
	for( int i = 0; i < count; ++i ){
		discs[ i ].Reset();
		discs[ i ].Advance( time );
	}
}

int Day15_Part1( int argc, char* argv[] )
{
	Disc discs[6] = { { 7, 0 }, { 13, 0 }, { 3, 2 }, { 5, 2 }, { 17, 0 }, { 19, 7 } };

	bool capsule = false;

	int time = 0;

	Reset( discs, 6, time );
	while( !capsule ){

		capsule = true;
		for( int n = 0; capsule && n < 6; ++n ){
			if( discs[ n ].current != 0 )
				capsule = false;
			else
			for( auto& d : discs )
				d.Advance( 1 );
		}

		if( !capsule ){
			++time;
			Reset( discs, 6, time );
		}
	}
	// Should yield 121834
	return time-1;
}

int Day15_Part2( int argc, char* argv[] )
{
	Disc discs[7] = { { 7, 0 }, { 13, 0 }, { 3, 2 }, { 5, 2 }, { 17, 0 }, { 19, 7 }, { 11, 0 } };

	bool capsule = false;

	int time = 0;

	Reset( discs, 7, time );
	while( !capsule ){

		capsule = true;
		for( int n = 0; capsule && n < 7; ++n ){
			if( discs[ n ].current != 0 )
				capsule = false;
			else
			for( auto& d : discs )
				d.Advance( 1 );
		}

		if( !capsule ){
			++time;
			Reset( discs, 7, time );
		}
	}
	// Should yield 3208099
	return time-1;
}

int Day15_Test( void )
{
/*
Disc #1 has 5 positions; at time=0, it is at position 4.
Disc #2 has 2 positions; at time=0, it is at position 1.
 */
	Disc discs[2] = { { 5, 4 }, { 2, 1 } };

	bool capsule = false;

	int time = 0;

	Reset( discs, 2, time );
	while( !capsule ){

		capsule = true;
		for( int n = 0; capsule && n < 2; ++n ){
			if( discs[ n ].current != 0 )
				capsule = false;
			else
			for( auto& d : discs )
				d.Advance( 1 );
		}

		if( !capsule ){
			++time;
			Reset( discs, 2, time );
		}
	}

	// Time - 1 because the capsule takes one second to fall to first disc from pushing the button
	return time - 1;
}



