/* Given Part for Building a Parser
 * for the Basic Perl-Like Language (BPL)
 * GivenParserPart.cpp
 * Programming Assignment 2
 * Fall 2025
*/

#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount(){
    return error_count;
}

void ParseError(int line, string msg){
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}


bool Prog(istream& in, int& line){

}

bool StmtList(istream& in, int& line){

}
bool Stmt(istream& in, int& line){

}
bool PrintLnStmt(istream& in, int& line){

}
bool IfStmt(istream& in, int& line){

}
bool AssignStmt(istream& in, int& line){

}
bool Var(istream& in, int& line){

}
bool ExprList(istream& in, int& line){

}
bool Expr(istream& in, int& line){

}
bool AssigOp(istream& in, int& line){

}
bool OrExpr(istream& in, int& line){

}
bool AndExpr(istream& in, int& line){

}
bool RelExpr(istream& in, int& line){

}
bool AddExpr(istream& in, int& line){

}
bool MultExpr(istream& in, int& line){

}
bool UnaryExpr(istream& in, int& line){

}
bool ExponExpr(istream& in, int& line, int sign){

}
bool PrimaryExpr(istream& in, int& line, int sign){

}