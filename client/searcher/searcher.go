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

func SearchAndPrint(indexPath, query string, accessor *C.IndexAccessor) {
	res := C.fts_search(C.CString(query), accessor)
	C.fts_print_result(res, accessor)
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
