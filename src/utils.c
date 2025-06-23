#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef bool     b1;
typedef uint8_t  b8;
typedef uint16_t b16;
typedef uint32_t b32;
typedef uint64_t b64;

#define kilo(x) ((x)*1024ul)
#define mega(x) (kilo(x)*1024ul)
#define giga(x) (mega(x)*1024ul)

#define array_count(a) sizeof(a)/sizeof(a[0])

#define _log(stream, format, ...)  fprintf(stream, format "\n", ##__VA_ARGS__)
#define loginfo(...)  _log(stdout, "[INFO] " __VA_ARGS__)
#define log(...)      _log(stdout, __VA_ARGS__)

#if LANG_UNIX && defined(DBG)
#include <execinfo.h>
#include <dlfcn.h>
#include <unistd.h>

#define err(message, ...)  do {                                                                           \
    _log(stderr, "[ERROR] (%s:%u) " message, __FILE__, __LINE__, ##__VA_ARGS__);                          \
    void *buffer[64];                                                                                     \
    int size = backtrace(buffer, 64);                                                                     \
    for (int i = 0, cnt = 0; i < size-1; i++) {                                                           \
        Dl_info info;                                                                                     \
        if (dladdr(buffer[i], &info)) {                                                                   \
            char cmd[512];                                                                                \
            snprintf(cmd, sizeof(cmd), "addr2line -f -p -e \"%s\" %p", info.dli_fname, buffer[i]);        \
            FILE *fp = popen(cmd, "r");                                                                   \
            if (fp) {                                                                                     \
                char line[512];                                                                           \
                if (fgets(line, sizeof(line), fp) && line[0] != '?') printf("    %-2d: %s", cnt++, line); \
                pclose(fp);                                                                               \
            }                                                                                             \
        }                                                                                                 \
    }                                                                                                     \
    exit(1);                                                                                              \
} while(0)
#else
#define err(message, ...) do {                                                   \
    _log(stderr, "[ERROR] (%s:%u) " message, __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(1);                                                                     \
} while(0)
#endif

#ifdef DBG
#define dbg(...)  _log(stdout, "[DEBUG] " __VA_ARGS__)

#define assert(cond, message, ...)                                      \
    do {                                                                \
        if (!(cond)) {                                                  \
            err("Assert failed `%s` : " message, #cond, ##__VA_ARGS__); \
            exit(1);                                                    \
        }                                                               \
    } while (0)

#else

#define assert(...) do {} while(0)
#define dbg(...)    do {} while(0)

#endif

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
