#include "utils.h"

#include "token.c"
#include "lexer.c"
#include "arena.c"

int main(int argc, char** argv) {
    if (argc != 2) err("How to use: `%s path/to/file.j", argv[0]);
    FILE* source = fopen(argv[1], "r");
    if (source == NULL) err("No file named `%s` found", argv[1]);

    u64 mem_size = giga(1);
    Arena arena = arena_new(mem_size);
    if (!arena.first) err("Could not allocate %ld byte", mem_size);
    Lexer l = lexer_new(source);

    while(!lexer_finished(&l)) {
        Token t = lexer_next_token(&l, &arena);
        if (t.type == TokenEnter) log("%s", tokentype_string[t.type]);
        else                      log("%s: `%s`", tokentype_string[t.type], t.s.ptr);
    }

    arena_free(&arena);

    return 0;
}
