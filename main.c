#include "main.h"

#define RENDER 1
#define BUILD_ARG "build"
#define SERVE_ARG "serve"

/* Global options. */
static unsigned parser_flags = 0;
#ifndef MD4C_USE_ASCII
static unsigned renderer_flags =
    MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;
#else
static unsigned renderer_flags = MD_HTML_FLAG_DEBUG;
#endif

struct membuffer {
    char* data;
    size_t asize;
    size_t size;
};

static void membuf_init(struct membuffer* buf, MD_SIZE new_asize) {
    buf->size = 0;
    buf->asize = new_asize;
    buf->data = malloc(buf->asize);
    if (buf->data == NULL) {
        fprintf(stderr, "membuf_init: malloc() failed.\n");
        exit(1);
    }
}

static void membuf_fini(struct membuffer* buf) {
    if (buf->data) free(buf->data);
}

static void membuf_grow(struct membuffer* buf, size_t new_asize) {
    buf->data = realloc(buf->data, new_asize);
    if (buf->data == NULL) {
        fprintf(stderr, "membuf_grow: realloc() failed.\n");
        exit(1);
    }
    buf->asize = new_asize;
}

static void membuf_append(struct membuffer* buf, const char* data,
                          MD_SIZE size) {
    if (buf->asize < buf->size + size)
        membuf_grow(buf, buf->size + buf->size / 2 + size);
    memcpy(buf->data + buf->size, data, size);
    buf->size += size;
}

void replaceString(char* original, char* toReplace, char* replacement) {
    char buffer[1000] = {0};
    char* insertPoint = &buffer[0];
    const char* tmp = original;
    size_t toReplaceLen = strlen(toReplace);
    size_t replacementLen = strlen(replacement);

    while (1) {
        const char* p = strstr(tmp, toReplace);

        if (p == NULL) {
            strcpy(insertPoint, tmp);
            break;
        }

        memcpy(insertPoint, tmp, p - tmp);
        insertPoint += p - tmp;
        memcpy(insertPoint, replacement, replacementLen);
        insertPoint += replacementLen;
        tmp = p + toReplaceLen;
    }

    strcpy(original, buffer);
}

static void process_output(const MD_CHAR* text, MD_SIZE size, void* userdata) {
    membuf_append((struct membuffer*)userdata, text, size);
}

static int process_file(FILE* in, FILE* out, char* page_title) {
    size_t n;
    struct membuffer buf_in = {0};
    struct membuffer buf_out = {0};
    int ret = -1;
    clock_t t0, t1;

    membuf_init(&buf_in, 32 * 1024);

    /* Read the input file into a buffer. */
    while (1) {
        if (buf_in.size >= buf_in.asize)
            membuf_grow(&buf_in, buf_in.asize + buf_in.asize / 2);

        n = fread(buf_in.data + buf_in.size, 1, buf_in.asize - buf_in.size, in);
        if (n == 0) break;
        buf_in.size += n;
    }

    /* Input size is good estimation of output size. Add some more reserve to
     * deal with the HTML header/footer and tags. */
    membuf_init(&buf_out, (MD_SIZE)(buf_in.size + buf_in.size / 8 + 64));

    /* Parse the document. This shall call our callbacks provided via the
     * md_renderer_t structure. */
    t0 = clock();

    ret = md_html(buf_in.data, (MD_SIZE)buf_in.size, process_output,
                  (void*)&buf_out, parser_flags, renderer_flags);

    t1 = clock();
    if (ret != 0) {
        fprintf(stderr, "Parsing failed.\n");
        goto out;
    }

    /* Write down the document in the HTML format. */
    build_header_web_page(out, page_title);
    fwrite(buf_out.data, 1, buf_out.size, out);
    fprintf(out, "</div></body></html>");

    if (t0 != (clock_t)-1 && t1 != (clock_t)-1) {
        double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
        if (elapsed < 1)
            fprintf(stderr, "Time spent on parsing: %7.2f ms.\n",
                    elapsed * 1e3);
        else
            fprintf(stderr, "Time spent on parsing: %6.3f s.\n", elapsed);
    }

    /* Success if we have reached here. */
    ret = 0;

out:
    membuf_fini(&buf_in);
    membuf_fini(&buf_out);

    return ret;
}

void proceedFilesRecursively(char* basePath) {
    char path[1000];
    char input_path[1000];
    char output_path[1000];

    struct dirent* dp;
    struct stat filestat;
    DIR* dir = opendir(basePath);

    FILE* in = stdin;
    FILE* out = stdout;

    if (!dir) {
        printf("Unable to open directory: %s\n", basePath);
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            sprintf(path, "%s/%s", basePath, dp->d_name);
            if (stat(path, &filestat) == -1) {
                continue;
            }
            if (S_ISDIR(filestat.st_mode)) {
                proceedFilesRecursively(path);
            } else if (strstr(dp->d_name, ".md") != NULL) {
                strcpy(input_path, path);
                strcpy(output_path, path);

                printf(">>> building %s\n", input_path);

                // we replace content by public since we're builing the website
                replaceString(output_path, "content", "public");
                replaceString(output_path, ".md", ".html");

                in = fopen(input_path, "rb");
                if (in == NULL) {
                    fprintf(stderr, "<< Cannot open %s\n", input_path);
                    exit(1);
                }

                out = fopen(output_path, "w+");
                if (out == NULL) {
                    fprintf(stderr, ">> Cannot open %s\n", output_path);
                    exit(1);
                }

                process_file(in, out, "");
                if (in != stdin) fclose(in);
                if (out != stdout) fclose(out);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        proceedFilesRecursively("./content");
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    } else {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void build_header_web_page(FILE* out, char* page_title) {
    fprintf(out, "<!DOCTYPE html><html><head><title>dk | %s </title>",
            page_title);
    fprintf(
        out,
        "<meta name = \"robots\" content = \"index,follow\" />"
        "<meta name = \"twitter:card\" content = \"summary_large_image\" />"
        "<meta name = \"twitter:site\" content = \"@sanixdarker\" />"
        "<meta name = \"twitter:creator\" content = \"@sanixdarker\" />"
        "<meta property = \"og:title\" content = \"dk\" />"
        "<meta property = \"og:description\" content = \"\" />"
        "<meta property = \"og:url\" content = \"https://sxdk.xyz\" />"
        "<meta property = \"og:image\" content = \"https://sxdk.xyz/og.png\" />"
        "<meta property = \"og:image:alt\" content = \"Og Image Alt\" />"
        "<meta property = \"og:image:width\" content = \"1200\" />"
        "<meta property = \"og:image:height\" content = \"640\" />"
        "<meta property = \"og:site_name\" content = \"dk\" />"
        "<link rel = \"canonical\" href = \"https://sxdk.xyz\" />"
        "<meta charSet = \"utf-8\" />"
        "<meta content = \"initial-scale=1.0, width=device-width\" name = "
        "\"viewport\" /><meta content = \"#009efa\" name = \"theme-color\" />"
        "<link rel=\"stylesheet\" href=\"/style.css\"/>"
        "<link href = \"/favicon.svg\" rel = \"icon\" />");

    fprintf(out, "<meta http-equiv=\"content-language\" content=\"en-us,fr\">");
    fprintf(out, "</head><body><div class=\"container\">");
}
