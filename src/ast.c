#pragma once

typedef enum {
    AstImportT
} AstType;

// NOTE: This is only a place holder, for the structures below
typedef struct {
    AstType type;
    void *left;
    void *right;
} Ast;

typedef struct {
    AstType type;
    String lib_name;
} AstImport;

void ast_print(Ast* ast) {
    switch(ast->type) {
        case AstImportT: {
            AstImport* a = (AstImport*)ast;
            log("AstImport: lib_name = %.*s", string_print(a->lib_name));
        }
    }
}
