#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>
#include <libsearcher/searcher.hpp>

#include <map>
#include <set>

#include <algorithm>
#include <cmath>
#include <iostream>

namespace fts::searcher {

namespace {

size_t term_frequency(const std::string& term, size_t document_id,
                      index::ReverseIndex& info) {
    return info.at(term).count(document_id);
}

size_t document_frequency(const std::string& term, index::ReverseIndex& info) {
    std::set<size_t> docs;

    // std::copy(info.at(term).begin(), info.at(term).end(),
    //           std::inserter(docs, docs.end()));

    for (const auto& pair : info.at(term)) {
        docs.insert(pair.first);
    }

    return docs.size();
}

std::set<size_t> get_num_docs(index_accessor::IndexAccessor& index_accessor,
                              parser::NgramVec& ngrams) {
    std::set<size_t> docs;

    for (const auto& ngram : ngrams) {
        for (const auto& term : ngram) {
            auto info = index_accessor.get_term_infos(term);
            // std::copy(info.at(term).begin(), info.at(term).end(),
            //           std::inserter(docs, docs.end()));
            for (const auto& pair : info.at(term)) {
                docs.insert(pair.first);
            }
        }
    }

    return docs;
}

double score(parser::NgramVec& ngrams, size_t doc_id,
             index_accessor::IndexAccessor& index_accessor) {
    double rel = 0.;
    for (const auto& ngram : ngrams) {
        for (const auto& term : ngram) {
            auto total_docs = index_accessor.total_docs();
            auto info = index_accessor.get_term_infos(term);
            auto t_freq = term_frequency(term, doc_id, info);
            auto idf = log(static_cast<double>(total_docs) /
                           static_cast<double>(document_frequency(term, info)));
            rel += static_cast<double>(t_freq) * idf;
        }
    }
    return rel;
}

}  // namespace

Results search(const std::string& query,
               index_accessor::IndexAccessor& index_accessor) {
    Results results([](double key1, double key2) { return key1 > key2; });

    auto ngrams = parser::parse_ngram(query, index_accessor.config());
    auto docs_id = get_num_docs(index_accessor, ngrams);

    for (const auto doc_id : docs_id) {
        double rel = score(ngrams, doc_id, index_accessor);
        results.insert(std::make_pair(rel, doc_id));
    }

    return results;
}

}  // namespace fts::searcher
