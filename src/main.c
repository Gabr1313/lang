#include "utils.h"

#include "token.c"
#include "lexer.c"
#include "arena.c"
#include "file.c"

int main(int argc, char** argv) {
    if (argc != 2) err("How to use: `%s path/to/file.j", argv[0]);

    u64 mem_size = giga(1);
    Arena arena = arena_new(mem_size, NULL);
    if (!arena.first) err("Could not allocate %ld byte", mem_size);

    String source = file_to_string(argv[1], &arena);
    Lexer lex = lexer_new(source);
    while(!lexer_finished(&lex)) {
        Token t = lexer_next_token(&lex);
        if (t.type == TokenEnter) log("%s", tokentype_string[t.type]);
        else                      log("%s: `%.*s`", tokentype_string[t.type], string_print(t.s));
    }

    file_string_free_if_not_arena(source, &arena);
    arena_free(&arena);

    return 0;
}
