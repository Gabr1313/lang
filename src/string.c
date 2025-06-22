#pragma once

#include "utils.h"

typedef struct {
    i8 *ptr;
    u64 count;
} String;

#define string_print(s) (i32)s.count, s.ptr
