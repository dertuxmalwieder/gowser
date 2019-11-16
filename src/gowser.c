/**
 * This is Gowser, a Gopher Browser.
 * https://bitbucket.org/tux_/gowser/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include <stdio.h>
#include <string.h>
#include <locale.h>

#include <curl/curl.h>
#include <iup.h>
#include <iupweb.h>
#include <str_builder.h>

#include "types.h"
#include "debug.h"
#include "gopherfuncs.h"
#include "html.h"
#include "gowser.h"
#include "osfuncs.h"
#include "constants.h"



Ihandle *browser, *urlbar, *histbutton; /* We need to interact with them later. */

int main(int argc, char **argv) {
    /* Initialize navigation history */
    nav_number = -1;

    /* Initialize GUI */
    Ihandle *dlg, *vbox, *hbox, *urllabel;

    IupOpen(&argc, &argv);
    IupWebBrowserOpen();

    urllabel = IupLabel(NULL);
    IupSetStrAttribute(urllabel, "TITLE", "Go to gopher://");

    urlbar = IupText(NULL);
    IupSetAttribute(urlbar, "EXPAND", "HORIZONTAL");
    IupSetAttribute(urlbar, "MARGIN", "5x2");
    IupSetAttribute(urlbar, "BORDER", "NO");
    IupSetCallback(urlbar, "K_ANY", (Icallback) urlbar_callback);

    histbutton = IupButton("back", "history_callback");
    IupSetAttribute(histbutton, "FLAT", "YES");
    IupSetAttribute(histbutton, "ACTIVE", "NO");
    IupSetCallback(histbutton, "ACTION", (Icallback) history_callback);

    hbox = IupHbox(urllabel, urlbar, histbutton, NULL);
    IupSetAttribute(hbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(hbox, "MARGIN", "8x3");

    browser = IupWebBrowser();
    vbox = IupVbox(hbox, browser, NULL);
    IupSetAttribute(browser, "MULTILINE", "YES");
    IupSetAttribute(browser, "EXPAND", "YES");
    IupSetAttribute(browser, "READONLY", "YES");
    IupSetCallback(browser, "NAVIGATE_CB", (Icallback) navigate_callback);

    dlg = IupDialog(vbox);
    IupSetStrAttribute(dlg, "TITLE", "Gowser, a Gopher Browser");
    IupSetAttribute(dlg, "SIZE", "HALFx280");

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(dlg, "USERSIZE", NULL);

    /* argv[1] can be a valid Gopher URL. */
    if (argc > 1) {
        /* At least one argument has been passed. */
        if (argc > 2) {
            /* As Gowser does not have a multi-window
               functionality (yet?), having more than one URL
               parameter is rather pointless. Inform the user
               about that.
            */
            IupMessage("Parameter Fuckup", "Gowser accepts a Gopher URL as a parameter. You gave it more than one parameter. Please don't.");
        }

        go_to_gopher_page(argv[1], 1);
    }
    else {
        /* Visit the start page, */
        go_to_gopher_page("welcome", 1);
    }

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}


static void go_to_gopher_page(char* gopherurl, int save_history) {
    /**
     * Downloads and fills the requested Gopher page into the window.
     * Builds the HTML string on the way.
     */
    int hasRawText = 0;
    download_result_t rawstring;

    /* Gowser has a build-in "start page" under the URI "welcome". */
    int is_welcome_page = (strncmp(gopherurl, "welcome", strlen(gopherurl)) == 0);
    if (is_welcome_page) {
        hasRawText = 1;
        rawstring.success = 1;

        char welcomepage[4096];
        sprintf(welcomepage, "\
               `         `.-/-`.:+-\n\
            `-/hyooos+ossysshyssshyo+oooo/`      ______________________________________________________\n\
      :oosssso++////+s///++o++//////+osysod-    /                                                      \\\n\
     osshyd++shhhho////sdhhyyhh+/////ooo++yo    |               _                            _         |\n\
     y+++sd+sy++hys///+hyo/ymhos////+dys+oh.    |              | |                          | |        |\n\
     .sssds/s.+mmm+////y-.dmmmy//////sdso/`     | __      _____| | ___ ___  _ __ ___   ___  | |_ ___   |\n\
       .od+s:-m/dms///+y yy:mmm//////yd:        | \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  |\n\
        +d+h-shomNs///oh:msdmmms+///+sd/        |  \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) | |\n\
    :ossooysoooshyo+/+sso++++ooss////ohh:       |   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  |\n\
 `+yo///////////hsyms+ymo//////////////+yh-     |                                                      |\n\
 yy/////////////hydmmdhd+///////////s+///+h:    |   __ _  _____      _____  ___ _ __                   |\n\
.mo//yo//////////+odho+///////////+syh+///ym.   |  / _` |/ _ \\ \\ /\\ / / __|/ _ \\ '__|                  |\n\
:Nh//yssso+///////+ho/////////+osso+/o+///om-   | | (_| | (_) \\ V  V /\\__ \\  __/ |                     |\n\
`ys//////ohmmo+oo++ho+oooooohdds//////////om`   |  \\__, |\\___/ \\_/\\_/ |___/\\___|_|                     |\n\
 ods///////sm.     o-      -ms////////////yy    |   __/ |                                              |\n\
 -sh////////yo     ::      so///////////odd.    |  |___/                A Gopher browser.              |\n\
   oy////////h-    .o      h///////////sh+:     \\_____________________________________________________/\n\
    +hss/////+y+///+so+//++s////////+sh+\n\
     `+dh+/////////+ss+//////////oshy/`         hg repository:   <a href='%s'>%s</a>\n\
       `.oyso+//////////////+ymhyo:`            www:             coming soon(ish)\n\
           ./oyhys+//////oyy+//`\n\
                ./oyysoydo.\n\
                    `:ys`\n\
", GOWSER_REPO, GOWSER_REPO);
        int len = strlen(welcomepage);
        rawstring.text = (char*)malloc(len+1);
        strcpy(rawstring.text, welcomepage);
    }
    else {
        rawstring = download_gopher_page(gopherurl);

        /* If the requested page is not a directory (type 1), do not try
         * to parse it. Just fill it into the HTML view and we're good.
         * As strtok() would basically throw away our original Gopher
         * URL, we need a copy first.
         */
        if (rawstring.success > 0) {
            char* gopherurl_copy = calloc(strlen(gopherurl)+1, sizeof(char));
            strncpy_s(gopherurl_copy, strlen(gopherurl) + 1, gopherurl, strlen(gopherurl));

            char* urlparts = strtok(gopherurl_copy, "/");
            char* urlarray[4];

            int i = 0;
            while (urlparts != NULL && i < 4) {
                urlarray[i++] = urlparts;
                urlparts = strtok(NULL, "/");
            }

            free(gopherurl_copy);

            /* If we have a gopher:// URL, the link type is urlarray[2]; else, urlarray[1]. */
            int typefound = (strncmp(gopherurl, "gopher://", 9) == 0) ? 2 : 1;
            if (i > typefound) {
                if (urlarray[typefound] != NULL && strncmp(urlarray[typefound], "1", 1) != 0) {
                    /* Not a directory. */
                    hasRawText = 1;
                }
            }
        }
    }

    str_builder_t* htmlsb = str_builder_create();

    /* Build the HTML: */
    html_head(gopherurl, htmlsb);

    /* Gowser might have had a problem. */
    if (rawstring.success == 0) {
        /* And it actually did. */
        str_builder_add_str(htmlsb, rawstring.text, 0);
        str_builder_add_str(htmlsb, "\n", 0);
    }
    else {
        char* gopher_contents = rawstring.text;
        while (gopher_contents) {
            char* next_line = strchr(gopher_contents, '\n');
            int linelength = next_line ? (next_line - gopher_contents) : strlen(gopher_contents);
            char* temp_str = (char *)malloc(linelength + 1);

            if (temp_str) {
                memcpy(temp_str, gopher_contents, linelength);
                temp_str[linelength] = '\0';

                if (hasRawText) {
                    /* Raw text is raw. */
                    str_builder_add_str(htmlsb, temp_str, 0);
                    str_builder_add_str(htmlsb, "\n", 0);
                }
                else {
                    gopher_line_t this_line = parse_gopher_line(temp_str);
                    html_from_gopherline(this_line, htmlsb);
                }

                free(temp_str);
            }
            else {
                debug_print("malloc() error: Couldn't malloc a new temp_str of length %d!\n", linelength + 1);
            }

            gopher_contents = next_line ? (next_line + 1) : NULL;
        }
    }

    html_foot(htmlsb);

    /* Memo by myself for myself:
       The resulting contents can be /really fucking large/.
       Using IupSetAttribute() will probably exceed the poin-
       ter's range and crash. Use IupSetStrAttribute()
       instead which uses the static string contents.
    */
    char* htmlstring = str_builder_dump(htmlsb, NULL);
    IupSetStrAttribute(browser, "HTML", htmlstring);
    IupSetStrAttribute(urlbar, "VALUE", gopherurl);

    /* Update history: */
    if (save_history == 1 && rawstring.success > 0) {
        if (nav_number+1 < GOWSERHISTORY) {
            strcpy(nav_history[++nav_number], gopherurl);
            debug_print("Added %s to your nav_history[%d].\n", gopherurl, nav_number);
            if (nav_number >= 1) {
                /* We have at least one previous page. */
                IupSetAttribute(histbutton, "ACTIVE", "YES");
            }
        }
        else {
            /* People seriously use more than 250 Gopher pages in one session? */
            debug_print("The maximum history size of %d is reached. Consider a higher number for the next version.\n", sizeof(nav_history));
        }
    }

    /* Clean up: */
    str_builder_destroy(htmlsb);
    if (rawstring.success == 1 && rawstring.text != NULL) {
        free(rawstring.text);
    }
}


static download_result_t download_gopher_page(char gopherurl[200]) {
    /* Tries to download <gopherurl> and return its contents. */
    download_result_t ret;

    CURL *curl_handle;
    CURLcode res;

    struct memstruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    /*
     * Don't assume that the user was wise.
     * We have several possibilities for what could have been
     * passed as a Gopher URL:
     *
     * - gopher://some.host.name/...     << perfect
     * - some.host.name/...              << add gopher://
     * - something completely different  << add gopher:// and hope for the best
     */

    const char* gopherprot = "gopher://";
    if (strncmp(gopherurl, gopherprot, 9) != 0) {
        /* no "gopher://". :-( */
        char gopherurl_final[250];
        snprintf(gopherurl_final, sizeof(gopherurl_final), "%s%s", gopherprot, gopherurl);
        curl_easy_setopt(curl_handle, CURLOPT_URL, gopherurl_final);
    }
    else {
        /* with gopher:// :-) */
        curl_easy_setopt(curl_handle, CURLOPT_URL, gopherurl);
    }
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* get it! */
    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        /* Show the user a hint that we failed. */
        debug_print("Error downloading page %s: %s\n", gopherurl, curl_easy_strerror(res));

        ret.success = 0;
        ret.text = "Sorry - this did not work.";
    }
    else {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         */
        ret.success = 1; /* Signal that everything is fine. */
        ret.text = malloc(chunk.size);
        snprintf(ret.text, chunk.size, "%s", chunk.memory);
    }

    /* cleanup */
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);

    curl_global_cleanup();

    return ret;
}


static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    /* cURL -> Memory Callback.
       Shamelessly borrowed from the cURL web page:
       https://curl.haxx.se/libcurl/c/getinmemory.html
    */

    size_t realsize = size * nmemb;
    struct memstruct *mem = (struct memstruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        debug_print("%s\n", "not enough memory (realloc returned NULL)");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void go_back(void) {
    /* Remove the last history entry and move one up (if applicable) */
    if (nav_number > 0 && nav_history[nav_number] != NULL) {
        go_to_gopher_page(nav_history[--nav_number], 0);
        strcpy(nav_history[nav_number], "\0");

        if (nav_number == 0) {
            /* Disable the "back" button again */
            IupSetAttribute(histbutton, "ACTIVE", "NO");
        }
    }
}


/*
  ---------------------------
  User interface callbacks:
  ---------------------------
*/

static int urlbar_callback(Ihandle *self, int c) {
    /* React on <Return> */
    if (c == K_CR) {
        /* The CarriageReturn was pressed. Pass the input text. */
        char* inputtext = IupGetAttribute(urlbar, "VALUE");

        go_to_gopher_page(inputtext, 1);
        IupSetAttribute(browser, "SCROLLTOPOS", "0,0");
    }
    return IUP_DEFAULT;
}


static int navigate_callback(Ihandle *self, char* url) {
    /* React on link click */
    if (strncmp(url, "gopher://", 9) == 0) {
        /* Clicked a Gopher link. */
        go_to_gopher_page(url, 1);
    }
    else {
        /* Clicked a different link. */
        if (strncmp(url, previous_url, strlen(url)) != 0) {
            /* It is probably unwanted to open an URL multiple times in a row. */
            if (open_external_URL(url) == 1) {
                /* Something went horribly wrong, */
                IupMessage("Oops!", "Gowser failed to start your default application. Would you mind to report this issue?");
            }
            IupSetAttribute(browser, "VALUE", url);
            strncpy(previous_url, url, sizeof(previous_url));
        }
    }

    return IUP_IGNORE;
}


static int history_callback(Ihandle *self) {
    /* React on "back" button click */
    go_back();
    return IUP_DEFAULT;
}
