#include "./lib/lib.h"

int main(int argc, char** argv) {
    char CONTENT_DIR[] = "./content";

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        // build headers/footers for all contents
        buildComponentsIntoMarkdownsFiles("./content");
        buildComponentsIntoMarkdownsFiles("./content/blogs");
        // TODO: buildComponentsIntoMarkdownsFiles("./content/projects");

        // transform .md -> .html (files)
        proceedFilesRecursivelly(CONTENT_DIR);

        // minify/bundle htmls/css files
        // FIXME: doesn't sweet well with <pre> tags, will need to update
        // that in the future
        minifyDirfiles("./public/");

        return EXIT_SUCCESS;
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        // no plan to implement it so far because it's docker-compose based
        // serve.
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    }

    printf("? Usage: ./builder build|serve");
    return EXIT_FAILURE;
}
