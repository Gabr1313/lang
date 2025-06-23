#pragma once

#include "string.c"

typedef enum {
    TokenIllegal,
    TokenEof,
     
    // Symbols
    TokenComment,
    TokenEnter,
    TokenSemiColon,
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
    "TokenSemiColon",
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
} Token;
