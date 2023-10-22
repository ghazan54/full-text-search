package main

import (
	"flag"
	"fmt"
	"os"

	"git.csc.sibsutis.ru/cpp2023/fts-kokorin-iv121s13/searcher"
)

func main() {
	var indexPath, query string

	readFlags(&indexPath, &query)

	accessor := searcher.NewHandle(indexPath)

	if accessor == nil {
		panic("accessor == nil")
	}

	defer searcher.DeleteHandle(accessor)

	if query == "" {
		searcher.InteractiveMode(indexPath, accessor)
	} else {
		searcher.SearchAndPrint(indexPath, query, accessor)
	}

}

func readFlags(indexPath, query *string) {
	flag.StringVar(indexPath, "index", "", "Where will the index directory be taken from (REQUIRED)")
	flag.StringVar(query, "query", "", "Query for searcher")

	flag.Parse()

	if *indexPath == "" {
		fmt.Println("--index is required\nRun with --help for more information.")
		os.Exit(1)
	}
}
