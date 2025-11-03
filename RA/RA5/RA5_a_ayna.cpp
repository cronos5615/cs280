#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
using std::map;
void analyzeVowels(std::string word, int &vowel_count);
int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);
    if(!file){
        std::cout << "CANNOT OPEN THE FILE " << filename << std::endl;
        return 0;
    }
    bool vow1 = false, vow2 = false, vow3 = false;
    if(argc>2){
        for(int i = 2;i<argc;i++){
            std::string temp = argv[i];
            if(temp == "-all"){
                vow1 = true;
                vow2 = true;
                vow3 = true;
            }
            else if (temp == "-vow1"){
                vow1 = true;
                continue;
            }
            else if (temp == "-vow2"){
                vow2 = true;
                continue;
            }
            else if (temp == "-vow3"){
                vow3 = true;
                continue;
            }
            else{
                std::cout << "UNRECOGNIZED FLAG " << temp << std::endl;
                return 0;
            }
        }
    }
    map<std::string,int> vowmap1,vowmap2,vowmap3;
    int numWords = 0, numLines = 0, numVow1 = 0, numVow2 = 0, numVow3 = 0;
    std::string line;
    while(std::getline(file,line)){
        numLines++;
        if(line.length() == 0)
            continue;
        std::stringstream ss(line);
        std::string word;
        int vowel_count = 0;
        while(ss >> word){
            analyzeVowels(word,vowel_count);
                if( vowel_count == 1 ){
                    vowmap1[word]++;
                    numVow1++;
                }
                else if ( vowel_count == 2 ){
                    vowmap2[word]++;
                    numVow2++;
                }
                else if ( vowel_count == 3 ){
                    vowmap3[word]++;
                    numVow3++;
                }
                numWords++;
        }

    }
    if (numLines == 0){
        std::cout << "File is empty." << std::endl;
        return 0;
    }
    std::cout << "Total Number of Lines: " << numLines << std::endl;
    std::cout << "Number of Words: " << numWords << std::endl;
    std::cout << "Number of Words with One Vowel: " << numVow1 << std::endl;
    std::cout << "Number of Words with Two Vowels: " << numVow2 << std::endl;
    std::cout << "Number of Words with Three or More Vowels: " << numVow3 << std::endl;
    if(vow1 && vowmap1.size() > 0){
        std::cout << "\nList of Words with One Vowel and their number of occurrences:" << std::endl;
        map<std::string, int>::iterator it;
        for (it = vowmap1.begin(); it != vowmap1.end(); it++)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
    }
    if(vow2 && vowmap2.size() > 0){
        std::cout << "\nList of Words with Two Vowels and their number of occurrences:" << std::endl;
        map<std::string, int>::iterator it;
        for (it = vowmap2.begin(); it != vowmap2.end(); it++)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
    }
    if(vow3 && vowmap3.size() > 0){
        std::cout << "\nList of Words with Three or More Vowels and their number of occurrences:" << std::endl;
        map<std::string, int>::iterator it;
        for (it = vowmap3.begin(); it != vowmap3.end(); it++)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
    }


}

void analyzeVowels(std::string word,int &vowel_count){
    vowel_count = 0;
    for (char &c : word) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    std::string vowels = "aeiou";
    for( int i = 0 ; i < word.length() ; i++ ){
        if(vowels.find(word[i]) != std::string::npos){
            vowel_count++;
        }
    }
}