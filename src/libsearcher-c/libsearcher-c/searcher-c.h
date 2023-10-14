#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IndexAccessor IndexAccessor;
typedef struct Results Results;

// typedef struct {
//     size_t ngram_min_length_;
//     size_t ngram_max_length_;
//     char** stop_words_;
// } C_Config;

IndexAccessor* fts_create_handle(const char* path);
void fts_delete_handle(IndexAccessor* accessor);
Results* fts_search(const char* query, const IndexAccessor* accessor);
void fts_print_result(const Results* res, const IndexAccessor* accessor);

#ifdef __cplusplus
}
#endif
