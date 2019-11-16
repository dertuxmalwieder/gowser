# gowser: a Gopher browser.

## build requirements:

* [IUP](http://webserver2.tecgraf.puc-rio.br/iup/)
* [curl](http://curl.haxx.se/)

Requires MS Windows or the GTK+ libraries.

## build.cmd for Windows with Clang:

    @echo off
    cd repo
    
    REM build
    clang-cl -m32 /Wall /D_CRT_SECURE_NO_WARNINGS /DCURL_STATICLIB /DU_STATIC_IMPLEMENTATION /Ox /MT /GS /Febin\gowser.exe src\3rdparty\stringbuilder\str_builder.c src\gopherfuncs.c src\osfuncs.c src\html.c src\gowser.c /I"..\includes" /I"..\includes\IUP" /I "src\3rdparty\stringbuilder" /link /libpath:..\libs libcurl_a.lib iup.lib iupole.lib iupweb.lib gdi32.lib shell32.lib ole32.lib comdlg32.lib comctl32.lib user32.lib /SAFESEH /DYNAMICBASE /SUBSYSTEM:WINDOWS /OPT:REF,ICF
    
    REM Cleanup
    if exist *.obj del *.obj
    
    cd ..

I'll add proper Makefiles later or something.
