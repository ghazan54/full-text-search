#pragma once

#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <filesystem>

#include <functional>
#include <string>
#include <vector>

namespace fts::searcher {

struct Result {
    size_t doc_id;
    double score;
};

using Results = std::vector<Result>;

Results search(const std::string& query,
               index_accessor::IndexAccessor& index_accessor);

}  // namespace fts::searcher
