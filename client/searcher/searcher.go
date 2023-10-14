package searcher

// #cgo CFLAGS: -I./../../src/libsearcher-c/
// #cgo LDFLAGS: -L./../../build/release/src/libsearcher-c/ -lsearcher-c -Wl,-rpath,./../../build/release/src/libsearcher-c/
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

	line.SetCtrlCAborts(true)

	for {
		input, err := line.Prompt("> ")
		if err != nil {
			if err == liner.ErrPromptAborted {
				fmt.Println("Aborted")
			} else {
				fmt.Println("Error reading line:", err)
			}
			break
		}

		Cls()

		if input == "" {
			break
		}

		SearchAndPrint(indexPath, input, accessor)

		line.AppendHistory(input)
	}
}

func Cls() {
	cmd := exec.Command("clear")
	cmd.Stdout = os.Stdout
	cmd.Run()
}
