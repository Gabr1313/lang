#pragma once

#include "string.c"

typedef enum {
    TokenIllegal,
    TokenEof,
     
    // Symbols
    TokenComment,
    TokenEnter,
    TokenSemicolon,
    TokenColon,
    TokenComma,
    TokenEqual,
    TokenAssign,
    TokenPlus,
    TokenPlusEqual,
    TokenMinus,
    TokenMinusEqual,
    TokenMultiply,
    TokenMultiplyEqual,
    TokenDivide,
    TokenDivideEqual,
    TokenLess,
    TokenLessEqual,
    TokenGreater,
    TokenGreaterEqual,
    TokenIncrement,
    TokenDecrement,
    TokenParenL,
    TokenParenR,
    TokenBracketL,
    TokenBracketR,
    TokenCurlyL,
    TokenCurlyR,
    TokenBang,
    TokenQuestion,
    TokenDot,
    TokenIdentifier,
    TokenString,
    TokenNumber,
    TokenRange,
    TokenPound,
    
    // Keywords
    TokenImport,
    TokenIf,
    TokenElse,
    TokenReturn,
    TokenFor,
    TokenIn,
} TokenType;

char *tokentype_string[] = {
    "TokenIllegal",
    "TokenEof",

    // Symbols
    "TokenComment",
    "TokenEnter",
    "TokenSemicolon",
    "TokenColon",
    "TokenComma",
    "TokenEqual",
    "TokenAssign",
    "TokenPlus",
    "TokenPlusEqual",
    "TokenMinus",
    "TokenMinusEqual",
    "TokenMultiply",
    "TokenMultiplyEqual",
    "TokenDivide",
    "TokenDivideEqual",
    "TokenLess",
    "TokenLessEqual",
    "TokenGreater",
    "TokenGreaterEqual",
    "TokenIncrement",
    "TokenDecrement",
    "TokenParenL",
    "TokenParenR",
    "TokenBracketL",
    "TokenBracketR",
    "TokenCurlyL",
    "TokenCurlyR",
    "TokenBang",
    "TokenQuestion",
    "TokenDot",
    "TokenIdentifier",
    "TokenString",
    "TokenNumber",
    "TokenRange",
    "TokenPound", 

    // Keywords
    "TokenImport",
    "TokenIf",
    "TokenElse",
    "TokenReturn",
    "TokenFor",
    "TokenIn",
};

typedef struct {
    TokenType type;
    String    s;
    u32 row, col;
} Token;

static inline b1 token_is_separtor(Token tok) {
    return tok.type == TokenEnter || tok.type == TokenSemicolon;
}

#define token_err(tok, str, ...) \
    err(str ": got `%.*s` of type %s at [%u:%u]", ##__VA_ARGS__, string_print((tok).s), tokentype_string[(tok).type], (tok).row, (tok).col)
