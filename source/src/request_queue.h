#pragma once 

#include "search_server.h" 
#include "document.h" 

#include <vector> 
#include <string> 
#include <deque> 
 
// request queue - functions - declaration
 
class RequestQueue { 
 
public: 
 
    explicit RequestQueue(const SearchServer& search_server); 
 
    template <typename DocumentPredicate> 
    std::vector<Document> AddFindRequest(std::string_view raw_query, DocumentPredicate document_predicate);
 
    std::vector<Document> AddFindRequest(std::string_view raw_query, DocumentStatus status);
 
    std::vector<Document> AddFindRequest(std::string_view raw_query);
 
    void AddRequest(std::vector<Document> result); 
 
    int GetNoResultRequests() const; 
 
private: 
 
    struct QueryResult { 
        bool was_found_ = 0; 
        std::vector<Document> query_result_; 
 
    }; 
 
    std::deque<QueryResult> requests_; 
 
    const static int min_in_day_ = 1440; 
 
    const SearchServer& search_server_; 
 
    int timer = 0; 
 
}; 
 
template <typename DocumentPredicate> 
std::vector<Document> RequestQueue::AddFindRequest(std::string_view raw_query, DocumentPredicate document_predicate) {
    std::vector<Document> result = search_server_.FindTopDocuments(raw_query, document_predicate); 
    AddRequest(result); 
    return result; 
}