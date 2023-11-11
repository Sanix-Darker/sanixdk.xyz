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

/***** for YamlParsing templating ***/
// Yaml parse|read
// struct to hold the key-value pairs (for key/val hashMaps)
typedef struct {
    char key[256];
    char value[256];
} KeyValuePair;
KeyValuePair* parseYamlFile(const char* filePath, int* numPairs);
const char* getValueByKey(const KeyValuePair* hashMap, int numPairs,
                          const char* key);
