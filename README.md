## Full-Text search

### Overview

Full-Text Search is a software tool developed in the C++ language designed to efficiently search for information in a database of books presented in CSV format. This tool allows users to find books of interest to them based on keywords and phrases as conveniently and efficiently as possible.

### Table of Contents

1. [Installation](#installation)
2. [Usage](#usage)
3. [Examples](#examples)

### Installation

```bash
git clone https://github.com/ghazan54/full-text-search.git
cd full-text-search/
cmake --preset release
cmake --build --preset release
cd build/release/
sudo ninja install
```

### Usage


```bash
fts [SUBCOMMAND] [OPTIONS]
```

### Examples

#### Example 1: indexer

```bash
fts indexer --index . --csv books.csv --config config.json
```

#### Example 2: searcher

```bash
fts searcher --index . --query "harry potter"
```
