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
// struct mem buffer for writing/reading into files
struct membuffer {
    char* data;
    size_t asize;
    size_t size;
};

// to proceed files recursivelly
void proceedFilesRecursivelly(char* basePath);

// for the build
void buildComponentsIntoMarkdownsFiles(const char* directory);

// for minifying the code output
void minifyDirfiles(const char* path);

// for yaml processing.
#define MAX_ENTRIES 100

typedef struct {
    char path[256];
    char link[256];
    char title[256];
    char image[256];
    char date[20];
    char tags[150];
} Entry;

typedef struct {
    char key[256];  // <- not needed i guess for now.
    Entry entry;
} EntryMap;
void parse_txt(const char* filename, EntryMap entryMap[], int* count);

void writeMetadatasToBlogList(const char* input_filename,
                              const char* output_filename);
