package searcher

// #include <libsearcher-c/searcher-c.h>
import "C"

import (
	"fmt"
	"os"
	"os/exec"

	"github.com/peterh/liner"
)

func NewHandle(indexPath string) *C.IndexAccessor {
	return C.fts_create_handle(C.CString(indexPath))
}

func DeleteHandle(accessor *C.IndexAccessor) {
	C.fts_delete_handle(accessor)
}

func printResults(result *C.Results, accessor *C.IndexAccessor) {
	for i := C.size_t(0); i < C.fts_get_results_size(result); i++ {
		row := C.fts_get_row_info(result, i, accessor)
		fmt.Printf("%d %.2f %s\n", row.doc_id, row.score, C.GoString(row.str))
	}
}

func SearchAndPrint(indexPath, query string, accessor *C.IndexAccessor) {
	result := C.fts_search(C.CString(query), accessor)
	if result == nil {
		panic("result == nil")
	}
	defer C.fts_delete_results(result)
	// C.fts_print_result(result, accessor)
	printResults(result, accessor)
}

func InteractiveMode(indexPath string, accessor *C.IndexAccessor) {
	line := liner.NewLiner()
	defer line.Close()

	line.SetCtrlCAborts(false)

	for {
		input, err := line.Prompt("> ")
		if err != nil {
			fmt.Println(err)
			break
		}

		if input == "" {
			break
		}

		cls()

		SearchAndPrint(indexPath, input, accessor)

		line.AppendHistory(input)
	}
}

func cls() {
	cmd := exec.Command("clear")
	cmd.Stdout = os.Stdout
	cmd.Run()
}
