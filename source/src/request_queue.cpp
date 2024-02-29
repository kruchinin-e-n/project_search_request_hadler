#include "request_queue.h" 
 
// request queue - functions - implementation
 
RequestQueue::RequestQueue(const SearchServer& search_server) 
    : search_server_(search_server) 
{ 
} 
 
std::vector<Document> RequestQueue::AddFindRequest(std::string_view raw_query, DocumentStatus status) {
    std::vector<Document> result = RequestQueue::search_server_.FindTopDocuments(raw_query, status);
    AddRequest(result); 
    return result; 
} 
 
std::vector<Document> RequestQueue::AddFindRequest(std::string_view raw_query) {
    std::vector<Document> result = RequestQueue::search_server_.FindTopDocuments(raw_query);
    AddRequest(result); 
    return result; 
} 
 

void RequestQueue::AddRequest(std::vector<Document> result) {
    if (requests_.size() == min_in_day_) { 
        requests_.pop_front(); 
        timer = 1; 
    } 
    QueryResult query_res; 
    query_res.query_result_ = result; 
    if (result.empty()) { 
        query_res.was_found_ = 0; 
        requests_.push_back(query_res); 
    } 
    if (!result.empty()) { 
        query_res.was_found_ = 1; 
        requests_.push_back(query_res); 
    } 
} 
 
int RequestQueue::GetNoResultRequests() const { 
    int result = 0; 
    for (const auto& query : requests_) { 
        if (query.was_found_ == 0) { 
            ++result; 
        } 
    } 
    return result; 
}