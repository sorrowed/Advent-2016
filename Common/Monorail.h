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

struct Op
{
	std::string Token;
	std::string Args;

	virtual void Parse( const std::string& token, std::stringstream& str ) = 0;
	virtual void Execute() = 0;

	virtual int Arguments()const = 0;
};

struct Copy: public Op
{
	std::string src;
	std::string dst;

	Copy( const std::string& token, std::stringstream& str );

	void Parse( const std::string& token, std::stringstream& str );
	void Execute();

	int Arguments()const { return 2; }
};

struct Jnz: public Op
{
	std::string src;
	std::string Offs;

	Jnz( const std::string& token, std::stringstream& str ) ;

	void Parse( const std::string& token, std::stringstream& str );

	void Execute();

	int Arguments()const { return 2; }
};

struct Inc: public Op
{
	std::string srcReg;
	Inc( const std::string& token, std::stringstream& str ) ;
	void Parse( const std::string& token, std::stringstream& str );
	void Execute();
	int Arguments()const { return 1; }
};

struct Dec: public Op
{
	std::string srcReg;

	Dec( const std::string& token, std::stringstream& str ) ;
	void Parse( const std::string& token, std::stringstream& str );
	void Execute();

	int Arguments()const { return 1; }
};

struct Tgl: public Op
{
	std::string srcReg;

	Tgl( const std::string& token, std::stringstream& str );
	void Parse( const std::string& token, std::stringstream& str );

	int Arguments()const { return 1; }

	void Execute();
};

#endif /* COMMON_MONORAIL_H_ */
