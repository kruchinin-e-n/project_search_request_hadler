#include "search_server.h"
#include "process_queries.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    SearchServer search_server("and with"s);
    int id = 0;
    for (
        const std::string& text : {
            "white cat and yellow hat"s,
            "curly cat curly tail"s,
            "nasty dog with big eyes"s,
            "nasty pigeon john"s,
        }
        ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, { 1, 2 });
    }
    std::cout << "ACTUAL by default:"s << std::endl;
    
    for (const Document& document : search_server.FindTopDocuments("curly nasty cat"s)) {
        std::cout 
            << "{ document_id = " << document.id 
            << ", relevance = " << document.relevance 
            << ", rating = " << document.rating 
            << " }" << std::endl;
    }
    std::cout << "BANNED:"s << std::endl;
    
    for (const Document& document : search_server.FindTopDocuments(std::execution::seq, "curly nasty cat"s, DocumentStatus::BANNED)) {
        std::cout << "{ " << "document_id = " << document.id << ", relevance = " << document.relevance << ", rating = " << document.rating << " }" << std::endl;
    }
    std::cout << "Even ids:"s << std::endl;
    
    for (const Document& document : search_server.FindTopDocuments(std::execution::par, "curly nasty cat"s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; })) {
        std::cout << "{ " << "document_id = " << document.id << ", relevance = " << document.relevance << ", rating = " << document.rating << " }" << std::endl;
    }
    return 0;
}