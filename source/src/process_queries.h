#pragma once

#include "document.h"
#include "search_server.h"

#include <vector>
#include <string>
#include <execution>
#include <algorithm>

// function ProcessQueries - declaration

std::vector<std::vector<Document>> ProcessQueries(
    const SearchServer& search_server, const std::vector<std::string>& queries);

// function ProcessQueriesJoined - declaration

std::vector<Document> ProcessQueriesJoined(
    const SearchServer& search_server, const std::vector<std::string>& queries);
