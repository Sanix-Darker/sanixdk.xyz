/* Global options. */
#ifndef SDK_LIBS

#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "./md4c-html.h"
#include "./md4c.h"

#define RENDER 1
#define BUILD_ARG "build"
#define SERVE_ARG "serve"

#else
static unsigned renderer_flags = MD_HTML_FLAG_DEBUG;
#endif

struct membuffer {
    char* data;
    size_t asize;
    size_t size;
};

// to proveed files recursivelly
void proceed_files_recursivelly(char* basePath);

// for the build
void processFile(const char* filename);
void processMarkdownFiles();
void createDirectories();
void createStyleFileAndCopyFavicon();
