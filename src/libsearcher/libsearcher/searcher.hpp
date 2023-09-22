#pragma once

#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <filesystem>

#include <functional>
#include <map>
#include <string>

namespace fts::searcher {

using Results =
    std::multimap<double, size_t, std::function<bool(double, double)>>;

Results search(const std::string& query,
               index_accessor::IndexAccessor& index_accessor);

}  // namespace fts::searcher
