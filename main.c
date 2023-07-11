#include "./lib/lib.h"

int main(int argc, char** argv) {
    char CONTENT_DIR[] = "./content";

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        // setting everything up
        createDirectories();
        processMarkdownFiles();
        createStyleFileAndCopyFavicon();

        proceed_files_recursivelly(CONTENT_DIR);
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    } else {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
