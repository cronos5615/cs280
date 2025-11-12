#include <iostream>
#include <map>

std::map<int,std::string> convert = {
    {10,"A"},
    {11,"B"},
    {12,"C"},
    {13,"D"},
    {14,"E"},
    {15,"F"}
};
std::string DecToBaseN(int dec, int base){
    if(base < 2 || base > 16){
        std::cout << "Invalid Base: " << base << std::endl;
        return "";
    }
    if(dec==0)
        return "";
    int division = dec/base;
    int rem = dec % base;
    return DecToBaseN(division,base) + (dec % base > 9 ? convert[rem] : std::to_string(rem));
}