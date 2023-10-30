#include "./lib/lib.h"

int main(int argc, char** argv) {
    char CONTENT_DIR[] = "./content";

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        // set dirs/files
        processDirectoryMarkdowns("./content");
        processDirectoryMarkdowns("./content/blogs");
        createStyleFileAndCopyFavicon();

        // build md -> html
        proceedFilesRecursivelly(CONTENT_DIR);

        // minify/bundle htmls/css files
        // FIXME: doesn't sweet well with <pre> tags, will need to update that
        // in the future
        minifyDirfiles("./public/");
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
