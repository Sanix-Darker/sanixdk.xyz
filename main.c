#include "./lib/lib.h"

int main(int argc, char** argv) {
    char CONTENT_DIR[] = "./content";

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        // set dirs/files
        createDirectories();
        processMarkdownFiles();
        createStyleFileAndCopyFavicon();

        // build md -> html
        proceed_files_recursivelly(CONTENT_DIR);

        // minify/bundle htmls/css files
        const char* minifyPath = "./public/";
        minifyDirfiles(minifyPath);
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        // no plan to implement it so far because it's docker-compose based
        // serve.
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    } else {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
