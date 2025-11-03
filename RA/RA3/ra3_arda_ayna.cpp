#include <iostream>
#include <fstream>
#include <sstream>
void throwError(char a, bool bracket);
bool isLineWhiteSpace(std::string line);
bool checkChainCommand(std::string line, int i);

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "NO SPECIFIED FILE NAME." << std::endl;
        return 0;
    }
    std::string special_char = "\\%&{}";
    char text;
    int numLines = 0, numComments = 0, numParag = 1, numIta = 0, numBol = 0, numCom = 0, numUl = 0, numNonParag = 0;
    bool bracketOpen = false, emptyLine = false, special_exists, nonParag = false, chainCommand = false;
    std::string filename = argv[1];
    std::ifstream file(filename);
    if(!file){
        std::cout << "CANNOT OPEN THE FILE: " << filename << std::endl;
        return 0;
    }
    std::string line;
    std::string command;
    char brace;
    while(std::getline(file,line)){
        numLines++;
        if(line.empty()) {
            emptyLine = true;
            if(nonParag){
                numNonParag++;
                nonParag = false;
            }
            continue;
        }
        if(isLineWhiteSpace(line)){  
            nonParag = true;
            continue;}
        else{
            nonParag = false;
        }

        for(int i = 0; i < line.length(); i++){
            text = line[i];
            switch(text){
                case '\\':
                    command = line.substr(i+1,2);
                    brace = line[i+3];
                    chainCommand = checkChainCommand(line,i);
                    if(command == "bf"){
                        numBol++;
                        numCom++;
                        if(brace == '{'){
                            i = i+3;
                        }
                        else{
                            if(!chainCommand) throwError('{',false);
                            i = i+2;
                        }
                        bracketOpen = true;
                        continue;
                    }
                    else if(command == "it"){
                        numIta++;
                        numCom++;
                        if(brace == '{'){

                            i = i+3;
                        }
                        else{
                            if(!chainCommand) throwError('{',false);
                            i = i+2;
                        }
                        bracketOpen = true;
                        continue;
                    }
                    else if(command == "ul"){
                        numUl++;
                        numCom++;
                        if(brace == '{'){
                            i = i+3;
                        }
                        else{
                            if(!chainCommand) throwError('{',false);
                            i = i+2;
                        }
                        bracketOpen = true;
                        continue;
                    }
                    special_exists = special_char.find(line[i+1]) != std::string::npos;
                    if(!special_exists){ 
                        throwError(text,true);
                        i++;
                        continue;
                    }
                    else if(special_exists){
                        i++;
                        continue;
                    }
                    else{
                        throwError(text,false);
                        continue;

                    }                    
                case '&':
                    throwError(text,true);
                    continue;
                case '%':
                    if(bracketOpen){
                        throwError(text,true);
                        continue;
                    }
                    numComments++;
                    line = line.substr(0,i+1);
                    continue;

                case '{':
                    if(bracketOpen){
                        throwError('}',false);
                        bracketOpen = false;
                        continue;
                    }
                    throwError(text,true);
                    continue;

                case '}':
                    if(bracketOpen){
                        bracketOpen = false;
                        continue;
                    }
                    throwError(text,true);
                    continue;
                
            }
        //inside the char for loop
        }
        //outside for, inside while
        if(emptyLine){
            emptyLine = false;
            numParag++;
        }


    }
    
    if (numLines == 0){
        std::cout << "The File is Empty." << std::endl;
        return 0;
    }
    std::cout << "\nNumber of Comments: " << numComments << std::endl;
    std::cout << "Number of Commands: " << numCom << std::endl;
    std::cout << "Number of Paragraphs: " << (numParag - numNonParag) << std::endl;
    std::cout << "Bold commands: " << numBol << std::endl;
    std::cout << "Italic commands: " << numIta << std::endl;
    std::cout << "Underline commands: " << numUl << std::endl;
}

bool isLineWhiteSpace(std::string line){
    for (char c : line){
        if(c != ' ') return false;
    }
    return true;
}
bool checkChainCommand(std::string line, int i){
    std::string temp = line.substr(i+4,3);
    if (temp == "\\bf" || temp == "\\it" || temp == "\\ul") return true;
    return false;



}
void throwError(char a, bool bracket){
    if (bracket){
        if(a == '\\'){
            std::cout<< "Error Message: Illegal use of '\\' special character in text." << std::endl;
        }
        else{
            std:: cout << "Error Message: Illegal use of a special character in text: " << a << std::endl;
    }}
    else{
        switch(a){
            case '\\':
                std::cout << "Error Message: Illegal use of '\\' special character in text." << std::endl;
                break;
            case '{':
                std::cout << "Error Message: Missing openning brace for a block." << std::endl;
                break;
            case '}':
                std::cout << "Error Message: Missing closing brace for a block." << std::endl;
                break;

        }
    }
}