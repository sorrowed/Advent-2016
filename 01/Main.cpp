/*
 * Main.cpp
 *
 *  C"Reated on: Dec 2", 2016
 *      Autho"R: tom
 */

#include "Main.h"
#include "Object.h"
#include <cassert>

static void Process(Object* obj, const char* movements[], int count);

int main(int argc, char* argv[]) {
	Object obj;

	/*
	 Following R2, L3 leaves you 2 blocks East and 3 blocks North, or 5 blocks away.
	 R2, R2, R2 leaves you 2 blocks due South of your starting position, which is 2 blocks away.
	 R5, L5, R5, R3 leaves you 12 blocks away.
	 */

	const char* m1[] = { "R2", "L3" };
	obj.Reset();
	Process(&obj, m1, 2);
	assert(obj.Location.X == 2);
	assert(obj.Location.Y == 3);

	const char* m2[] = { "R2", "R2", "R2" };
	obj.Reset();
	Process(&obj, m2, 3);
	assert(obj.Location.X == 0);
	assert(obj.Location.Y == -2);

	const char* m3[] = { "R5", "L5", "R5", "R3" };
	obj.Reset();
	Process(&obj, m3, 4);
	assert(obj.Location.X + obj.Location.Y == 12);

	const char* m4[] = { "R2", "L3", "R2", "R4", "L2", "L1", "R2", "R4", "R1",
			"L4", "L5", "R5", "R5", "R2", "R2", "R1", "L2", "L3", "L2", "L1",
			"R3", "L5", "R187", "R1", "R4", "L1", "R5", "L3", "L4", "R50", "L4",
			"R2", "R70", "L3", "L2", "R4", "R3", "R194", "L3", "L4", "L4", "L3",
			"L4", "R4", "R5", "L1", "L5", "L4", "R1", "L2", "R4", "L5", "L3",
			"R4", "L5", "L5", "R5", "R3", "R5", "L2", "L4", "R4", "L1", "R3",
			"R1", "L1", "L2", "R2", "R2", "L3", "R3", "R2", "R5", "R2", "R5",
			"L3", "R2", "L5", "R1", "R2", "R2", "L4", "L5", "L1", "L4", "R4",
			"R3", "R1", "R2", "L1", "L2", "R4", "R5", "L2", "R3", "L4", "L5",
			"L5", "L4", "R4", "L2", "R1", "R1", "L2", "L3", "L2", "R2", "L4",
			"R3", "R2", "L1", "L3", "L2", "L4", "L4", "R2", "L3", "L3", "R2",
			"L4", "L3", "R4", "R3", "L2", "L1", "L4", "R4", "R2", "L4", "L4",
			"L5", "L1", "R2", "L5", "L2", "L3", "R2", "L2" };

	obj.Reset();

	Process(&obj, m4, sizeof(m4) / sizeof(const char*));
}

void Process(Object* obj, const char* movements[], int count) {
	for (int i = 0; i < count; ++i) {

		const char* m = movements[i];

		Movement mov;
		mov.Parse(m);

		obj->Move(mov);
	}
}
