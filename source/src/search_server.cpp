#include "search_server.h" 

// class SearchServer - functions - implementation

void SearchServer::AddDocument(int document_id,
	std::string_view document,
	DocumentStatus status,
	const std::vector<int>& ratings) {

	if ((document_id < 0) || (documents_.count(document_id) > 0)) {
		throw std::invalid_argument("Invalid document_id"s);
	}

	storage_.push_back(std::string{ document });

	const auto words = SplitIntoWordsNoStop(storage_.back());
	const double inv_word_count = 1.0 / words.size();

	for (const std::string_view word : words) {
		word_to_document_freqs_[word][document_id] += inv_word_count;
		document_to_word_freqs_[document_id][word] += inv_word_count;
	}

	ids_.emplace(document_id);
	documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentStatus status) const {
	return FindTopDocuments(std::execution::seq, raw_query, status);
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query) const {
	return FindTopDocuments(std::execution::seq, raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
	return static_cast <int> (documents_.size());
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const {
	const std::map<std::string_view, double>& result = document_to_word_freqs_.at(document_id);
	return result;
}

void SearchServer::RemoveDocument(int document_id) {
	return RemoveDocument(std::execution::seq, document_id);
}

void SearchServer::RemoveDocument(const std::execution::sequenced_policy&, int document_id) {

	documents_.erase(document_id);
	ids_.erase(document_id);
	document_to_word_freqs_.erase(document_id);

	for (auto& element : word_to_document_freqs_) {
		element.second.erase(document_id);
	}
}

void SearchServer::RemoveDocument(const std::execution::parallel_policy&, int document_id) {

	std::map<std::string_view, double>& word_freqs = document_to_word_freqs_.at(document_id);
	std::vector<std::string_view> to_erase(word_freqs.size());

	std::transform(
		std::execution::par,
		word_freqs.begin(),
		word_freqs.end(),
		to_erase.begin(),
		[] (const std::pair <std::string_view, double>& word_freq) { return word_freq.first; }
	);

	std::for_each(
		std::execution::par,
		to_erase.begin(),
		to_erase.end(),
		[this, document_id](std::string_view word) {
			word_to_document_freqs_.at(word).erase(document_id);
		}
	);

	documents_.erase(document_id);
	ids_.erase(document_id);
	document_to_word_freqs_.erase(document_id);
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(std::string_view raw_query, int document_id) const {
	return MatchDocument(std::execution::seq, raw_query, document_id);
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::sequenced_policy&, std::string_view raw_query, int document_id) const {
	const Query query = ParseQuery(raw_query);
	std::vector<std::string_view> matched_words;

	if (any_of(query.minus_words.begin(), query.minus_words.end(),
		[this, document_id](const std::string_view word) {
			return (word_to_document_freqs_.count(word) > 0 && word_to_document_freqs_.at(word).count(document_id) > 0);
		})) {
		return { matched_words, documents_.at(document_id).status };
	}

	for (const std::string_view& word : query.plus_words) {
		if (word_to_document_freqs_.count(word) == 0) {
			continue;
		}
		if (word_to_document_freqs_.at(word).count(document_id)) {
			matched_words.push_back(word);
		}
	}

	return { matched_words, documents_.at(document_id).status };
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::parallel_policy&, std::string_view raw_query, int document_id) const {

	if ((document_id < 0) || (documents_.count(document_id) == 0)) {
		throw std::invalid_argument("document id is out of range"s);
	}

	const Query query = ParseQueryParallel(raw_query);
	const auto& word_freqs = document_to_word_freqs_.at(document_id);
	std::vector<std::string_view> matched_words;

	if (any_of(query.minus_words.begin(),
		query.minus_words.end(),
		[&word_freqs](std::string_view word) {
			return (word_freqs.count(word) > 0);
		})) {
		return { matched_words, documents_.at(document_id).status };
	}

	matched_words.resize(query.plus_words.size());

	const auto& it = copy_if(query.plus_words.begin(),
		query.plus_words.end(),
		matched_words.begin(),
		[&word_freqs](std::string_view word) {
			return (word_freqs.count(word) > 0);
		});

	matched_words.erase(it, matched_words.end());

	std::sort(std::execution::par, matched_words.begin(), matched_words.end());
	const auto& itr = unique(matched_words.begin(), matched_words.end());
	matched_words.erase(itr, matched_words.end());

	return { matched_words, documents_.at(document_id).status };
}

bool SearchServer::IsStopWord(std::string_view word) const {
	return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(std::string_view word) {
	return std::none_of(word.begin(), word.end(),
		[](char c) {
			return c >= '\0' && c < ' ';
		});
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(std::string_view text) const {
	std::vector<std::string_view> words;
	for (std::string_view word : SplitIntoWordsView(text)) {
		if (!IsValidWord(word)) {
			throw std::invalid_argument("Word is invalid"s);
		}
		if (!IsStopWord(word)) {
			words.push_back(word);
		}
	}
	return words;
}


int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
	if (ratings.empty()) {
		return 0;
	}
	int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
	return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view text) const {
	if (text.empty()) {
		throw std::invalid_argument("Query word is empty"s);
	}
	std::string_view word = text;
	bool is_minus = false;
	if (word[0] == '-') {
		is_minus = true;
		word = word.substr(1);
	}
	if (word.empty() || word[0] == '-' || !IsValidWord(word)) {
		throw std::invalid_argument("Query word is invalid");
	}
	return { word, is_minus, IsStopWord(word) };
}

SearchServer::Query SearchServer::ParseQuery(std::string_view text) const {
	Query query;
	for (std::string_view word : SplitIntoWordsView(text)) {
		const auto query_word = ParseQueryWord(word);
		if (!query_word.is_stop) {
			if (query_word.is_minus) {
				query.minus_words.push_back(query_word.data);
			}
			else {
				query.plus_words.push_back(query_word.data);
			}
		}
	}

	std::sort(query.minus_words.begin(), query.minus_words.end());
	auto it = std::unique(query.minus_words.begin(), query.minus_words.end());
	query.minus_words.erase(it, query.minus_words.end());

	std::sort(query.plus_words.begin(), query.plus_words.end());
	it = std::unique(query.plus_words.begin(), query.plus_words.end());
	query.plus_words.erase(it, query.plus_words.end());

	return query;
}

SearchServer::Query SearchServer::ParseQueryParallel(std::string_view text) const {
	Query result;
	for (std::string_view word : SplitIntoWordsView(text)) {
		const QueryWord query_word = ParseQueryWord(word);
		if (!query_word.is_stop) {
			if (query_word.is_minus) {
				result.minus_words.push_back(query_word.data);
			}
			else {
				result.plus_words.push_back(query_word.data);
			}
		}
	}
	return result;
}

double SearchServer::ComputeWordInverseDocumentFreq(std::string_view word) const {
	return std::log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}