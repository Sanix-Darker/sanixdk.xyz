#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "./md4c/md4c-html.h"
#include "./md4c/md4c.h"

#define RENDER 1
#define BUILD_ARG "build"
#define SERVE_ARG "serve"

// paths for convertion
static char* input_path = NULL;
static char* output_path = NULL;

/* Global options. */
static unsigned parser_flags = 0;
#ifndef MD4C_USE_ASCII
static unsigned renderer_flags =
    MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;
#else
static unsigned renderer_flags = MD_HTML_FLAG_DEBUG;
#endif

// Unfortunatelly we need some small css style
char* css_style =
    "import "
    "url(http://fonts.googleapis.com/"
    "css?family=Roboto:400,100,100italic,300,300ita‌lic,400italic,500,"
    "500italic,700,700italic,900italic,900);"
    "html,body,html * {"
    "  font-family: 'Roboto', sans-serif;"
    "}"
    ".container {"
    "   padding-left: 25%;"
    "   padding-right: 25%;"
    "}"
    "@media screen and (max-width: 1500px) {"
    "  .container {"
    "    padding-left: 15%;"
    "    padding-right: 15%;"
    "  }"
    "}"
    "@media screen and (max-width: 1100px) {"
    "  .container {"
    "    padding-left: 10%;"
    "    padding-right: 10%;"
    "  }"
    "}"
    "@media screen and (max-width: 768px) {"
    "  .container {"
    "    padding-left: 5%;"
    "    padding-right: 5%;"
    "  }"
    "}"
    "pre {"
    "  background-color: #f6f8fa;"
    "  border-radius: 3px;"
    "  padding: 16px;"
    "}"
    "code {"
    "  color: #24292e;"
    "  font-family: 'Courier New', Courier, monospace;"
    "  font-size: 14px;"
    "}"
    "@media (prefers-color-scheme: dark) {"
    "body {"
    "  background-color: #1a1a1a;"
    "    color: #fff;"
    "  }"
    "  pre {"
    "    background-color: #2d2d2d;"
    "  }"
    "  code {"
    "    color: #ccc;"
    "  }"
    "}";

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

void removeNewLines(char* str) {
    char* p = str;
    char* q = str;

    while (*p) {
        if (*p != '\n') {
            *q++ = *p;
        }
        p++;
    }
    *q = '\0';
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
    fprintf(out,
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n<title>dk | %s",
            page_title);
    fprintf(out,
            "</title>\n"
            "<meta charset=\"UTF-8\">");
    fprintf(out, "<style>%s</style>\n", css_style);
    fprintf(out,
            "</head>\n"
            "<body>"
            "<div class=\"container\">\n");
    fwrite(buf_out.data, 1, buf_out.size, out);
    fprintf(out, "</div></body>\n");
    fprintf(out, "</html>\n");

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

void proceedFilesRecursively(char* basePath, FILE* in, FILE* out) {
    char path[1000];
    struct dirent* dp;
    struct stat filestat;
    DIR* dir = opendir(basePath);

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
                proceedFilesRecursively(path, in, out);
            } else if (strstr(dp->d_name, ".md") != NULL) {
                printf(">>> building %s\n", path);

                input_path = path;
                output_path = path;

                // we replace content by public since we're builing the website
                replaceString(output_path, "content", "public");
                replaceString(output_path, ".md", ".html");

                if (input_path != NULL && strcmp(input_path, "-") != 0) {
                    in = fopen(input_path, "rb");
                    if (in == NULL) {
                        fprintf(stderr, "Cannot open %s.\n", input_path);
                        exit(1);
                    }
                }

                if (output_path != NULL && strcmp(output_path, "-") != 0) {
                    out = fopen(output_path, "w");
                    if (out == NULL) {
                        fprintf(stderr, "Cannot open %s.\n", output_path);
                        exit(1);
                    }
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

    FILE* in = stdin;
    FILE* out = stdout;

    if (strcmp(argv[1], BUILD_ARG) == 0) {
        proceedFilesRecursively("./content", in, out);
    } else if (strcmp(argv[1], SERVE_ARG) == 0) {
        printf("NOT IMPLEMENTED YET !");
        return EXIT_SUCCESS;
    } else {
        printf("? Usage: ./builder build|serve");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
