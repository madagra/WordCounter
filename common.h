#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

/* This file contains some common resources for checking errors in the code as 
 * well as some macros which are used throughout the code. All standard library
 * dependencies are also included here. */

#ifndef COMMON_H
#define COMMON_H

/* provided branch predictor information to the compiler to speed up the 
 * most probable IF branches */ 
#define LIKELY(expression) __builtin_expect(!!(expression), 1)

/* debug printing */
#if DEBUG
#define LOG_DEBUG printf
#else
#define LOG_DEBUG(...) do {} while(0) 
#endif

/* check function error code and abort the program
 * if an error is detected */
#define CHECK_ERROR(f) do { \
    int ret = (f);          \
    if(SUCCESS != ret) {    \
        print_error(ret);   \
    }                       \
} while(0)

/* error types */
enum {
    SUCCESS,
    IO_ERROR,
    OUT_OF_MEMORY,
    FAIL
};

struct timeval tp;

void print_error(int);

/* get current time */
static inline double get_time() {
       gettimeofday(&tp, NULL);
          return (double) tp.tv_sec+(1.e-6)*tp.tv_usec;
}

#endif
