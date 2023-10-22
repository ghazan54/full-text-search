#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct IndexAccessor IndexAccessor;
typedef struct Results Results;

typedef struct {
    char* str;
    size_t doc_id;
    double score;
} CResult;

IndexAccessor* fts_create_handle(const char* path);
void fts_delete_handle(IndexAccessor* accessor);
Results* fts_search(const char* query, const IndexAccessor* accessor);
void fts_delete_results(Results* res);
size_t fts_get_results_size(const Results* res);
CResult fts_get_row_info(const Results* res, const size_t idx,
                         const IndexAccessor* accessor);

void fts_print_result(const Results* res, const IndexAccessor* accessor);

#ifdef __cplusplus
}
#endif
