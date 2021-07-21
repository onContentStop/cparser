#include "compiler.h"
#include "compiler.hpp"
#include <stdexcept>

extern "C"
{
    extern int phase2lex(void);
    extern void phase2set_in(FILE *);
    extern int phase2lineno;
    typedef void *phase3scan_t;
    extern void phase3_scan_bytes(const char *, size_t, phase3scan_t);
    extern void phase3lex_init(phase3scan_t *);
}

#include <fmt/format.h>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>

static RawSource readSource(std::string_view fileName);
static TokenizedSource tokenize(RawSource source);

constexpr static std::array includePath{
    "",
    "/usr/include",
};

void compile(std::string_view fileName)
{
    RawSource source = readSource(fileName);
    TokenizedSource tokens = tokenize(source);
}

static RawSource readSource(std::string_view fileName)
{
    std::string source;
    FILE *fp = std::fopen(fmt::format("{}", fileName).c_str(), "r");
    if (!fp)
    {
        throw std::runtime_error{fmt::format("couldn't find {}", fileName)};
    }
    phase2set_in(fp);
    char c;
    std::size_t col = 1;
    while ((c = phase2lex()) != EOF)
    {
        if (c == '\n')
        {
            col = 0;
        }
        ++col;
        source.push_back(c);
    }
    return RawSource{
        .source = source,
        .span =
            TextSpan{
                .firstLine = 1,
                .lastLine = static_cast<size_t>(phase2lineno),
                .firstColumn = 1,
                .lastColumn = col,
            },
    };
}

static TokenizedSource tokenize(RawSource source)
{
    std::vector<Token> tokens;
    int line = 1;
    int col = 1;
    phase3scan_t scanner;
    phase3lex_init(&scanner);
    phase3_scan_bytes(source.source.c_str(), source.source.size(), scanner);
    fmt::print("{}\n", source.source);
    // phase4debug = 1;
    while (phase4parse(scanner))
    {
    }
    return TokenizedSource{
        .tokens = tokens,
    };
}

void include_file(const token_list *tokens)
{
    if (tokens->next != nullptr)
    {
        throw std::runtime_error{"too many tokens in #include statement\n"};
    }
    if (tokens->token->type != PP_HEADER_NAME)
    {
        throw std::runtime_error{"expected header name\n"};
    }
    compile(std::string_view{tokens->token->text + 1, tokens->token->text + strlen(tokens->token->text) - 1});
}

void define_simple_macro(const token *name, token_list *replacement)
{
}

void define_function_like_macro(const token *name, identifier_list *parameters, token_list *replacement)
{
}

void define_function_like_variadic_macro(const token *name, token_list *replacement)
{
}

void define_function_like_variadic_macro_with_args(const token *name, identifier_list *parameters,
                                                   token_list *replacement)
{
}

void undefine_macro(const token *name)
{
}
