#pragma once

#include <stdio.h>

#include "utils.h"

#include "arena.c"
#include "token.c"

typedef struct {
    FILE* source;
    i32 curr;
} Lexer;

Lexer lexer_new(FILE* source) {
	Lexer l  = {};
	l.source = source;
    l.curr   = fgetc(source);
	return l;
}

static inline b1 lexer_finished(Lexer *l) {
    return l->curr == EOF;
}

static inline i32 lexer_next_char(Lexer *l) {
    l->curr = fgetc(l->source);
    return l->curr;
}

static inline i64 lexer_get_pos(Lexer *l) {
    return ftell(l->source);
}

static inline void lexer_rewind(Lexer *l, i64 pos) {
    if (fseek(l->source, pos-1, SEEK_SET)) err("Could not set the position in the file");
    lexer_next_char(l);
}

static inline b1 is_white_space(i32 c) {
    return c == ' ' || c == '\t';
}

static inline b1 is_enter(i32 c) {
    return c == '\n' || c == '\r';;
}

static inline b1 is_white_space_or_enter(i32 c) {
    return is_white_space(c) || is_enter(c);
}

static inline b1 is_alpha(i32 c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

static inline b1 is_num(i32 c) {
    return (c >= '0' && c <= '9');
}

static inline b1 is_alpha_num(i32 c) {
    return is_alpha(c) || is_num(c);
}

static inline void skip_white_spaces(Lexer *l) {
    while (!lexer_finished(l) && is_white_space(l->curr)) lexer_next_char(l);
}

void lexer_scan_identifier(Lexer *l) {
    for (u32 i = 0; !lexer_finished(l); i++) {
        if (!is_alpha_num(lexer_next_char(l))) break;
    }
}

void lexer_scan_number(Lexer *l) {
    for (u32 i = 0; !lexer_finished(l); i++) {
        if (!is_num(lexer_next_char(l))) break;
    }
}

Token lexer_next_token(Lexer *l, Arena *arena) {
    skip_white_spaces(l);

    Token t = {};
    if (lexer_finished(l)) {
        t.type = TokenEof;
        return t;
    }

    i64 initial = lexer_get_pos(l);
    if (is_alpha(l->curr)) {
        t.type = TokenIdentifier;
        lexer_scan_identifier(l);
    } else if (is_num(l->curr)) {
        t.type = TokenNumber;
        lexer_scan_number(l);
    } else {

#define char_single_token(_char, _tok) \
            case _char: {              \
                t.type = _tok;         \
                lexer_next_char(l);    \
            } break

#define char_double_token_1(_char, _tok, _2char, _2tok) \
            case _char: {                               \
                i32 _c = lexer_next_char(l);            \
                if (_c == _2char) {                     \
                    t.type = _2tok;                     \
                    lexer_next_char(l);                 \
                } else {                                \
                    t.type = _tok;                      \
                }                                       \
            } break

#define char_double_token_2(_char, _tok, _2char, _2tok, _3char, _3tok) \
            case _char: {                                              \
                i32 _c = lexer_next_char(l);                           \
                if (_c == _2char) {                                    \
                    t.type = _2tok;                                    \
                    lexer_next_char(l);                                \
                } else if (_c == _3char) {                             \
                    t.type = _3tok;                                    \
                    lexer_next_char(l);                                \
                } else {                                               \
                    t.type = _tok;                                     \
                }                                                      \
            } break

        switch (l->curr) {
            char_single_token('\n', TokenEnter);
            char_single_token(';', TokenSemiColon);
            char_single_token(':', TokenColon);
            char_single_token(',', TokenComma);
            char_single_token('(', TokenParenL);
            char_single_token(')', TokenParenR);
            char_single_token('[', TokenBracketL);
            char_single_token(']', TokenBracketR);
            char_single_token('{', TokenCurlyL);
            char_single_token('}', TokenCurlyR);
            char_single_token('#', TokenPound);
            char_single_token('!', TokenBang);
            char_single_token('?', TokenQuestion);
            char_double_token_1('=', TokenAssign, '=', TokenEqual);
            char_double_token_1('.', TokenDot, '.', TokenRange);
            char_double_token_1('<', TokenLess, '=', TokenLessEqual);
            char_double_token_1('>', TokenGreater, '=', TokenGreaterEqual);
            char_double_token_1('*', TokenMultiply, '=', TokenMultiplyEqual);
            char_double_token_2('+', TokenPlus, '=', TokenPlusEqual, '-', TokenIncrement);
            char_double_token_2('-', TokenMinus, '=', TokenMinusEqual, '-', TokenDecrement);
            case '"': {
                t.type = TokenString;
                while (!lexer_finished(l)) {
                    if (lexer_next_char(l) == '"') {
                        lexer_next_char(l);
                        break;
                    }
                }
            } break;
            case '/': {
                i32 c = lexer_next_char(l);
                if (c == '/') {
                    t.type = TokenComment;
                    while (!lexer_finished(l)) {
                        if (is_enter(lexer_next_char(l))) break;
                    }
                } else if (c == '=') {
                    t.type = TokenDivideEqual;
                    lexer_next_char(l);
                } else {
                    t.type = TokenDivide;
                }
            } break;
// default_fake:
            default: {
                t.type = TokenIllegal;
                while (!lexer_finished(l)) {
                    if (is_white_space_or_enter(lexer_next_char(l))) break;
                }
            } break;
        }
    }

    t.s.count        = (u64)(lexer_get_pos(l) - initial);
    t.s.ptr          = arena_push(arena, t.s.count+1);
    t.s.ptr[t.s.count] = '\0';
    if (t.s.count == 0) return t;

    lexer_rewind(l, initial);
    t.s.ptr[0] = (i8)l->curr;
    for (u32 i = 1; i < t.s.count; i++) t.s.ptr[i] = (i8)lexer_next_char(l);
    lexer_next_char(l);
    return t;
}
