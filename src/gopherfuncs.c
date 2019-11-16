/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "types.h"
#include "gopherfuncs.h"


gopher_line_t parse_gopher_line(char* inputstring) {
    /* Formats a Gopher line for output in the main window. */
    gopher_line_t ret;
    gopher_link_t link;

    if (inputstring == NULL) {
        /* Bad server is bad. */
        debug_print("Erroneous input NULL string found.\n", NULL);
        link.displaystring = NULL;
    }
    else {
        switch (inputstring[0]) {
        case '0':
        case '1':
            /* Some link. */
            link = generate_link(inputstring);
            break;
        case 'i':  /* Informational messages. */
        default:   /* Whatever. */
            char** line = split_gopher_line(inputstring);
            link.displaystring = line[1];
        }

        link.type = inputstring[0];
    }

    ret.text = link;
    return ret;
}


gopher_link_t generate_link(char inputstring[500]) {
    /* Tries to squeeze all link elements out of the inputstring. */
    gopher_link_t ret;
    char** tokenarray = split_gopher_line(inputstring);

    strncpy_s(&ret.type, 1, tokenarray[0], 1);
    ret.displaystring = tokenarray[1];

    strncpy_s(ret.selector, sizeof(ret.selector), tokenarray[2], sizeof(ret.selector));
    strncpy_s(ret.hostname, sizeof(ret.hostname), tokenarray[3], sizeof(ret.hostname));

    ret.port = atoi(tokenarray[4]);

    return ret;
}


char** split_gopher_line(char* inputstring) {
    /** Returns an array of all 5 regular elements of a Gopher line:
     * [0] = link type
     * [1] = display text
     * [2] = selector
     * [3] = hostname
     * [4] = port
     *
     * Note: The selector is optional. If there is none, [2] will be
     *       skipped.
     */
    static char* tokenarray[5] = {"", "", "", "", ""};

    /* Get the first character as "type" and move the string to the right: */
    char linktype = inputstring[0];
    tokenarray[0] = &linktype;
    inputstring++;

    /* Let us assume that there is a tab character between the elements ... */
    int i = 1;
    char* tokens = strtok(inputstring, "\t");

    while (tokens != NULL) {
        tokenarray[i++] = tokens;
        tokens = strtok(NULL, "\t");
    }

    switch (linktype) {
        /*
          Some links don't have selectors. Those have only 4 elements.
          Detect them and move them to the right where applicable:
        */
    case '0':
    case '1':
        if (i == 4) {
            debug_print("Found a link without a selector. I'll shift the elements.\n", NULL);
            tokenarray[4] = tokenarray[3];
            tokenarray[3] = tokenarray[2];
            tokenarray[2] = "";
        }
        break;
    }

    return tokenarray;
}
