#pragma once

typedef enum {
    AstImport
} AstType;

typedef struct {
    AstType type;
    void   *ptr;
} AstNode;

typedef struct _Ast {
    AstNode node;
    struct _Ast *left;
    struct _Ast *right;
} Ast;
