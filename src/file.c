#pragma once

#include "string.c"
#include "arena.c"

#if LANG_UNIX 
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// `arena` is ignored
String file_to_string(char *filename, Arena *_arena) {
    (void)_arena;
    int fd = open(filename, O_RDONLY);
    if (fd == -1) err("Failed to open file `%s`", filename);
    struct stat st;
    if (fstat(fd, &st) == -1) err("Failed to read the stats of file `%s`", filename);
    String s = {0};
    s.count = (u32)st.st_size;
    s.ptr = mmap(NULL, s.count, PROT_READ, MAP_PRIVATE, fd, 0);
    if (s.ptr == MAP_FAILED) err("Failed to map file `%s` to memory", filename); 
    if (close(fd) == -1) assert(false, "Failed to close file");
    return s;
}
void file_string_free_if_not_arena(String s) {
    if (munmap(s.ptr, s.count)) assert(false, "Failed to unmap memory");
}
#else
String file_to_string(char *filename, Arena *arena) {
    FILE* file= fopen(filename, "r");
    if (file == NULL) err("Failed to open file `%s`", filename);
    fseek(file, 0, SEEK_END);
    String s = {0};
    s.count = (u32)ftell(file);
    rewind(file);
    s.ptr = arena_push(arena, s.count);
    fread(s.ptr, 1, s.count, file);
    if (fclose(file) == -1) assert(false, "Failed to close file");
    return s;
}

// noop
void file_string_free_if_not_arena(String _source) {
    (void)_source;
}
#endif
