#pragma once

#include "ast.c"
#include "lexer.c"
#include "arena.c"

typedef struct {
    Arena *arena;
    Lexer *lex;
    Token  tok;
    Token  tok_pick;
} Parser;

Parser parser_new(Arena *arena, Lexer *lex) {
    Parser pars   = {};
    pars.arena    = arena;
    pars.lex      = lex;
    pars.tok      = lexer_next_token(lex);
    pars.tok_pick = lexer_next_token(lex);
    return pars;
}

static inline Token parser_next_token(Parser *pars) {
    pars->tok      = pars->tok_pick;
    pars->tok_pick = lexer_next_token(pars->lex);
    return pars->tok;
}

void parser_skip_separtors(Parser *pars) {
    while (token_is_separtor(pars->tok)) parser_next_token(pars);
}

AstImport* parse_pound(Parser *pars) {
    if (parser_next_token(pars).type == TokenImport) {
        if (parser_next_token(pars).type == TokenIdentifier) {
            if (!token_is_separtor(pars->tok_pick)) token_err(pars->tok, "Expected a separator");
            AstImport* expr = arena_push_struct_zero(pars->arena, AstImport);
            expr->type     = AstImportT;
            expr->lib_name = pars->tok.s;
            return expr;
        } else {
            token_err(pars->tok, "Invalid token found after `#import`, expected `%s`", tokentype_string[TokenIdentifier]);
        }
    } else {
        token_err(pars->tok, "Invalid token found after `#`");
    }
}

Ast* parse_expression(Parser *pars) {
    switch (pars->tok.type) {
        case TokenPound: return (Ast*)parse_pound(pars);
        default: token_err(pars->tok, "Invalid token to start an expression");
    }
}

void parser_all(Parser *pars) {
    // while(pars->tok.type != TokenEof) {
    //     Token t = pars->tok;
    //     if (t.type == TokenEnter) log("[%3u:%3u] %s", t.row, t.col, tokentype_string[t.type]);
    //     else                      log("[%3u:%3u] %s: `%.*s`", t.row, t.col, tokentype_string[t.type], string_print(t.s));
    //     parser_next_token(pars);
    // }
    while(pars->tok.type != TokenEof) {
        Ast* ast = parse_expression(pars);
        ast_print(ast);
        parser_skip_separtors(pars);
    }
} 
