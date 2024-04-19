#include "process_queries.h" 

// function ProcessQueries - implemetation

std::vector<std::vector<Document>> ProcessQueries(const SearchServer& search_server, const std::vector<std::string>& queries) {
	std::vector<std::vector<Document>> result(queries.size());
	std::transform(
		std::execution::par,
		queries.begin(),
		queries.end(),
		result.begin(),
		[&search_server](std::string word) {
			return search_server.FindTopDocuments(word);
		});
	return result;
}

// function ProcessQueriesJoined - implemetation

std::vector<Document> ProcessQueriesJoined(const SearchServer& search_server, const std::vector<std::string>& queries) {
	std::vector<Document> documents;

	for (const auto& document : ProcessQueries(search_server, queries)) {
		documents.insert(documents.end(), document.begin(), document.end());
	}

	return documents;
}