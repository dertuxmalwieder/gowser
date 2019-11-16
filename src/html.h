/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */


#ifndef GOWSER_HTML
#define GOWSER_HTML

#include <str_builder.h>
#include "types.h"

void html_from_gopherline(gopher_line_t gopherline, str_builder_t* sb);
void html_head(const char* gopherurl, str_builder_t* sb);
void html_foot(str_builder_t* sb);

#endif
