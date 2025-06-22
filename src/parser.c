#pragma once

#include "lexer.c"

typedef struct {
    Arena *arena;
    Lexer *lex;
} Parser;

Parser parser_new(Arena *arena,Lexer *lex) {
    Parser pars = {};
    pars.arena  = arena;
    pars.lex    = lex;
    return pars;
}

Ast parser_expression(Parse pars) {
}

void parser_all(Parser pars) {
    while(!lexer_finished(pars.lex)) {
        parser_expression(pars);
    }
}
