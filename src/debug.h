/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef GOWSER_DEBUG
#define GOWSER_DEBUG

#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <errno.h>

#define DEBUG 1
#define DEBUGFILE "debug.txt"

/* Debug macro: Prints some debug output to DEBUGFILE (or
   to stderr if DEBUGFILE could not be appended to) if
   enabled. */
#ifdef __STDC_LIB_EXT1__
/* We can use fopen_s() */
#define debug_print(fmt, ...)                                           \
    do {                                                                \
        if (DEBUG) {                                                    \
            FILE* fDebug;                                               \
            errno_t err;                                                \
            if ((err = fopen_s(&fDebug, DEBUGFILE, "a+")) == 0) {       \
                if (fDebug != NULL) {                                   \
                    fprintf(fDebug, "%s:%d:%s(): " fmt, __FILE__,       \
                            __LINE__, __func__, __VA_ARGS__);           \
                    fclose(fDebug);                                     \
                } else {                                                \
                    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__,       \
                            __LINE__, __func__, __VA_ARGS__);           \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (0)
#else
/* C11 is not welcome here. :-( */
#define debug_print(fmt, ...)                                   \
    do {                                                        \
        if (DEBUG) {                                            \
            FILE* fDebug = fopen(DEBUGFILE, "a+");              \
            if (fDebug != NULL) {                               \
                fprintf(fDebug, "%s:%d:%s(): " fmt, __FILE__,   \
                        __LINE__, __func__, __VA_ARGS__);       \
                fclose(fDebug);                                 \
            } else {                                            \
                fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__,   \
                        __LINE__, __func__, __VA_ARGS__);       \
            }                                                   \
        }                                                       \
    } while (0)
#endif

#endif /* GOWSER_DEBUG */
