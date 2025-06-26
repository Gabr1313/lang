#pragma once

#include "string.c"
#include "token.c"
#include "arena.c"

#define STRINGMAP_MAX_FILL 2

typedef struct {
    String    s;
    TokenType type;
} KeywordsBucket;

// NOTE `String.count == 0` is considered as tombstone value
typedef struct {
    KeywordsBucket *ptr;
    u32 cap_exp;
    u32 count;
    Arena *arena;
} KeywordsMap;

static inline b1 keywordmap_is_occupied(KeywordsMap *hm, u32 idx) {
    return hm->ptr[idx].s.count != 0;
}

TokenType keywordmap_find(KeywordsMap *hm, String x) {
    u32 hash = (u32)string_hash(x);
    u32 mask = (1u << hm->cap_exp)-1;
    for (u32 pos = hash&mask; keywordmap_is_occupied(hm, pos); pos = (pos+1)&mask) {
        if (string_equal(hm->ptr[pos].s, x)) return hm->ptr[pos].type;
        assert(((pos+1)&mask) != (hash&mask), "Hashmap insert went around the clock");
    }
    return TokenIdentifier;
}

void keywordmap_insert(KeywordsMap *hm, String x, TokenType type) {
    assert(hm->count*STRINGMAP_MAX_FILL <= (1u << hm->cap_exp), "You shuld have created a bigger hashmap!"); 
    u32 mask = (1u << hm->cap_exp)-1;
    u32 pos;
    u32 hash = (u32)string_hash(x);
    for (pos = hash&mask; keywordmap_is_occupied(hm, pos); pos = (pos+1)&mask) {
        if (string_equal(hm->ptr[pos].s, x)) break;
        assert(((pos+1)&mask) != (hash&mask), "Hashmap insert went around the clock");
    }
    hm->ptr[pos].s    = x;
    hm->ptr[pos].type = type;
}

KeywordsMap keywordsmap_new(Arena *arena) {
    KeywordsMap hm = {0};
    hm.arena       = arena;
    hm.cap_exp     = 4;
    hm.ptr         = arena_push_zero(hm.arena, sizeof(*hm.ptr)*(1u << hm.cap_exp));
    hm.count       = 0;

    keywordmap_insert(&hm, string_from_char("import"), TokenImport);
    keywordmap_insert(&hm, string_from_char("if"),     TokenIf);
    keywordmap_insert(&hm, string_from_char("else"),   TokenElse);
    keywordmap_insert(&hm, string_from_char("return"), TokenReturn);
    keywordmap_insert(&hm, string_from_char("for"),    TokenFor);
    keywordmap_insert(&hm, string_from_char("in"),     TokenIn);

    return hm;
}
