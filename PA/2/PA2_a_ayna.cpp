/* Given Part for Building a Parser
 * for the Basic Perl-Like Language (BPL)
 * GivenParserPart.cpp
 * Programming Assignment 2
 * Fall 2025
*/

#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

map<Token,string> errors = {
	{RPAREN,"Missing Right Parenthesis of "},
	{LPAREN,"Missing Left Parenthesis of "},
	{LBRACES,"Missing left brace for "},
	{RBRACES,"Missing right brace for "},
	{SEMICOL,"Missing semicolon at end of Statement"}
};
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

using namespace Parser;

static int error_count = 0;

void ParseError(int line, string msg){
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}

bool check_syntax(LexItem lex,int line, Token expected, Token curBlock){
	switch(curBlock){
		case IF:
			if (expected == lex.GetToken()) return true;
			if(expected == RBRACES || expected == LBRACES)
				ParseError(line+1,errors[expected]+"If Statement Clause");
			else if(expected == LPAREN || expected == RPAREN)
				ParseError(line,errors[expected]+"If condition");
			return false;
		case ELSE:
			if (expected == lex.GetToken()) return true;
			if(expected == RBRACES || expected == LBRACES)
				ParseError(line-1,errors[expected]+"an Else-Clause");
			return false;
		case PRINTLN:
			if (expected == lex.GetToken()) return true;
		    if(expected == LPAREN || expected ==RPAREN)
				ParseError(line,errors[expected]+"PrintLn Statement");
			return false;
		default:
			return true;
	}

}

int ErrCount(){
    return error_count;
}




bool Prog(istream& in, int& line){
	if(StmtList(in,line)){
		cout << "Declared Variables:" << endl;
		auto it = defVar.cbegin();
		while(true){
			if(it->second){
				cout << it->first;
			}
			++it;
			if(it != defVar.cend()){
				cout << ", ";
				continue;
			}
			cout << endl << endl;
			break;
		}
		cout << "Done" << endl;
    	
		return true;
	}
	return false;
}

bool StmtList(istream& in, int& line){
	
	LexItem lex;
	Token tok;
	while(Stmt(in,line)){
		lex = GetNextToken(in,line);
		tok = lex.GetToken();
		if(tok == SEMICOL){
			lex = GetNextToken(in,line);
			if(lex.GetToken() == DONE) break;
			else if(lex.GetToken() == ELSE){
				ParseError(line, "Illegal If Statement Else-Clause");
				return false;
			}
			
			else PushBackToken(lex);
			continue;}
		else{
			ParseError(line-1, errors[SEMICOL]);
			return false;
		}
	}
	if(error_count == 0){
		return true;}
	ParseError(line,"Syntactic error in Program Body");	
	return false;

	
}
bool Stmt(istream& in, int& line){
	LexItem lex = GetNextToken(in,line);
	Token tok = lex.GetToken();
	PushBackToken(lex);
	if(tok == IF){
		return(IfStmt(in,line));
	}
	else if(tok == PRINTLN){
		return PrintLnStmt(in,line);
	}
	else if (tok == RBRACES) return false;
	else{
        bool assign = AssignStmt(in,line);
        if(!assign){
            ParseError(line,"Incorrect Assignment Statement");
        }
        return assign;
    }
}
bool PrintLnStmt(istream& in, int& line){
	LexItem lex = GetNextToken(in,line);
	if(check_syntax(lex,line,PRINTLN,PRINTLN)){
		if(check_syntax(GetNextToken(in,line),line,LPAREN,PRINTLN)){
			if(ExprList(in,line)){
				if(check_syntax(GetNextToken(in,line),line,RPAREN,PRINTLN)){
					return true;
				}
			}
		}
	}
	else{
		PushBackToken(lex);
	}
	ParseError(line,"Incorrect PrintLn Statement");
	return false;

}
bool IfStmt(istream& in, int& line){
	LexItem lex = GetNextToken(in,line);
	if(check_syntax(lex,line,IF,IF)){
		if(check_syntax(GetNextToken(in,line),line,LPAREN,IF)){
			if(Expr(in,line)){
				if(check_syntax(GetNextToken(in,line),line,RPAREN,IF)){
					if(check_syntax(GetNextToken(in,line),line,LBRACES,IF)){
						if(StmtList(in,line)){
							if(check_syntax(GetNextToken(in,line),line,RBRACES,IF)){
								lex = GetNextToken(in,line);
								if(lex.GetToken() == ELSE){
									if(check_syntax(GetNextToken(in,line),line,LBRACES,ELSE)){
										if(StmtList(in,line)){
											if(check_syntax(GetNextToken(in,line),line,RBRACES,ELSE)){
												return true;
											}
										}
										else{
											line--;
											ParseError(line,"Missing Statement for Else-Clause");
										}
									}
								}
								else{
									PushBackToken(lex);
									return true;
								}
							}
						}
						else{
							ParseError(line,"Missing Statement for If Statement Clause");
						}
					}
				}
			}
		}
	}
	else
		PushBackToken(lex);
	ParseError(line,"Incorrect If-Statement");
	return false;
}
bool AssignStmt(istream& in, int& line){
	if(!Var(in,line)){
		return false;
	}
	LexItem v = GetNextToken(in,line);
	string lexeme = v.GetLexeme();
	if(!AssigOp(in,line)){
		ParseError(line,"Missing Assignment Operator");
		return false;
	}
	defVar[lexeme] = true;
	if(!Expr(in,line)){
		ParseError(line,"Missing operand for an operator");
		ParseError(line,"Missing Expression in Assignment Statement");
		return false;
	}
	return true;
}

bool Var(istream& in, int& line){
	LexItem lex = GetNextToken(in,line);
	Token tok = lex.GetToken();
	PushBackToken(lex);
	if (tok == IDENT){
		return true;
	}
	ParseError(line,"In Var: Not an ident");
	return false;
	


}
bool ExprList(istream& in, int& line){
	bool loop = false;
	LexItem lex;
	while(Expr(in,line)){
		loop = true;
		lex = GetNextToken(in,line);
		if(lex.GetToken() == COMMA)
			continue;
		PushBackToken(lex);
		break;
		
	}
	if(!loop)
		return false;	
	return true;
	
}
bool Expr(istream& in, int& line){
    return OrExpr(in,line);
}
bool AssigOp(istream& in, int& line){
	LexItem lex = GetNextToken(in,line);
	Token tok = lex.GetToken();
	if(tok == ASSOP || tok == CADDA || tok == CSUBA || tok == CCATA)
		return true;
	return false;
}
bool OrExpr(istream& in, int& line){
	bool loop = false;
	LexItem lex;
	while(  AndExpr(in,line)){
		loop = true;
		lex = GetNextToken(in,line);
		if(lex.GetToken() == OR)
			continue;
		PushBackToken(lex);
		break;
		
	}
	if(!loop)
		return false;	
	return true;
}
bool AndExpr(istream& in, int& line){
	bool loop = false;
	LexItem lex;
	while(RelExpr(in,line)){
		loop = true;
		lex = GetNextToken(in,line);
		if(lex.GetToken() == AND)
			continue;
		PushBackToken(lex);
		break;
		
	}
	if(!loop)
		return false;	
	return true;
}
bool RelExpr(istream& in, int& line){
	if(AddExpr(in,line)){
		LexItem lex = GetNextToken(in,line);
		Token tok = lex.GetToken();
		if(tok == SEQ || tok == SGT || tok == SEQ || tok == NLT || tok == NGTE || tok == NEQ){
			if(AddExpr(in,line))
				return true;
		}
		PushBackToken(lex);
		return true;
	}
	return false;
}
bool AddExpr(istream& in, int& line){
	bool loop = false;
	LexItem lex;
	while(MultExpr(in,line)){
		loop = true;
		lex = GetNextToken(in,line);
		Token tok = lex.GetToken();
		if(tok == PLUS || tok == MINUS || tok == CAT){
			if(MultExpr(in,line)){
				continue;
			}
			else{
				ParseError(line,"Missing operand after operator");
				return false;
			}
		}
		PushBackToken(lex);
		break;
		
	}

	if(!loop)
		return false;	
	return true;
}
bool MultExpr(istream& in, int& line){
	bool loop = false;
	LexItem lex;
	while( UnaryExpr(in,line)){
		loop = true;
		lex = GetNextToken(in,line);
		Token tok = lex.GetToken();
		if(tok == MULT || tok == DIV || tok == REM || tok == SREPEAT){
			if(UnaryExpr(in,line)){
				continue;
			}
			else {
				ParseError(line, "Missing operand for an operator");
				ParseError(line,"Missing operand after operator");
				return false;
			}
		}
		PushBackToken(lex);
		break;
	}
	if(!loop){
		return false;	
	}
	return true;
}
bool UnaryExpr(istream& in, int& line){
	int sign;
	LexItem lex = GetNextToken(in,line);
	Token tok = lex.GetToken();
	if(tok == MINUS || tok == PLUS || tok == NOT){
		if(tok == MINUS) sign = -1;
		else if (tok == PLUS) sign = 1;
		else sign = 0;
		return ExponExpr(in,line,sign);
	}
	PushBackToken(lex);
	if(tok == SEMICOL){
		return false;
	}
	if(ExponExpr(in,line,1)){
		return true;
	}
	
	return false;


}
//($r * ($y + $x **))
bool ExponExpr(istream& in, int& line, int sign){
	LexItem lex;
	Token tok;
	if(PrimaryExpr(in,line,sign)){
		lex = GetNextToken(in,line);
		tok = lex.GetToken();
		while(tok == EXPONENT){
			if(PrimaryExpr(in,line,sign)){
				lex = GetNextToken(in,line);
				tok = lex.GetToken();
				continue;
			}
			
			ParseError(line,"Missing exponent operand after exponentiation");
			ParseError(line, "Missing operand for an operator");
			
			return false;
		}
		PushBackToken(lex);
		return true;
	}
	return false;
	
}
bool PrimaryExpr(istream& in, int& line, int sign){
	LexItem lex = GetNextToken(in,line);
	Token tok = lex.GetToken();
	string lexeme = lex.GetLexeme();
	if( tok == ICONST || tok == FCONST || tok == SCONST){
		return true;
	}
	else if(tok == IDENT){
		if(defVar[lexeme]){
			return true;
		}
		ParseError(line,"Using Undefined Variable: " + lexeme);
		return false;
	}
	else if(tok == LPAREN){ 
		if(Expr(in,line)){
			lex = GetNextToken(in,line);
			tok = lex.GetToken();
			if(tok == RPAREN)
				return true;
			ParseError(line,"Missing right Parenthesis after expression");
            PushBackToken(lex); 
		}
		else{
			ParseError(line,"Missing expression after Left Parenthesis");
		
		}
		return false;
	}

	PushBackToken(lex);
	return false;
	
	
	
}
