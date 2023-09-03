#include <libsum/sum.hpp>

#include <CLI/CLI.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        CLI::App app("fts sum");

        int firstCount;
        app.add_option("--first", firstCount, "First count")->required();

        int secondCount;
        app.add_option("--second", secondCount, "Second count")->required();

        CLI11_PARSE(app, argc, argv);

        std::cout << fts::sum(firstCount, secondCount) << '\n';

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
