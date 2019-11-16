/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef GOWSER_TYPES
#define GOWSER_TYPES


/* Content types: */

typedef struct gopher_link {
   char type;
   char* displaystring;
   char selector[100];
   char hostname[100];
   int port;
} gopher_link_t;


typedef struct gopher_line {
    gopher_link_t text;
    int colorpair; /* unused */
} gopher_line_t;


/* Download types: */

typedef struct download_result {
    int success;
    char* text;
} download_result_t;


#endif
