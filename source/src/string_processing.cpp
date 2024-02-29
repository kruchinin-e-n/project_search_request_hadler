#include "string_processing.h" 
#include <cstdint>
 
// text processing - functions - implementation
 
std::vector<std::string> SplitIntoWords(std::string_view text) {
    std::vector<std::string> words;
    std::string word;
    for (const char c : text) { 
        if (c == ' ') { 
            if (!word.empty()) { 
                words.push_back(word); 
                word.clear(); 
            } 
        } 
        else { 
            word += c; 
        } 
    } 
    if (!word.empty()) { 
        words.push_back(word); 
    } 
 
    return words; 
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view text) {
    std::vector<std::string_view> result;
    text.remove_prefix(std::min(text.size(), text.find_first_not_of(" ")));

    const int64_t pos_end = text.npos;

    while (!text.empty()) {
        int64_t space = text.find(' ');
        result.push_back(space == pos_end ? text.substr(0, pos_end) : text.substr(0, space));
        text.remove_prefix(std::min(text.size(), text.find_first_not_of(" ", space)));
    }

    return result;
}