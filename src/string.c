#pragma once

#include "utils.c"

typedef struct {
    char *ptr;
    u32 count;
} String;

#define string_print(s) (i32)s.count, s.ptr

String string_from_char(char *t) {
    String s = {0};
    s.ptr    = t;
    while (s.ptr[s.count]) s.count++;
    return s;
}

b1 string_equal(String a, String b) {
    if (a.count != b.count) return false;
    if (a.ptr == b.ptr) return true;
    for (u32 i = 0; i < a.count; i++) {
        if (a.ptr[i] != b.ptr[i]) return false;
    }
    return true;
}

u64 string_hash(String a) {
    u64 hash = 0;
    for (u32 i = 0; i < a.count; i++) hash = (hash*257 + (u64)a.ptr[i]) % 0x7f807f807f7f7d; // biggest prime < 2^64 / 257
    return hash;
}
