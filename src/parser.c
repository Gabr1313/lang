#pragma once

#include "ast.c"
#include "lexer.c"
#include "arena.c"

typedef struct {
    Arena *arena;
    Lexer *lex;
    Token  currToken;
} Parser;

Parser parser_new(Arena *arena, Lexer *lex) {
    Parser pars    = {};
    pars.arena     = arena;
    pars.lex       = lex;
    pars.currToken = lexer_next_token(lex);
    return pars;
}

void parser_all(Parser pars) {
    while(pars.currToken.type != TokenEof) {
        Token t = pars.currToken;
        if (t.type == TokenEnter) log("%s", tokentype_string[t.type]);
        else                      log("%s: `%.*s`", tokentype_string[t.type], string_print(t.s));
        pars.currToken = lexer_next_token(pars.lex);
    }
} 
