#include <iostream>
int main(){
    std::string first_name,last_name;
    int section;
    std::cout << "Welcome to CS 280 (Fall 2025)" << std::endl;
    std::cout << "What are your first name, last name, and section number?" << std::endl;
    std::cin >> first_name >> last_name >> section;
    std::cout << "Hello " << first_name << ", Welcome to CS 280 Section " << section << "." << std::endl;
    return 0;
}