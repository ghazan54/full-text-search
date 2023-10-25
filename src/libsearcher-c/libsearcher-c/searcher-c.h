/**
 * @file index_accessor.h
 * @brief C API for accessing the index and performing searches.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * @brief Opaque struct representing an index accessor handle.
 */
typedef struct IndexAccessor IndexAccessor;

/**
 * @brief Opaque struct representing search results.
 */
typedef struct Results Results;

/**
 * @brief Struct representing a result row in the search results.
 */
typedef struct {
    char* str;     /**< The string result. */
    size_t doc_id; /**< The ID of the document. */
    double score;  /**< The score of the result. */
} CResult;

/**
 * @brief Creates an index accessor handle.
 * @param path The path to the index.
 * @return A pointer to the IndexAccessor handle, or NULL on failure.
 */
IndexAccessor* fts_create_handle(const char* path);

/**
 * @brief Deletes an index accessor handle.
 * @param accessor The IndexAccessor handle to be deleted.
 */
void fts_delete_handle(IndexAccessor* accessor);

/**
 * @brief Performs a search for the given query.
 * @param query The search query.
 * @param accessor The IndexAccessor handle for accessing the index.
 * @return A pointer to the Results object containing the search results, or
 * NULL on failure.
 */
Results* fts_search(const char* query, const IndexAccessor* accessor);

/**
 * @brief Deletes the Results object.
 * @param res The Results object to be deleted.
 */
void fts_delete_results(Results* res);

/**
 * @brief Gets the size of the search results.
 * @param res The Results object.
 * @return The size of the search results.
 */
size_t fts_get_results_size(const Results* res);

/**
 * @brief Gets information about a specific result row.
 * @param res The Results object.
 * @param idx The index of the result row.
 * @param accessor The IndexAccessor handle for accessing additional
 * information.
 * @return A CResult struct containing result information.
 */
CResult fts_get_row_info(const Results* res, size_t idx,
                         const IndexAccessor* accessor);

/**
 * @brief Prints the search results to the console.
 * @param res The Results object.
 * @param accessor The IndexAccessor handle for accessing additional
 * information.
 */
void fts_print_result(const Results* res, const IndexAccessor* accessor);

#ifdef __cplusplus
}
#endif
