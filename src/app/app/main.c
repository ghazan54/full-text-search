#include <libsearcher-c/searcher-c.h>

int main(void) {
    TextIndexAccessor *accessor = fts_create_handle();
    fts_search("harry", accessor);
    fts_delete_handle(accessor);
}
