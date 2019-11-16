/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */


#ifndef GOWSER_H
#define GOWSER_H


#include <stdlib.h>
#include "types.h"


struct memstruct {
  char*  memory;
  size_t size;
};


/* History-related: */

#define MAXURLLENGTH 250
#define GOWSERHISTORY 250
char    previous_url[MAXURLLENGTH]; /* not for internal history */

char    nav_history[GOWSERHISTORY][MAXURLLENGTH];
int     nav_number;
void    go_back(void);


/* Other functions: */

static void               go_to_gopher_page(char* gopherurl, int save_history);
static download_result_t  download_gopher_page(char gopherurl[200]);


/* Callbacks: */

static size_t             write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);
static int                urlbar_callback(Ihandle *self, int c);
static int                navigate_callback(Ihandle *self, char* url);
static int                history_callback(Ihandle *self);


#endif
