/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

/* OS-specific functions */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShellApi.h>
#else
/* Assume POSIX: */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#endif
#include "osfuncs.h"


int open_external_URL(char url[250]) {
    /* Opens a http(s) URL. Returns 0 on success, else 1. */
#ifdef _WIN32
    return (int)(ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOW)) > 32 ? 0 : 1;
#else
    /* Assume xdg-open */
    char command_string[260];
    snprintf(command_string, sizeof(command_string), "xdg-open %s", url);
    int retval = system(command_string);
    return retval != NULL ? retval : 1;
#endif
}
