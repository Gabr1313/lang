#pragma once

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
#define err(message, ...)  do {                                                  \
    _log(stderr, "[ERROR] (%s:%u) " message, __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(1);                                                                     \
} while(0)

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
