#include <libsum/sum.hpp>

#include <CLI/CLI.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        CLI::App app("fts sum");

        int first_count = 0;
        app.add_option("--first", first_count, "First count")->required();

        int second_count = 0;
        app.add_option("--second", second_count, "Second count")->required();

        CLI11_PARSE(app, argc, argv);

        std::cout << fts::sum(first_count, second_count) << '\n';

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
