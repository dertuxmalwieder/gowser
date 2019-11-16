/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef GOWSER_UI
#define GOWSER_UI


#include "types.h"


/* line-related: */
gopher_line_t parse_gopher_line(char* inputstring);
gopher_link_t generate_link(char inputstring[500]);

char**        split_gopher_line(char* inputstring);


#endif
