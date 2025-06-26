#pragma once

// TODO: many things are stored as String, but I think they should be something else

typedef enum {
    AstImportT,
    AstArgumentListT,
    AstReturnListT,
    AstDeclarationT,
    AstFunctionT,
} AstType;

// NOTE: This is only a place holder, for the structures below
typedef struct {
    AstType ast_type;
} Ast;

typedef struct {
    AstType ast_type;
    String lib_name;
} AstImport;

typedef struct {
    AstType ast_type;
    u32     count;
    String *types;
    String *names;
} AstArgumentList;

typedef struct {
    AstType ast_type;
    u32     count;
    String *types;
} AstReturnList;

typedef struct {
    AstType ast_type;
    String  var_name;
    AstReturnList *retvals;
    b1      constant;
    Ast    *expression;
} AstDeclaration;

typedef struct {
    AstType          ast_type;
    String           name;
    b1               constant;
    AstReturnList   *retvals;
    AstArgumentList *args;
    u32              body_count;
    Ast             *body;
} AstFunction;

void ast_print(Ast* ast, Arena *arena, u32 indent) { // TODO: every case should call a function, in order to be slightly more efficient with recursive calls (no switch in those cases!)
    char pad[indent*4+1];
    for (u32 i = 0; i < indent*4; i++) pad[i] = ' ';
    pad[indent*4] = 0;

    if (ast == NULL) {
        log("%sNULL", pad);
        return;
    }

    switch(ast->ast_type) {
        case AstImportT: {
            AstImport* a = (AstImport*)ast;
            log("%sImport: lib_name = %.*s", pad, string_print(a->lib_name));
        } break;
        case AstArgumentListT: {
            AstArgumentList *a = (AstArgumentList*)ast;
            if (a->count > 0) {                   
                arena_tprint(arena, ": ");
                for (u32 i = 0; i < a->count-1; i++) arena_tprint(arena, "%.*s, ", string_print(a->types[i]));
                arena_tprint(arena, "%.*s",   string_print(a->types[a->count-1]));
                log("%sArgument List: %u%s", pad, a->count, arena_tprint_start);
                arena_tprint_reset();
            } else log("%sArgument List: 0", pad);
        } break;
        case AstReturnListT: {
            AstReturnList *a = (AstReturnList*)ast;
            if (a->count > 0) {                   
                arena_tprint(arena, ": ");
                for (u32 i = 0; i < a->count-1; i++) arena_tprint(arena, "%.*s, ", string_print(a->types[i]));
                arena_tprint(arena, "%.*s",   string_print(a->types[a->count-1]));
                log("%sArgument List: %u%s", pad, a->count, arena_tprint_start);
                arena_tprint_reset();
            } else log("%sArgument List: 0", pad);
        } break;
        case AstDeclarationT: {
            log("%sDeclaration: TODO", pad);
        } break;
        case AstFunctionT: {
            AstFunction *a = (AstFunction*)ast;
            log("%sFunction: %s %.*s", pad, a->constant ? "const" : "var", string_print(a->name));
            ast_print((Ast*)a->args,    arena, indent+1);
            ast_print((Ast*)a->retvals, arena, indent+1);
        } break;
    }
}
