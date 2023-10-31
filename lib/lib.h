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

#include "./jinjac/jinjac.h"
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

/***** for jinja templating ***/
// struct just like hashmap key-value
typedef struct {
    const char* key;
    const char* value;
} KeyValue;

// to proceed files recursivelly
void proceedFilesRecursivelly(char* basePath);

// for the build
void processFile(const char* filename);
void createStyleFileAndCopyFavicon();
void processDirectoryMarkdowns();

// for minifying the code output
void minifyDirfiles(const char* path);
