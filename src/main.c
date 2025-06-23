#if 1
    #if defined(__unix__) || defined(__APPLE__)
        #define LANG_UNIX
    #endif
#endif

#include "utils.c"
#include "keywordsmap.c"
#include "token.c"
#include "lexer.c"
#include "parser.c"
#include "arena.c"
#include "file.c"

int main(int argc, char** argv) {
    if (argc != 2) err("How to use: `%s path/to/file.j", argv[0]);

    u64 mem_size = giga(1);
    Arena arena = arena_new(mem_size, NULL);
    if (!arena.first) err("Could not allocate %ld byte", mem_size);

    String source = file_to_string(argv[1], &arena);
    Lexer lex = lexer_new(source, keywordsmap_new(&arena));

    Parser pars = parser_new(&arena, &lex);
    parser_all(pars);

    file_string_free_if_not_arena(source);
    arena_free(&arena);

    return 0;
}
