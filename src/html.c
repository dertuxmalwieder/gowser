/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <stdio.h>
#include <str_builder.h>
#include <string.h>

#include "html.h"


void html_from_gopherline(gopher_line_t gopherline, str_builder_t* sb) {
    /* Returns the HTML version of this particular Gopher line. */
    if (gopherline.text.displaystring != NULL &&
        strncmp(gopherline.text.displaystring, " ", strlen(gopherline.text.displaystring)) != 0)
    {
        /* Empty lines happen sometimes. Only process non-empty lines here. */
        char line[500];

        switch (gopherline.text.type) {
            /*
              Text file icons are one character wide.
              Directory icons are two characters wide.
              Leave space accordingly.
             */
        case '0':
            /* Text file */
            snprintf(line, sizeof(line), "&#128441;  <a href=\"gopher://%s:%d/0%s\">%s</a>", gopherline.text.hostname, gopherline.text.port, gopherline.text.selector, gopherline.text.displaystring);
            str_builder_add_str(sb, line, 0);
            break;
        case '1':
            /* Directory */
            snprintf(line, sizeof(line), "&#128449; <a href=\"gopher://%s:%d/1%s\">%s</a>", gopherline.text.hostname, gopherline.text.port, gopherline.text.selector, gopherline.text.displaystring);
            str_builder_add_str(sb, line, 0);
            break;
        case '4':
        case '5':
        case '9':
        case 's':
            /* Binary file */
            /* TODO */
            break;
        case '7':
            /* Full-Text Search */
            /* TODO */
            break;
        case 'g':
        case 'I':
            /* Image file */
            /* TODO */
            break;
        case 'h':
            /* HTML link */
            /* TODO: HTML formatting? */
            snprintf(line, sizeof(line), "&#127760;  <a href=\"gopher://%s:%d/1%s\">%s</a>", gopherline.text.hostname, gopherline.text.port, gopherline.text.selector, gopherline.text.displaystring);
            str_builder_add_str(sb, line, 0);
            break;
        case 'i':
            /* Informational message */
            snprintf(line, sizeof(line), "   <span class=\"info\">%s</span>", gopherline.text.displaystring);
            str_builder_add_str(sb, line, 0);
            break;
        default:
            /* Unsupported */
            str_builder_add_str(sb, gopherline.text.displaystring, 0);
            break;
        }
    }

    str_builder_add_str(sb, "\n", 0); /* Note that we are in a <pre> - no <br> needed nor useful */
}


void html_head(const char* gopherurl, str_builder_t* sb) {
    /* Returns the HTML header for <gopherurl>. */
    char line[200];

    str_builder_add_str(sb, "<html>\n", 0);
    str_builder_add_str(sb, "<head>\n", 0);
    snprintf(line, sizeof(line), "<title>%s</title>\n", gopherurl);
    str_builder_add_str(sb, line, 0);
    str_builder_add_str(sb, "<meta charset=\"utf-8\" />\n", 0);
    str_builder_add_str(sb, "<style type=\"text/css\">\n", 0);
    /* TODO: Consider to allow using personalized CSS? */
    str_builder_add_str(sb, "body { font-family:monospace; font-size: 12px; }\n", 0);
    str_builder_add_str(sb, ".info { color:darkred; }\n", 0);
    str_builder_add_str(sb, "</style>\n", 0);
    str_builder_add_str(sb, "</head>\n", 0);
    str_builder_add_str(sb, "<body>\n", 0);
    str_builder_add_str(sb, "<pre>", 0);
}


void html_foot(str_builder_t* sb) {
    /* Returns the usual HTML footer. */
    str_builder_add_str(sb, "</pre>\n", 0);
    str_builder_add_str(sb, "</body>\n", 0);
    str_builder_add_str(sb, "</html>", 0);
}
