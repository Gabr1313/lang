#pragma once

#include <stdlib.h>

#include "utils.h"

// 64-bit aligned
typedef struct {
	void *first;
	void *ptr;
	u64 cap;
} Arena;

#define arena_push_struct(arena, type)            (type *)arena_push(     (arena), sizeof(type))
#define arena_push_struct_zero(arena, type)       (type *)arena_push_zero((arena), sizeof(type))
#define arena_push_array(arena, type, count)      (type *)arena_push(     (arena), sizeof(type)*(count))
#define arena_push_array_zero(arena, type, count) (type *)arena_push_zero((arena), sizeof(type)*(count))

#define _align_down(what) ( ((u64)(what))         & ~7ull)
#define _align_up(what)   ((((u64)(what)) + 7ull) & ~7ull)

Arena arena_new(u64 size) {
	Arena arena = {};
	arena.cap   = size;
    arena.first = malloc(arena.cap);
	arena.ptr   = arena.first;
	return arena;
}

void arena_free(Arena *arena) {
    free(arena->first);
}

void* arena_push(Arena *arena, u64 bytes) {
	void *retval = arena->ptr;
	arena->ptr = (u8*)(arena->ptr) + bytes;
	arena->ptr = (void*)_align_up(arena->ptr);
	return retval;
}

void* arena_push_zero(Arena *arena, u64 bytes) {
	void *retval = arena_push(arena, bytes);
	for (u64 *p = (u64*)retval; (void*)p < arena->ptr; ++p) {
		*p = 0;
	}
	return retval;
}

void arena_pop(Arena *arena, u64 bytes) {
	arena->ptr = (u8*)(arena->ptr) - bytes;
	arena->ptr = (void*)_align_down(arena->ptr);
}

void arena_clear(Arena *arena) {
	arena->ptr = arena->first;
}
