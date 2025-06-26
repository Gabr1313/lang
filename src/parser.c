#pragma once

#include "ast.c"
#include "lexer.c"
#include "arena.c"

typedef struct {
    Arena *arena;
    Lexer *lex;
    Token  tok;
    Token  pick_tok;
} Parser;

Parser parser_new(Arena *arena, Lexer *lex) {
    Parser pars   = {0};
    pars.arena    = arena;
    pars.lex      = lex;
    pars.tok      = lexer_next_token(lex);
    pars.pick_tok = lexer_next_token(lex);
    return pars;
}

static inline Token parser_next_token(Parser *pars) {
    pars->tok      = pars->pick_tok;
    pars->pick_tok = lexer_next_token(pars->lex);
    return pars->tok;
}

void parser_skip_separtors(Parser *pars) {
    while (token_is_separtor(pars->tok)) parser_next_token(pars);
}

AstImport* parse_pound(Parser *pars) {
    if (parser_next_token(pars).type != TokenImport) token_err(pars->tok, "Invalid token found after `#`");
    if (parser_next_token(pars).type != TokenIdentifier) token_err(pars->tok, "Invalid token found after `#import`, expected `%s`", tokentype_string[TokenIdentifier]);
    AstImport *expr = arena_push_struct_zero(pars->arena, *expr);
    expr->ast_type  = AstImportT;
    expr->lib_name  = pars->tok.s;
    if (!token_is_separtor(parser_next_token(pars))) token_err(pars->tok, "Expected a separator");
    return expr;
}

AstFunction* parse_function_body(Parser *pars, AstDeclaration decl, AstArgumentList *args) {
    AstFunction *func = arena_push_struct_zero(pars->arena, *func);
    if (!decl.constant) token_err(pars->tok, "Functions should always be constant, use `:` istead of `=`"); // TODO: use the right token, not the `{`
    func->ast_type    = AstFunctionT;
    func->name        = decl.var_name;
    func->retvals     = decl.retvals;
    func->constant    = decl.constant;
    func->args        = args;
    parser_next_token(pars);
    for (i32 cnt = 1; cnt > 0; parser_next_token(pars)) {
        cnt += (pars->tok.type == TokenCurlyL) - (pars->tok.type == TokenCurlyR); // TODO
    }
    parser_next_token(pars);
    return func;
}

AstReturnList* parse_return_list(Parser *pars) {
    AstReturnList *rl = arena_push_struct_zero(pars->arena, *rl);
    rl->ast_type      = AstReturnListT;
    if (pars->tok.type != TokenIdentifier) return rl;
    rl->types = arena_push_zero(pars->arena, 4*sizeof(*rl->types));
    rl->types[rl->count++] = pars->tok.s;
    while (parser_next_token(pars).type == TokenComma) {
        if (rl->count == 4) token_err(pars->tok, "TODO: parse more than 4 elements");
        else {
            if (parser_next_token(pars).type == TokenIdentifier) rl->types[rl->count++] = pars->tok.s;
            else token_err(pars->tok, "Expected a type");
        }
    }
    return rl;
}

AstArgumentList* parse_argument_list(Parser *pars) {
    AstArgumentList *al = arena_push_struct_zero(pars->arena, *al);
    al->ast_type        = AstArgumentListT;
    if (pars->tok.type != TokenIdentifier) return al;
    al->types = arena_push_zero(pars->arena, 4*sizeof(*al->types));
    al->names = arena_push_zero(pars->arena, 4*sizeof(*al->names));
    al->types[al->count] = pars->tok.s;
    if (parser_next_token(pars).type == TokenIdentifier) al->names[al->count++] = pars->tok.s;
    else token_err(pars->tok, "Expected a variable name");
    while (parser_next_token(pars).type == TokenComma) {
        if (al->count == 4) token_err(pars->tok, "TODO: parse more than 4 elements");
        else {
            if (parser_next_token(pars).type == TokenIdentifier) al->types[al->count] = pars->tok.s;
            else token_err(pars->tok, "Expected a type");
            if (parser_next_token(pars).type == TokenIdentifier) al->names[al->count++] = pars->tok.s;
            else token_err(pars->tok, "Expected a variable name");
        }
    }
    return al;
}

Ast* parse_declaration(Parser *pars) {
    AstDeclaration decl = {0};
    decl.ast_type       = AstDeclarationT;
    decl.var_name       = pars->tok.s;
    if (parser_next_token(pars).type != TokenColon) token_err_internal(pars->tok, "Called parse_declaration() without a `:` as the second token.");

    parser_next_token(pars);
    decl.retvals = parse_return_list(pars);

    if (pars->tok.type == TokenAssign) decl.constant = 0;
    else if (pars->tok.type == TokenColon)  decl.constant = 1;
    else token_err(pars->tok, "Invalid token, expected `=` or `:`"); // TODO: or `#`

    if (parser_next_token(pars).type == TokenCurlyL) return (Ast*)parse_function_body(pars, decl, parse_argument_list(pars));
    if (pars->tok.type == TokenIdentifier) {
        if (pars->pick_tok.type == TokenIdentifier) return (Ast*)parse_function_body(pars, decl, parse_argument_list(pars));
        // TODO: parse expression
    }


    AstDeclaration *retval = arena_push_struct(pars->arena, *retval);
    memcpy(retval, &decl, sizeof(decl));
    return (Ast*)retval;
}

Ast* parse_identifier(Parser *pars) {
    switch (pars->pick_tok.type) {
        case TokenColon: return (Ast*)parse_declaration(pars);
        default: token_err(pars->pick_tok, "Invalid token found after an identifier");
    }
}

Ast* parse_statement(Parser *pars) {
    switch (pars->tok.type) {
        case TokenPound:      return (Ast*)parse_pound(pars);
        case TokenIdentifier: return (Ast*)parse_identifier(pars);
        default: token_err(pars->tok, "Invalid token to start a statement");
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
        Ast* ast = parse_statement(pars);
        ast_print(ast, pars->arena, 0);
        parser_skip_separtors(pars);
    }
} 
