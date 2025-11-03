#include "lex.h"
#include <fstream>
#include <set>
#include <vector>
#include <cmath>
int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "No specified input file." << endl;
        return 0;
    }
    
    string filename = argv[1];
    ifstream file(filename);
    bool v = false, num = false, str = false, idents = false;
    if(!file){
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        return 0;
    }
    if(argc>2){
        for(int i = 2;i<argc;i++){
            string temp = argv[i];
            if(temp.substr(0,1) != "-"){
                cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
                return 0;
            }
            else{
                if(temp == "-v"){
                    v = true;
                }
                else if(temp == "-num"){
                    num = true;
                }
                else if(temp == "-str"){
                    str = true;
                }
                else if(temp == "-ids"){
                    idents = true;
                }
                else{
                    cout << "UNRECOGNIZED FLAG {" << temp << "}" << endl;
                    return 0;
                 }
            }
        }
    }
    int linenum = 1, tokenCount = 0;
    //vector<Token> tok_vector;
    //vector<string> lexeme_vector;
    set<string> IDENT_Lexemes;
    set<double> NUM_Lexemes;
    set<string> STR_Lexemes;
    LexItem lex;
    Token tok;
    string lexeme;
    while (true){
        lex = getNextToken(file,linenum);
        tok = lex.GetToken();
        lexeme = lex.GetLexeme();
        if (tok == ERR){
            cout << "ERR: Error-Unrecognized Lexeme {" << lexeme << "} in line " << linenum << endl;
            return 0;
        }
        else if (tok == DONE){

            break;
        }
        if(v){
            cout << lex;
        }
        tokenCount++;
        if(tok == IDENT)
            IDENT_Lexemes.insert(lexeme);
        else if(tok == ICONST)
            NUM_Lexemes.insert(stod(lexeme));
        else if(tok == FCONST){
            bool has_e = lexeme.find('E') == string::npos ? lexeme.find('e') != string::npos : true;
            if(has_e){
                int index = lexeme.find('E') == string::npos ? lexeme.find('e') : lexeme.find('E');
                double result = stod(lexeme.substr(0,index)) * pow(10,stoi(lexeme.substr(index+1)));
                NUM_Lexemes.insert(result);
            }
            else{
                NUM_Lexemes.insert(stod(lexeme));
            }
        }
        else if(tok == SCONST)
            STR_Lexemes.insert(lexeme.substr(1,lexeme.length()-2));
    }
    linenum--;
    if(linenum == 0){
        cout << "Empty File." << endl;
        return 0;
    }
    int identCount = IDENT_Lexemes.size();
    int numLitCount = NUM_Lexemes.size();
    int strLitCount = STR_Lexemes.size();
    
    cout << endl << "Lines: " << linenum << endl;
    cout << "Total Tokens: " << tokenCount << endl;
    cout << "Identifiers: " << identCount << endl;
    cout << "Numeric Literals: " << numLitCount << endl;
    cout << "String Literals: " << strLitCount << endl;
    if(idents){
        cout << "IDENTIFIERS:" << endl;
        set<string>::iterator it = IDENT_Lexemes.begin();
        for(int i = 0; i < identCount; i++){
            cout << *it;
            ++it;
            if(i != identCount-1){
                cout << ", ";
            }
        }
        cout << endl;
    }
    if(num){
        cout << "NUMERIC LITERALS:" << endl;
        set<double>::iterator it = NUM_Lexemes.begin();
        for(int i = 0; i < numLitCount; i++){
            cout << *it << endl;
            ++it;
        }
    }
    if(str){
        cout << "STRING LITERALS:" << endl;
        set<string>::iterator it = STR_Lexemes.begin();
        for(int i = 0; i < strLitCount; i++){
            cout << "<" << *it << ">" << endl;
            ++it;
        }
    }

}