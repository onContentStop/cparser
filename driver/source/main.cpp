#include "cparser/lexer.hpp"
#include "cparser/parser.hpp"
#include "cparser/token.hpp"
#include "fmt/core.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    std::span args{argv, static_cast<std::size_t>(argc)};

    if (args.size() > 2) {
        fmt::print(stderr, "Usage: {} [file]\n", args[0]);
        return 1;
    }

    if (args.size() == 2) {
        std::ifstream file{args[1]};
        if (!file) {
            fmt::print(stderr, "Failed to open file: {}\n", args[1]);
            return 1;
        }

        std::ostringstream buffer;
        std::copy(std::istreambuf_iterator{file}, std::istreambuf_iterator<char>{},
                std::ostreambuf_iterator{buffer});

        auto parser = cc::parser{buffer.str()};
        auto expression = parser.parse();
        auto diagnostics = parser.take_diagnostics();
        for (const auto& diagnostic : diagnostics.diagnostics()) {
            fmt::print(stderr, "{}\n", diagnostic);
        }
        return 0;
    }

    std::string line;
    while (true) {
        fmt::print("> ");
        if (!std::getline(std::cin, line)) {
            break;
        }

        cc::parser parser{std::move(line)};
        auto expression = parser.parse();
        auto diagnostics = parser.take_diagnostics();
        for (const auto& diagnostic : diagnostics.diagnostics()) {
            fmt::print(stderr, "{}\n", diagnostic);
        }
        if (diagnostics.diagnostics().empty()) {
            expression->pretty_print();
        }
    }
}
