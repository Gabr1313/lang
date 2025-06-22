#pragma once

typedef enum {
    AstImport
} AstType;

typedef struct {
    void *ptr;
    AstType type;
} AstNode;

typedef struct {
    AstNode node;
    Ast *left;
    Ast *right;
} Ast;
