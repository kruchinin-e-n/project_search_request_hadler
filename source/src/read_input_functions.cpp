#include "read_input_functions.h" 
 
// functions - reading - implementation
 
std::string_view ReadLine() {
    std::string s;
    std::getline(std::cin, s);
    return s; 
} 
 
int ReadLineWithNumber() { 
    int result; 
    std::cin >> result;
    ReadLine(); 
    return result; 
}