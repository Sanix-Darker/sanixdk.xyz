#include "./lib/lib.h"

int main(int argc, char** argv) {
    char CONTENT_DIR[] = "./content";

    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        // Build the blog list as
        writeMetadatasToBlogList("./content/metadatas.txt",
                                 "./content/blogs.md");

        // build headers/footers for all contents
        buildComponentsIntoMarkdownsFiles("./content");
        buildComponentsIntoMarkdownsFiles("./content/blogs");
        // transform .md -> .html (files)
        proceedFilesRecursively(CONTENT_DIR);

        // minify/bundle htmls/css files
        // FIXME: doesn't sweet well with <pre> tags, will need to update
        // that in the future
        minifyDirfiles("./public/");

        return EXIT_SUCCESS;
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        fprintf(stderr,
                "serve is not implemented; use `make up` for local serving\n");
        return EXIT_FAILURE;
    }

    printf("? Usage: ./builder build|serve");
    return EXIT_FAILURE;
}
