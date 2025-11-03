#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string special_char = "_&.";
std::string nums = "0123456789";
int determineType(std::string word);
bool isNumConst(std::string word);
int main(){
	std::string filename,text;
	std::cout << "Enter the file name to read from:" << std::endl;
	std::cin >> filename;
	std::ifstream file(filename);
	if(!(file)){
		std::cout << "\nCANNOT OPEN THE FILE: " << filename << std::endl; 
		return 0;
	}
	int numLines = 0,numFilledLines = 0,numWord = 0,numName = 0,numConst = 0;
	while(std::getline(file,text)){
		numLines ++;
		if(text == "") continue;
		numFilledLines++;
		std::istringstream line(text);
		std::string word;
		std::vector<std::string> words;
		while (line >> word) words.push_back(word);
		for(std::string v_word : words)
		{
			if(v_word == "")
				continue;
			numWord++;
			int type = determineType(v_word);
			if(type == 0) numName++;
			else if(type == 1)  numConst++;	
		}

	}		
	if(numLines == 0){
		std::cout << "File is empty." << std::endl;
		return 0;}
	std::cout << "Total Number of Lines: " << numLines << "\nNumber of non-blank lines: " << numFilledLines << "\nNumber of Words: " << numWord << "\nNumber of Numeric Constants: " << numConst << "\nNumber of Names: " << numName << std::endl;
	
}
bool isName(std::string word){
	if (letters.find(word[0]) != std::string::npos)
	{	for(int i = 1; i<word.length();i++){
			if (letters.find(word[i]) != std::string::npos) continue;
			if (nums.find(word[i]) != std::string::npos) continue;
			if (special_char.find(word[i]) != std::string::npos){
				if (special_char.find(word[i-1]) != std::string::npos){
					return false;}
			}
			else{
				return false;
			}
		}
		return true;	     
		
	}
	return false;	

}
bool isNumConst(std::string word){
	int dotCount = 0;
	if(word[0] == '.' || word[word.length()-1] == '.') return false;
	for(char c: word){
		if (nums.find(c) != std::string::npos)
			continue;
		else if(c == '.' && dotCount < 1)
			dotCount++;
		else
			return false;}
	return true;
}

/*
 * Determines if text is a word, name, or a numerical constant
 * 0 : Name 
 * 1 : Numerical constant
 * 2 : placeholder, not necessary
 */


int determineType(std::string word){
	if(isName(word)) return 0;
	if(isNumConst(word)) return 1;
	return 2;
}
