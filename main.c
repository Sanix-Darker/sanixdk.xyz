#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./md4c/md4c.h"

#define RENDER 1
#define BUILD_ARG "build"
#define SERVE_ARG "serve"

struct Component {
    u_int8_t component_id;  // this will never reach > 10 we should be fine
    char content[100];
    bool need_rendering;
};

struct Node {
    struct Component content;
    int child;  // this should be a pointer to the next component_id ( i don't
                // know yet how am going to mixed that up tho )
};

struct Layout {
    struct Node nodes[20];  // I fix myself no never reach this limit
};

// Convert the yaml to html and specify the component as {{
// input-component(component-path) }} that are going to be fill up later with
// the appropriate component read the

// Convert the markdown component into html

// inject each converted markdown components to the built yaml-html files

int main(int argc, char* argv[]) {
    /* u_int8_t call = 0; */

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        printf("Hello there, am working !!");
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    } else {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    // get markdown components
    // build an html tree with it
    return EXIT_SUCCESS;
}
