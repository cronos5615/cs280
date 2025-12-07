/* Implementation of Recursive-Descent Parser
 * for the Basic Perl-Like Language (BPL)
 * parse.cpp
 * Programming Assignment 2
 * Fall 2025
*/

#include "parserInt.h"



map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

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

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}

bool consumeFalseIf(istream* in){

}
//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;
	//cout << tok << " " << tok.GetLinenum() <<endl;
	
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
			
	if(!f1) 
	{
		return false;
	}
	else 
	{
		
		cout << endl <<endl;
		cout << "DONE" << endl;	
		return true;
	}	
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
		
	LexItem tok;
	
	status = Stmt(in, line);
	while(status )
	{
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else if(tok == RBRACES)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		
		if(tok != SEMICOL)
		{
			line--;
			ParseError(line, "Missing semicolon at end of Statement");
			return false;
		}
		
		status = Stmt(in, line);
		
	}
			
	tok = Parser::GetNextToken(in, line);
	
	if(tok == ELSE )
	{
		ParseError(line, "Illegal If Statement Else-Clause");
		return false;
	}
	else if(tok == RBRACES)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	else if(tok == DONE && !status)
	{
		
		ParseError(line, "Syntactic error in Program Body");
		return false;
	}
	else
	{
		ParseError(line, "Syntactic error in Program Body");
		return false;
	}
	
}//End of StmtList function

//Stmt ::= AssignStme | PrintLnStmt | IfStmt  
bool Stmt(istream& in, int& line){
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {
	case IDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return status;
		}
		break;
	case PRINTLN:
		
		status = PrintLnStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect PrintLn Statement");
			return status;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect If-Statement");
			return status;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	
	case DONE:
		return true;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function


//PrintLnStmt:= PRINTLN (ExpreList) 
bool PrintLnStmt(istream& in, int& line) {
	LexItem t;
		
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of PrintLn Statement");
		return false;
	}
	Value temp;
	bool ex = ExprList(in, line);
	if( !ex ) {
		ParseError(line, "Missing expression list after PrintLn");
		return false;
	}
	for(int i = 0; i < ValQue->size(); i++){
		std::cout << ValQue->front();
		ValQue->pop();
	}
	std::cout << endl;
	
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of PrintLn Statement");
		return false;
	}
	
	return true;
}//End of PrintlnStmt

//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	Value val;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	
	ex = Expr(in,line, val);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != LBRACES)
	{
		Parser::PushBackToken(t);
		ParseError(line, "Missing left brace for If Statement Clause");
		return false;
	}
	if((val || false).GetBool()){
		status = StmtList(in, line);
	}
	else{
		istream copy = in;
	}
	if(!status )
		{
		ParseError(line, "Missing Statement for If Statement Clause");
		return false;
		}
		t = Parser::GetNextToken(in, line);
	if(t != RBRACES)
	{
		ParseError(line, "Missing right brace for If Statement Clause");
		return false;
	}	
	t = Parser::GetNextToken(in, line);
	
	if( t == ELSE ) {
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES)
		{
			Parser::PushBackToken(t);
			ParseError(line, "Missing left brace for Else-Clause");
			return false;
		}
		status = StmtList(in, line);
		if( !status)
		{
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if( t != RBRACES)
		{
			line--;
			Parser::PushBackToken(t);
			ParseError(line, "Missing right brace for an Else-Clause");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(t);
		return true;
	}
	return true;
}//End of IfStmt function

//Var ::= IDENT
bool Var(istream& in, int& line, string& var)
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT ){
		identstr = tok.GetLexeme();
		var = identstr;
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			
		}	
		
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	
	return false;
}//End of Var

//AssignStmt:= Var AssignOp Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t;
	Value val;
	string var;
	varstatus = Var( in, line, var);
		
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP || t == CADDA || t == CSUBA || t == CCATA){
			status = Expr(in, line, val);
			
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			//define the variable
			TempsResults[var] = val;
			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value val;
	status = Expr(in,line, val);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(val);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList
bool OrExpr(istream& in, int& line, Value& val){
	return true;
}
//Expr ::= OrExpr ::= AndExpr { || AndExpr }
bool Expr(istream& in, int& line, Value& val) {
	Value v1;
	LexItem tok;
	bool t1 = AndExpr(in,line, val);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR  ) 
	{
		t1 = AndExpr(in,line, v1);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after OR operator");
			return false;
		}
		val = val || v1;
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of OrExpr or Expr

//	AndExpr ::= RelExpr  [ && RelExpr ]
bool AndExpr(istream& in, int& line, Value& val) {
	LexItem tok;
	Value v1;
	bool t1 = RelExpr(in,line, val);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND  ) 
	{
		t1 = RelExpr(in,line, v1);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after AND operator");
			return false;
		}
		val = val && v1;
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AndExpr

//RelExpr ::= AddExpr  [ ( @le | @gt | @eq | < | >= | == ) AddExpr ]
bool RelExpr(istream& in, int& line, Value& val) {
	LexItem tok;
	Value v1;
	bool t1 = AddExpr(in,line, val);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NGTE || tok == NLT || tok == NEQ || tok == SLTE || tok == SGT || tok == SEQ ) 
	{
		t1 = AddExpr(in,line, v1);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after a relational operator");
			return false;
		}
		if( tok == NGTE){
			val = val >= v1;
		}
		else if( tok == NLT ){
			val =  val < v1;
		}
		else if( tok == NEQ ){
			val = val == v1;
		}
		else if( tok == SLTE ){
			val = val.SLE(v1);
		}
		else if( tok == SGT ){
			val = val.SGT(v1);
		}
		else{
			val = val.SEQ(v1);
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value& val) {
	Value v1,v2;
	bool t1 = MultExpr(in,line, v1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	val = v1;
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		t1 = MultExpr(in,line, v2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == PLUS){
			val = val + v2;
		}
		else if(tok == MINUS){
			val = val - v2;
		}
		else if(tok == CAT){
			val = val.Catenate(v2);
		}
		if(val.IsErr()){
			ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
            return false;
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}

bool MultExpr(istream& in, int& line, Value& val) {
	Value v1;
	bool t1 = UnaryExpr(in,line, val);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  || tok == SREPEAT)
	{
		t1 = UnaryExpr(in,line, v1);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == MULT)
			val = val * v1;
		else if(tok == DIV){
			if(v1.GetBool() == 0){
				std::cout << "Run-Time Error: Illegal Division by Zero" << endl;
				ParseError(line,"Illegal operand type or value for the division operation.");
			}
			val = val/v1;
		}
		else
			val = val.Repeat(v1);
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//UnaryExpr ::= [( - | + | ! )] ExponExpr 
bool UnaryExpr(istream& in, int& line, Value& val)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else if(t == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(t);
		
	status = ExponExpr(in, line, sign, val);
	if(!status)
	{
		ParseError(line, "Missing operand for an operator");
		return false;
	}
	return status;
	switch(sign){
		case -1:
			val = val * Value(-1.0);

		case 1:
			//to detect +(string)
			val = val * Value(1.0);
		case 2:
			val = !val;
	}
}//End of UnaryExpr

//ExponExpr ::= PrimaryExpr { ** PrimaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, int sign, Value& val)
{
	bool status;
	Value v1;
		
	status = PrimaryExpr(in, line, sign, val);
	if( !status ) {
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT)
	{
		status = ExponExpr(in, line, sign, v1);
		if( !status ) {
			ParseError(line, "Missing exponent operand after exponentiation");
			return false;
		}
		val = val.Expon(v1);
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			std::cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of ExponExpr


//PrimaryExpr ::= IDENT | ICONST | FCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign, Value& val) {
	LexItem tok = Parser::GetNextToken(in, line);
	
	if( tok == IDENT ) {
		 
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable: "+ lexeme);
			Parser::PushBackToken(tok);
			
			return false;	
		}
		val = Value(TempsResults[lexeme]);
		return true;
	}
	else if( tok == ICONST ) {
		val = tok.GetLexeme();
		return true;
	}
	else if( tok == SCONST ) {
		val = tok.GetLexeme();
		return true;
	}
	else if( tok == FCONST ) {
		val = tok.GetLexeme();
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in,line, val);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		std::cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	return false;
}



