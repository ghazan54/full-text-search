package main

// #cgo CFLAGS: -I./../src/libsearcher-c/
// #cgo LDFLAGS: -L./../build/debug/src/libsearcher-c/ -lsearcher-c -Wl,-rpath,./../build/debug/src/libsearcher-c/
// #include <libsearcher-c/searcher-c.h>
import "C"
import (
	"bufio"
	"flag"
	"fmt"
	"os"
	"unsafe"

	"github.com/inancgumus/screen"
)

func main() {
	var indexPath, query string

	readFlags(&indexPath, &query)

	accessor := C.fts_create_handle(strToCStr(indexPath))

	if query == "" {
		searcher_interactive_mode(indexPath, accessor)
	} else {
		searcher_search_and_print(indexPath, query, accessor)
	}

	C.fts_delete_handle(accessor)
}

func readFlags(indexPath, query *string) {
	flag.StringVar(indexPath, "index", "", "Where will the index directory be taken from")
	flag.StringVar(query, "query", "", "Query for searcher")

	flag.Parse()

	if *indexPath == "" {
		panic("--index is required\nRun with --help for more information.")
	}
}

func searcher_search_and_print(indexPath, query string, accessor *C.IndexAccessor) {
	res := C.fts_search(strToCStr(query), accessor)
	C.fts_print_result(res, accessor)
}

func searcher_interactive_mode(indexPath string, accessor *C.IndexAccessor) {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print(">> ")
		query, err := reader.ReadString('\n')
		cls()
		if err != nil {
			panic(err)
		}

		if query == "" {
			break
		}

		searcher_search_and_print(indexPath, query, accessor)

	}
}

func strToCStr(str string) *C.char {
	bstr := []byte(str)
	return (*C.char)(unsafe.Pointer(&bstr[0]))
}

func cls() {
	screen.Clear()
	screen.MoveTopLeft()
}
