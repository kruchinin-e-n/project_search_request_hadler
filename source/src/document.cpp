#include "document.h" 
 
// struct Document - constructor

Document::Document(int id, double relevance, int rating)
  : id(id),
    relevance(relevance),
    rating(rating) {
}
 
// struct Document - operator overloading - "<<"

std::ostream& operator<<(std::ostream& output, const Document document) {
  output << "{ document_id = " << document.id
         << ", relevance = " << document.relevance
         << ", rating = " << document.rating
         << " }";

  return output;
}
