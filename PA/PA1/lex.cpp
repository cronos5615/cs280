#include "lex.h"


map<Token,string> tokens = {
    {ICONST,"ICONST"},
    {FCONST,"FCONST"},
    {IDENT,"IDENT"},
    {SCONST,"SCONST"},
    {PRINTLN, "PRINTLN"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {IDENT, "IDENT"},
    {ICONST, "ICONST"},
    {FCONST, "FCONST"},
    {SCONST, "SCONST"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {REM, "REM"},
    {EXPONENT, "EXPONENT"},
    {NEQ, "NEQ"},
    {SEQ, "SEQ"},
    {NLT, "NLT"},
    {NGTE, "NGTE"},
    {SLTE, "SLTE"},
    {SGT, "SGT"},
    {CAT, "CAT"},
    {SREPEAT, "SREPEAT"},
    {AND, "AND"},
    {OR, "OR"},
    {NOT, "NOT"},
    {ASSOP, "ASSOP"},
    {CADDA, "CADDA"},
    {CSUBA, "CSUBA"},
    {CCATA, "CCATA"},
    {COMMA, "COMMA"},
    {SEMICOL, "SEMICOL"},
    {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},
    {LBRACES, "LBRACES"},
    {RBRACES, "RBRACES"},
    {ERR, "ERR"},
    {DONE, "DONE"}
};


map<string,Token> ids = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", MULT},
    {"/", DIV},
    {"%", REM},
    {"**", EXPONENT},
    {".", CAT},
    {".x.", SREPEAT},
    {"==", NEQ},
    {"@eq", SEQ},
    {"<", NLT},
    {">=", NGTE},
    {"@le", SLTE},
    {"@gt", SGT},
    {"&&", AND},
    {"||", OR},
    {"!", NOT},
    {"=", ASSOP},
    {"+=", CADDA},
    {"-=", CSUBA},
    {".=", CCATA},
    {"println", PRINTLN},
    {"else", ELSE},
    {"if", IF},
    {";", SEMICOL},
    {",", COMMA},
    {"(", LPAREN},
    {")", RPAREN},
    {"{", LBRACES},
    {"}", RBRACES}
};
  
ostream& operator<<(ostream& out, const LexItem& tok){
    Token temp = tok.GetToken();
    out << tokens[temp] << ": ";
    if (temp == ICONST || temp == FCONST)
        out << "[" << tok.GetLexeme() << "]" << endl;
    else if (temp == IDENT)
        out << "("<< tok.GetLexeme() << ")" << endl;
    else if (temp == SCONST){
        string temp = tok.GetLexeme();
        temp = temp.substr(1,temp.length()-2);

        out << "<" << temp << ">" << endl;
    }
    else if (temp == ERR)
        out << "Error-Unrecognized Lexeme {" << tok.GetLexeme() << "} in line " << tok.GetLinenum() << endl;
    else{
        out << "\"" << tok.GetLexeme() << "\"" << endl;
    }
    return out;
}

LexItem id_or_kw(const string& lexeme, int linenum){
    string temp = lexeme;
    for (char &car : temp) {
        car = tolower(static_cast<unsigned char>(car));
        }
    if(ids.find(temp)!=ids.end()){
        return LexItem(ids[temp],lexeme,linenum);
    }
    else{
        return LexItem(IDENT,lexeme,linenum); 
    }

}
LexItem getNextToken(istream& in, int& linenum,string& lex){
    return LexItem(ERR,lex,linenum);
}
LexItem getNextToken(istream& in, int& linenum){
    string lexeme = "";
    char c = in.peek();
    //IDENT and KEYWORD
    if(isalpha(c) || c == '$'){
        lexeme += char(in.get());
        c = in.peek();
        while(isalnum(c) || c == '_' || c == '$'){
            lexeme += char(in.get());
            c = in.peek();
        } 
        return id_or_kw(lexeme,linenum);
    }
    //FCONST
    else if(isdigit(c)){
        lexeme += char(in.get());
        c = in.peek();
        while(isdigit(c)){
            lexeme += char(in.get());
            c = in.peek();
        }
        if(c == '.'){
            lexeme += char(in.get());
            c = in.peek();
            if(isdigit(c)){
                while(isdigit(c)){
                    lexeme += char(in.get());
                    c = in.peek();
                }
                if(c == '.'){
                    lexeme += char(in.get());
                    return LexItem(ERR,lexeme,linenum);
                }
                else if(c == 'e' || c == 'E'){
                    lexeme += char(in.get());
                    c = in.peek();
                    if(c == '+' || c == '-'){
                        lexeme += char(in.get());
                        c = in.peek();
                    }
                    if(isdigit(c)){
                        while(isdigit(c)){
                            lexeme += char(in.get());
                            c = in.peek();
                        }
                    }
                }
            }
            else{
                in.unget();
                lexeme = lexeme.substr(0,lexeme.length()-1);
            }
            return LexItem(FCONST,lexeme,linenum);
        }
        else{
            return LexItem(ICONST,lexeme,linenum);
        }

    }
    //SCONST
    else if(c == '\'' || c == '\"'){
        lexeme += char(in.get());
        char quote = c;
        bool quote_open = true;
        while(quote_open){
            c = in.peek();
            if(c == '\"' || c == '\''){
                if(quote == c){
                        quote_open = false;
                    }
            }
            else if(c == '\n' || c == EOF){
                return LexItem(ERR,lexeme,linenum);
            }
            lexeme += char(in.get());
        }
        return LexItem(SCONST,lexeme,linenum);
    }
    else if (c == '\n'){
        in.get();
        linenum++;
        return getNextToken(in,linenum);
    }
    else if (c == EOF){
        return LexItem(DONE,lexeme,linenum);
    }   
    else if(c == ' ' || c == '\t'){
        in.get();
        c = in.peek();
        while(c== ' ' || c == '\t'){
            in.get();
            c = in.peek();
        }
        return getNextToken(in,linenum);
    }
    else if( c == '#'){
        while(true){
            in.get();
            c = in.peek();
            if(c == '\n' || c == EOF){
                break;
            }
        }
        return getNextToken(in,linenum);
    }
    else if( c == '+'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '='){
            lexeme += char(in.get());
        }
        return LexItem(ids[lexeme],lexeme,linenum);

    }
    else if( c == '-'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '='){
            lexeme += char(in.get());
        }
        return LexItem(ids[lexeme],lexeme,linenum);

    }
    else if( c == '*'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '*'){
            lexeme += char(in.get());
        }
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if (c == '/'){
        lexeme += char(in.get());
        return LexItem(DIV,lexeme,linenum);
    }
    else if (c == '%'){
        lexeme += char(in.get());
        return LexItem(REM,lexeme,linenum);
    }
    else if (c == '.'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '='){
            lexeme += char(in.get());
        }
        else if (c == 'x'){
            lexeme += char(in.get());
            c = in.peek();
            if(c == '.'){
                lexeme += char(in.get());
            }
            else{
                lexeme = lexeme.substr(0,1);
                in.unget();
            }
        }
        if(ids.find(lexeme)!=ids.end())
            return LexItem(ids[lexeme],lexeme,linenum);
        return LexItem(ERR,lexeme,linenum);
    }
    else if(c == '>'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '='){
            lexeme += char(in.get());
            return LexItem(ids[lexeme],lexeme,linenum);
        }
        return LexItem(ERR,lexeme,linenum);
    }
    else if(c == '='){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '='){
            lexeme += char(in.get());
        }
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == '@'){
        lexeme += char(in.get());
        c = in.peek();
        if (c == 'l' || c == 'L'){
            lexeme += char(in.get());
                c = in.peek();
                if(c == 'e' || c == 'E')   
                    lexeme += char(in.get());
        }
        else if( c == 'e' || c == 'E'){
            lexeme += char(in.get());
                c = in.peek();
                if(c == 'q' || c == 'Q')   
                    lexeme += char(in.get());
        }
        else if( c == 'g' || c == 'G'){
            lexeme += char(in.get());
                c = in.peek();
                if(c == 't' || c == 'T')   
                    lexeme += char(in.get());
        }
        string temp = lexeme;
        for (char &car : temp) {
        car = tolower(static_cast<unsigned char>(car));
        }
        if(ids.find(temp)!=ids.end())
            return LexItem(ids[temp],lexeme,linenum);
        return LexItem(ERR,lexeme,linenum);

    }
    else if(c == '<'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == '&'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '&'){
            lexeme += char(in.get());
            return LexItem(ids[lexeme],lexeme,linenum);
        }
        return LexItem(ERR,lexeme,linenum);
    }
    else if(c == '|'){
        lexeme += char(in.get());
        c = in.peek();
        if(c == '|'){
            lexeme += char(in.get());
            return LexItem(ids[lexeme],lexeme,linenum);
        }
        return LexItem(ERR,lexeme,linenum);
    }
    else if(c == '!'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == ';'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == ','){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == '('){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == ')'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == '{'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }
    else if(c == '}'){
        lexeme += char(in.get());
        return LexItem(ids[lexeme],lexeme,linenum);
    }

    else{
        lexeme += char(in.get());
        return LexItem(ERR,lexeme,linenum);
    }
}


