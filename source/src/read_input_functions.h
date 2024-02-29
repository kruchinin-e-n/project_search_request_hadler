#pragma once 
#include "document.h" 
#include "paginator.h" 
 
#include <string> 
#include <iostream> 
 
// functions - reading - declaration
 
std::string_view ReadLine(); 
 
int ReadLineWithNumber(); 
 
template <typename Iterator> 
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator>& a);