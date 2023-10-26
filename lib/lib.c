#include "lib.h"

static unsigned parser_flags = 0;
static unsigned renderer_flags =
    MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;

void minifyFile(const char* filePath) {
    // Open the source file for reading
    FILE* sourceFile = fopen(filePath, "r");
    if (sourceFile == NULL) {
        perror("Error opening file");
        return;
    }

    // Create a temporary file for writing
    char tempFilePath[FILENAME_MAX];
    strcpy(tempFilePath, filePath);
    strcat(tempFilePath, ".temp");
    FILE* tempFile = fopen(tempFilePath, "w");
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        fclose(sourceFile);
        return;
    }

    // Process the source file line by line
    char line[1024];
    while (fgets(line, sizeof(line), sourceFile) != NULL) {
        size_t length = strlen(line);
        // Remove newline characters
        while (length > 0 &&
               (line[length - 1] == '\n' || line[length - 1] == '\r')) {
            line[length - 1] = '\0';
            length--;
        }
        // Write the line to the temporary file
        fputs(line, tempFile);
    }

    // Close the files
    fclose(sourceFile);
    fclose(tempFile);

    // Remove the original file
    if (remove(filePath) != 0) {
        perror("Error removing source file");
        return;
    }

    // Rename the temporary file to the original file name
    if (rename(tempFilePath, filePath) != 0) {
        perror("Error renaming temporary file");
        return;
    }
}

void minifyDirfiles(const char* path) {
    DIR* directory;
    struct dirent* entry;

    directory = opendir(path);
    if (directory == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_DIR) {
            // Ignore "." and ".." directories
            if (strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0) {
                char filePath[300];
                snprintf(filePath, sizeof(filePath), "%s/%s", path,
                         entry->d_name);
                minifyDirfiles(filePath);
            }
        } else {
            // Check if the file is HTML or CSS
            char* extension = strrchr(entry->d_name, '.');
            // only js and css files for now
            if (extension != NULL && (strcmp(extension, ".js") == 0 ||
                                      strcmp(extension, ".css") == 0)) {
                char filePath[300];
                snprintf(filePath, sizeof(filePath), "%s/%s", path,
                         entry->d_name);
                minifyFile(filePath);
            }
        }
    }

    closedir(directory);
}

void processFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read the content of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* content = malloc(file_size + 1);
    if (content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        exit(1);
    }
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);

    // Modify the file contents
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Error opening file for writing");
        free(content);
        exit(1);
    }

    char line[1000];
    FILE* headerFile = fopen("./content/components/header.md", "r");
    if (headerFile == NULL) {
        perror("Error opening header file");
        free(content);
        fclose(outputFile);
        exit(1);
    }
    while (fgets(line, sizeof(line), headerFile) != NULL) {
        fputs(line, outputFile);
    }
    fclose(headerFile);

    fputs(content, outputFile);

    FILE* footerFile = fopen("./content/components/footer.md", "r");
    if (footerFile == NULL) {
        perror("Error opening footer file");
        free(content);
        fclose(outputFile);
        exit(1);
    }
    while (fgets(line, sizeof(line), footerFile) != NULL) {
        fputs(line, outputFile);
    }
    fclose(footerFile);

    FILE* commentFile = fopen("./content/components/comment-footer.md", "r");
    if (commentFile == NULL) {
        perror("Error opening footer-comment file");
        free(content);
        fclose(outputFile);
        exit(1);
    }

    if (strstr(filename, ".md") != NULL &&
        (strstr(filename, "blogs/") != NULL ||
         strstr(filename, "projects/") != NULL)) {
        while (fgets(line, sizeof(line), commentFile) != NULL) {
            fputs(line, outputFile);
        }
    }

    fclose(commentFile);
    fclose(outputFile);

    free(content);
}

void processDirectoryMarkdowns(const char* directory) {
    DIR* dir;
    struct dirent* entry;

    dir = opendir(directory);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".md") != NULL) {
            char filepath[300];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory,
                     entry->d_name);
            processFile(filepath);
        }
    }

    closedir(dir);
}

void processMarkdownFiles() {
    processDirectoryMarkdowns("./content");
    processDirectoryMarkdowns("./content/blogs");
}

void createStyleFileAndCopyFavicon() {
    int status = system(
        "mkdir -p public public/blogs public/projects public/components && cp "
        "./content/style.css ./public/style.css && cp "
        "./content/favicon.ico "
        "./public/favicon.ico");
    if (status != 0) {
        perror("Error creating style.css file or copying favicon.ico");
        exit(1);
    }
}

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
    fwrite(buf_out.data, 1, buf_out.size, out);

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

char* build_previews(const char* dir_name) {
    DIR* dir;
    struct dirent* entry;
    char* preview;
    int preview_size = 0;
    int character_size = 300;

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        return NULL;
    }

    // Calculate the total size of the previews
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_name,
                     entry->d_name);
            FILE* fp = fopen(file_path, "r");
            if (fp == NULL) {
                perror("fopen");
                closedir(dir);
                return NULL;
            }
            fseek(fp, 0, SEEK_END);
            int file_size = ftell(fp);
            fclose(fp);

            int preview_len =
                (file_size > character_size) ? character_size : file_size;
            preview_size += preview_len;
        }
    }

    // Allocate memory for the previews
    preview = (char*)malloc((preview_size + 1) * sizeof(char));
    if (preview == NULL) {
        perror("malloc");
        closedir(dir);
        return NULL;
    }
    preview[0] = '\0';  // initialize the string to the empty string

    // Concatenate the first 1000 characters of each file into the previews
    rewinddir(dir);  // reset directory stream
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_name,
                     entry->d_name);
            FILE* fp = fopen(file_path, "r");
            if (fp == NULL) {
                perror("fopen");
                closedir(dir);
                free(preview);
                return NULL;
            }
            char buffer[character_size + 1];
            int nread = fread(buffer, sizeof(char), character_size, fp);
            if (nread > 0) {
                buffer[nread] = '\0';
                strcat(preview, buffer);
            }
            fclose(fp);
        }
    }

    closedir(dir);

    return preview;
}

void proceed_files_recursivelly(char* basePath) {
    char path[1000];
    char input_path[1000];
    char output_path[1000];

    struct dirent* dp;
    struct stat filestat;
    DIR* dir = opendir(basePath);

    FILE* in = stdin;
    FILE* out = stdout;

    char* string_title_target_replacements[5] = {
        "public/", ".html", "/blogs/", "/projects/", ".",
    };

    if (!dir) {
        printf("Unable to open directory: %s\n", basePath);
        return;
    }

    /* build_blog_page(); */
    /* char* recent_projects = preview_recent_projects(); */

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            sprintf(path, "%s/%s", basePath, dp->d_name);
            if (stat(path, &filestat) == -1) {
                continue;
            }
            if (S_ISDIR(filestat.st_mode)) {
                proceed_files_recursivelly(path);
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

                for (int i = 0; i < 5; i++) {
                    replaceString(output_path,
                                  string_title_target_replacements[i], "");
                }
                // the title will be the file name without some stuffs in it
                process_file(in, out, output_path);
                if (in != stdin) fclose(in);
                if (out != stdout) fclose(out);
            }
        }
    }

    closedir(dir);
}
