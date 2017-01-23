/*
 * Monorail.h
 *
 *  Created on: Jan 23, 2017
 *      Author: tom
 */

#ifndef COMMON_MONORAIL_H_
#define COMMON_MONORAIL_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class Op;

struct Register
{
	char Id;
	long Value;
};

typedef std::map<std::string, Register> RegMap;
typedef std::vector<std::shared_ptr<Op>> OpQue;

extern OpQue ops;
extern OpQue::iterator curOp;
extern RegMap registers;

std::shared_ptr<Op> Parse( const char* input );

static inline
void Reset()
{
	registers.clear();
	ops.clear();
}

static inline
bool IsReg( const std::string& token )
{
	const char* regs = "abcdefghijklmnopqrstuvwxyz";

	return token.find_first_of( regs, 0 ) != std::string::npos;
}

enum OpType_e
{
	DontCare = 0,
	Inc,
	Jnz
};

struct Op
{
	OpType_e	OpType;
	std::string Args;

	Op( ) : OpType( OpType_e::DontCare ){}

	Op( OpType_e type ) : OpType( type ){}

	virtual void Parse( std::stringstream& str ) = 0;
	virtual void Execute() = 0;

	virtual int Arguments()const = 0;
};

struct Copy1: public Op
{
	int srcVal;
	std::string dstReg;

	Copy1( std::stringstream& str ) :
		Op()
	{
		Parse( str );

		std::stringstream a;
		a << srcVal;
		a << ' ';
		a << dstReg;

		Args = a.str();
	}

	void Parse( std::stringstream& str )
	{
		str >> srcVal;
		str >> dstReg;
	}

	void Execute()
	{
		registers[ dstReg ].Value = srcVal;
		curOp++;
	}

	int Arguments()const { return 2; }
};

struct Copy2: public Op
{
	std::string srcReg;
	std::string dstReg;

	Copy2( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		str >> dstReg;

		Args = srcReg + ' ' + dstReg;
	}

	void Execute()
	{
		registers[ dstReg ].Value = registers[ srcReg ].Value;
		curOp++;
	}

	int Arguments()const { return 2; }
};

struct Jnz: public Op
{
	std::string src;
	int Offs;

	Jnz( std::stringstream& str ) :
		Op( OpType_e::Jnz )
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> src;
		str >> Offs;

		std::stringstream a;
		a << src;
		a << ' ';
		a << Offs;

		Args = a.str();

	}

	void Execute()
	{
		int value = 1;
		if( IsReg( src ) )
			value = registers[ src ].Value;
		else
		{
			std::stringstream s( src );
			s >> value;
		}

		curOp += value;
	}

	int Arguments()const { return 2; }
};

struct Inc: public Op
{
	std::string srcReg;
	Inc( std::stringstream& str ) :
		Op( OpType_e::Inc )
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;
		Args = srcReg;
	}

	void Execute()
	{
		registers[ srcReg ].Value++;
		curOp++;
	}

	int Arguments()const { return 1; }
};

struct Dec: public Op
{
	std::string srcReg;

	Dec( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;

		Args = srcReg;
	}

	void Execute()
	{
		registers[ srcReg ].Value--;
		curOp++;
	}

	int Arguments()const { return 1; }
};

/*
tgl x toggles the instruction x away (pointing at instructions like jnz does: positive means forward; negative means backward):

    For one-argument instructions, inc becomes dec, and all other one-argument instructions become inc.
    For two-argument instructions, jnz becomes cpy, and all other two-instructions become jnz.
    The arguments of a toggled instruction are not affected.
    If an attempt is made to toggle an instruction outside the program, nothing happens.
    If toggling produces an invalid instruction (like cpy 1 2) and an attempt is later made to execute that instruction, skip it instead.
    If tgl toggles itself (for example, if a is 0, tgl a would target itself and become inc a), the resulting instruction is not executed until the next time it is reached.
 */
struct Tgl: public Op
{
	std::string srcReg;

	Tgl( std::stringstream& str ) :
		Op()
	{
		Parse( str );
	}

	void Parse( std::stringstream& str )
	{
		str >> srcReg;

		Args = srcReg;
	}

	int Arguments()const { return 1; }

	void Execute()
	{
		int offset = registers[ srcReg ].Value;

		auto op = (curOp + offset );
		if( op >= ops.begin() && op < ops.end() )
		{
			std::stringstream str;

			if( (*op)->Arguments() == 1 ){

				if( (*op)->OpType == OpType_e::Inc )
				{
					str << "dec ";
					str << (*op)->Args;
				}
				else
				{
					str << "inc ";
					str << (*op)->Args;
				}
			}
			else
			if( (*op)->Arguments() == 2 ){
				if( (*op)->OpType == OpType_e::Jnz )
				{
					str << "cpy ";
					str << (*op)->Args;
				}
				else
				{
					str << "jnz ";
					str << (*op)->Args;
				}
			}

			auto o = ::Parse( str.str().c_str() );
			*op = o;

			if( offset != 0 )
				curOp++;
		}
		else
			curOp++;
	}
};

#endif /* COMMON_MONORAIL_H_ */
