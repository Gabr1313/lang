#pragma once

#include "utils.c"
#include "token.c"
#include "keywordsmap.c"

typedef struct {
    String      source;
    u64         pos;
    KeywordsMap kw_map;
} Lexer;

Lexer lexer_new(String source, KeywordsMap kw_map) {
	Lexer lex  = {};
	lex.source = source;
    lex.pos    = 0;
    lex.kw_map = kw_map;
	return lex;
}

static inline i8 lexer_curr(Lexer *lex) {
    return lex->source.ptr[lex->pos];
}

static inline b1 lexer_finished(Lexer *lex) {
    return lex->pos == lex->source.count;
}

static inline i32 lexer_next_char(Lexer *lex) {
    return lex->source.ptr[++(lex->pos)];
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

static inline void skip_white_spaces(Lexer *lex) {
    while (!lexer_finished(lex) && is_white_space(lexer_curr(lex))) lexer_next_char(lex);
}

void lexer_scan_identifier(Lexer *lex) {
    for (u32 i = 0; !lexer_finished(lex); i++) {
        if (!is_alpha_num(lexer_next_char(lex))) break;
    }
}

void lexer_scan_number(Lexer *lex) {
    for (u32 i = 0; !lexer_finished(lex); i++) {
        if (!is_num(lexer_next_char(lex))) break;
    }
}

#define char_single_token(_char, _tok) \
case _char: {                          \
    t.type = _tok;                     \
    lexer_next_char(lex);              \
} break

#define char_double_token_1(_char, _tok, _2char, _2tok) \
case _char: {                                           \
    i32 _c = lexer_next_char(lex);                      \
    if (_c == _2char) {                                 \
        t.type = _2tok;                                 \
        lexer_next_char(lex);                           \
    } else {                                            \
        t.type = _tok;                                  \
    }                                                   \
} break

#define char_double_token_2(_char, _tok, _2char, _2tok, _3char, _3tok) \
case _char: {                                                          \
    i32 _c = lexer_next_char(lex);                                     \
    if (_c == _2char) {                                                \
        t.type = _2tok;                                                \
        lexer_next_char(lex);                                          \
    } else if (_c == _3char) {                                         \
        t.type = _3tok;                                                \
        lexer_next_char(lex);                                          \
    } else {                                                           \
        t.type = _tok;                                                 \
    }                                                                  \
} break
Token lexer_next_token(Lexer *lex) {
    skip_white_spaces(lex);

    Token t = {};
    if (lexer_finished(lex)) {
        t.type = TokenEof;
        return t;
    }

    t.s.ptr = lex->source.ptr + lex->pos;
    if (is_alpha(lexer_curr(lex))) {
        lexer_scan_identifier(lex);
        t.s.count = (u32)(lex->source.ptr + lex->pos - t.s.ptr);
        t.type    = keywordmap_find(&lex->kw_map, t.s);
    } else if (is_num(lexer_curr(lex))) {
        t.type = TokenNumber;
        lexer_scan_number(lex);
    } else {
        switch (lexer_curr(lex)) {
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
                while (!lexer_finished(lex)) {
                    if (lexer_next_char(lex) == '"') {
                        lexer_next_char(lex);
                        break;
                    }
                }
            } break;
            case '/': {
                i32 c = lexer_next_char(lex);
                if (c == '/') {
                    t.type = TokenComment;
                    while (!lexer_finished(lex)) {
                        if (is_enter(lexer_next_char(lex))) break;
                    }
                } else if (c == '=') {
                    t.type = TokenDivideEqual;
                    lexer_next_char(lex);
                } else {
                    t.type = TokenDivide;
                }
            } break;
// default_fake:
            default: {
                t.type = TokenIllegal;
                while (!lexer_finished(lex)) {
                    if (is_white_space_or_enter(lexer_next_char(lex))) break;
                }
            } break;
        }
    }

    t.s.count = (u32)(lex->source.ptr + lex->pos - t.s.ptr);
    return t;
}
