#include "lib.h"

static unsigned parser_flags = 0;
static unsigned renderer_flags =
    MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;

EntryMap getMetadataForFilePath(const EntryMap* entryMap, const char* filePath,
                                int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(entryMap[i].entry.path, filePath) == 0) {
            return entryMap[i];
        }
    }
    return entryMap[0];
}

/**
 * Removes newline characters from a file and saves the modified content to the
 * same file. Input:
 * - filePath: A C-style string representing the path to the file.
 * - The file at filePath should exist and be readable.
 * - The file at filePath should have write permission.
 * - The path for the temporary file (filePath + ".temp") should have write
 * permission.
 */
void minifyFile(const char* filePath) {
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

/**
 * Recursively minifies files in a given directory.
 *
 * @param path - The path to the directory to be minified.
 *
 * The function opens the directory specified by 'path' and iterates through
 * each entry in the directory. If a subdirectory is found, it ignores the "."
 * and ".." directories and recursively calls the function with the path to the
 * subdirectory. If a file is found, it checks if the file has a ".js" or ".css"
 * extension and calls the 'minifyFile' function to minify the file.
 */
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

void writeMetadatasToBlogList(const char* input_filename,
                              const char* output_filename) {
    FILE* input_file = fopen(input_filename, "r");
    FILE* output_file = fopen(output_filename, "w");

    if (!input_file || !output_file) {
        perror("File opening failed");
        return;
    }

    Entry metadata;
    char line[1024];

    // Write the beginning of the HTML document
    fprintf(output_file, "<h1 class='typing'> BLOG POSTS</h1> <br/>");

    // FIXME:handle search later
    //fprintf(output_file, "<div class='search-container'> <div class='search-wrapper'> <input type='text' class='search-input' placeholder='search by title or content...' id='search-input' > </div> </div><br/>");

    fprintf(output_file, "<div class=\"blog-list\" style=\"--total-items: 0;\">");

    while (fgets(line, sizeof(line), input_file)) {
        if (sscanf(line, "path: %[^\n]", metadata.path) != 1) continue;
        fgets(line, sizeof(line), input_file);

        sscanf(line, "link: %[^\n]", metadata.link);
        fgets(line, sizeof(line), input_file);

        sscanf(line, "title: %[^\n]", metadata.title);
        fgets(line, sizeof(line), input_file);

        sscanf(line, "image: %[^\n]", metadata.image);
        fgets(line, sizeof(line), input_file);

        sscanf(line, "date: %[^\n]", metadata.date);
        fgets(line, sizeof(line), input_file);

        sscanf(line, "tags: %[^\n]", metadata.tags);
        fgets(line, sizeof(line), input_file);

        sscanf(line, "time: %[^\n]", metadata.time);

        // Write the HTML structure for this blog item
        fprintf(output_file, "<a href=\"%s\"><div class=\"blog-item\">\n", metadata.link);
        fprintf(output_file, "<div class=\"triangle-bg\" style=\"--bg-image: url('%s');\"></div>\n", metadata.image); //<div class=\"category-badge\">AI</div>
        fprintf(output_file, "    <h3><a href=\"%s\" class=\"blog-title\">%s</a></h3>\n", metadata.link, metadata.title);
        fprintf(output_file, "    <div class=\"blog-meta\">\n");
        fprintf(output_file, "        <span>%s</span>\n", metadata.date);
        // Assuming you want to keep tags in meta, or move them below
        fprintf(output_file, "        <span>%s</span>\n", metadata.time);
        fprintf(output_file, "    </div>\n");
        // Excerpt would go here if you have it
        fprintf(output_file, "    <p class=\"blog-excerpt\">\n");
        // TODO: add in metadatas, the section for a resume of the blog post
        // that should be in this p tag
        // so this need to be done by also updating the current struct
        fprintf(output_file, "    ");
        fprintf(output_file, "    </p>\n");
        fprintf(output_file, "    <div class=\"tags\">\n");
        // Process tags - assuming they are comma-separated
        char *token = strtok(metadata.tags, ",");
        while (token != NULL) {
            fprintf(output_file, "        <span class=\"tag\">%s</span>\n", token);
            token = strtok(NULL, ",");
        }
        fprintf(output_file, "    </div>\n");
        fprintf(output_file, "</div></a>\n");
    }
    fprintf(output_file, "</div>");
    // Write the end of the HTML document
    // fprintf(output_file, "</body>\n</html>");

    fclose(input_file);
    fclose(output_file);
}

void writeMetadatasToHeader(FILE* file, Entry* eM) {
    FILE* templateFile = fopen("./content/components/blog-header.md", "r");

    // Determine file size
    fseek(templateFile, 0, SEEK_END);
    long fileSize = ftell(templateFile);
    fseek(templateFile, 0, SEEK_SET);

    // Allocate memory for file content
    char* templateContent = malloc(fileSize + 1);
    if (templateContent == NULL) {
        perror("Failed to allocate memory");
        fclose(templateFile);
    }

    // Read file content
    fread(templateContent, 1, fileSize, templateFile);
    templateContent[fileSize] = '\0';  // Null-terminate the string
    fclose(templateFile);

    char contentOfFile[4096];  // Adjust the buffer size as needed

    // Replace placeholders in the template with actual content
    sprintf(contentOfFile, templateContent, eM->title, eM->link, eM->title,
            eM->title, eM->image, eM->image, eM->link, eM->title, eM->image);

    // Write the content to the file
    fputs(contentOfFile, file);
}

/**
 * This function processes a file by performing the following steps:
 * 1. Opens the file specified by the 'filename' parameter for reading.
 * 2. If the file opening fails, an error message is printed and the function
 * exits. Input: filename (const char*) - the name of the file to be processed
 *
 * 3. Reads the content of the file into a dynamically allocated buffer.
 *    Input: file (FILE*) - the file pointer to read from
 *    Output: content (char*) - a dynamically allocated buffer that contains the
 * file content
 *
 * 4. Modifies the file contents by performing the following operations:
 *    - Opens the same file for writing.
 *    - Opens the "header.md" file for reading, and appends its content to the
 * output file.
 *    - Appends the content of the input file to the output file.
 *    - Opens the "comment-footer.md" file for reading, and appends its content
 * to the output file (only if the input file is a Markdown file and located in
 * either the "blogs" or "projects" directory).
 *    - Opens the "footer.md" file for reading, and appends its content to the
 * output file. Input: outputFile (FILE*) - the file pointer to write to content
 * (char*) - the buffer containing the file content filename (const char*) - the
 * name of the file to be processed
 *
 * 5. Closes all opened files and frees the dynamically allocated buffer.
 */

void addHeaderFooterToFile(EntryMap* entryMap, const char* filename,
                           int count) {
    FILE* fileInReadMode = fopen(filename, "r");
    if (fileInReadMode == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read the content of the file
    fseek(fileInReadMode, 0, SEEK_END);
    long fileSize = ftell(fileInReadMode);
    rewind(fileInReadMode);
    char* contentOfFile = malloc(fileSize + 1);
    if (contentOfFile == NULL) {
        perror("Error allocating memory");
        fclose(fileInReadMode);
        exit(1);
    }
    fread(contentOfFile, 1, fileSize, fileInReadMode);
    contentOfFile[fileSize] = '\0';
    fclose(fileInReadMode);

    // Modify the file contents
    FILE* fileInWriteMode = fopen(filename, "w");
    if (fileInWriteMode == NULL) {
        perror("Error opening file for writing");
        free(contentOfFile);
        exit(1);
    }

    char line[1000];

    // We add comments-footer-component only if its from a blog-post file
    if (strstr(filename, ".md") != NULL &&
        (strstr(filename, "blogs/") != NULL ||
         strstr(filename, "projects/") != NULL)) {
        // get metadatas for the file
        EntryMap eM = getMetadataForFilePath(entryMap, filename, count);

        writeMetadatasToHeader(fileInWriteMode, &eM.entry);
        fputs(contentOfFile, fileInWriteMode);

        FILE* commentFooterFileInReadMode =
            fopen("./content/components/comment-footer.md", "r");
        if (commentFooterFileInReadMode == NULL) {
            perror("Error opening footer-comment file");
            free(contentOfFile);
            fclose(fileInWriteMode);
            exit(1);
        }

        while (fgets(line, sizeof(line), commentFooterFileInReadMode) != NULL) {
            fputs(line, fileInWriteMode);
        }

        fclose(commentFooterFileInReadMode);
    } else {
        FILE* headerFileInReadMode =
            fopen("./content/components/header.md", "r");
        if (headerFileInReadMode == NULL) {
            perror("Error opening header file");
            free(contentOfFile);
            fclose(fileInWriteMode);
            exit(1);
        }
        while (fgets(line, sizeof(line), headerFileInReadMode) != NULL) {
            fputs(line, fileInWriteMode);
        }
        fclose(headerFileInReadMode);
        fputs(contentOfFile, fileInWriteMode);
    }

    // we add search-script component to the blogs.md
    if (strstr(filename, "blogs.md") != NULL) {
        FILE* searchFileComponent =
            fopen("./content/components/search-script.md", "r");
        if (searchFileComponent == NULL) {
            perror("Error opening search-script file");
            free(contentOfFile);
            fclose(fileInWriteMode);
            exit(1);
        }

        while (fgets(line, sizeof(line), searchFileComponent) != NULL) {
            fputs(line, fileInWriteMode);
        }

        fclose(searchFileComponent);
    }

    // we write the footer component
    FILE* footerFileInReadMode = fopen("./content/components/footer.md", "r");
    if (footerFileInReadMode == NULL) {
        perror("Error opening footer file");
        free(contentOfFile);
        fclose(fileInWriteMode);
        exit(1);
    }
    while (fgets(line, sizeof(line), footerFileInReadMode) != NULL) {
        fputs(line, fileInWriteMode);
    }
    fclose(footerFileInReadMode);

    fclose(fileInWriteMode);
    free(contentOfFile);
}

void createStyleFileAndCopyFavicon() {
    // FIXME: change this to be C oriented code.
    // Yeah, i know, messy messy messy
    // I don't care, will change the loggic when i will be happy
    int status = system(
        "mkdir -p public public/blogs public/projects public/components && "
        "cp ./content/favicon.ico ./content/robots.txt "
        "./public/");
    if (status != 0) {
        perror("Error setting resources files (style, ico,...) ");
        exit(1);
    }
}

/*
 * This function takes in a directory path as input and processes all the
 * markdown files present in that directory.
 *
 * Input:
 * - directory: A string representing the directory path where the markdown
 * files are located.
 *
 * Output: None
 *
 * Steps:
 * 1. Open the directory specified by the input directory path.
 * 2. If the directory cannot be opened, print an error message and exit the
 * program.
 * 3. Iterate through each entry (file or subdirectory) in the directory.
 * 4. Check if the entry is a regular file and if its name contains ".md"
 * (indicating a markdown file).
 * 5. If the conditions in step 4 are satisfied, create the file path by
 * combining the directory path and the entry name.
 * 6. Call the addHeaderFooterToFile function, passing the file path as an
 * argument to process the file.
 * 7. Repeat steps 3-6 for all entries in the directory.
 * 8. Close the directory.
 *
 */
void buildComponentsIntoMarkdownsFiles(const char* directory) {
    // FIXME: this is just to set up all styles
    // should not be a part of this function btw
    createStyleFileAndCopyFavicon();

    // for metadatas
    // we add to the header of the file what we read from the metadata file
    EntryMap entryMap[MAX_ENTRIES];
    int count = 0;
    parse_txt("content/metadatas.txt", entryMap, &count);

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

            printf(">>>>>>>>>>>>>>>>>>>>> %s\n\n", filepath);
            addHeaderFooterToFile(entryMap, filepath, count);
        }
    }

    closedir(dir);
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

/**
 * This function replaces all occurrences of a substring in a string with
 * another substring.
 *
 * @param original - the original string to be modified
 * @param toReplace - the substring to be replaced
 * @param replacement - the substring to replace all occurrences of "toReplace"
 *
 * The function modifies the "original" string by replacing all occurrences of
 * "toReplace" with "replacement". It uses a temporary buffer to hold the
 * modified string, and then copies the modified string back to the "original"
 * string.
 */
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

/*
 * This function processes a file by reading its contents, parsing it into HTML
 * format, and writing the result to an output file.
 *
 * Input:
 * - in: A FILE pointer representing the input file.
 * - out: A FILE pointer representing the output file.
 * - page_title: A pointer to a char array representing the title of the page.
 *
 * Output:
 * - Returns an integer indicating the success of the function. Returns 0 if
 * successful, and a negative value otherwise.
 * - Writes the parsed HTML contents to the output file.
 *
 * Steps:
 * 1. Initialize memory buffers for input and output.
 * 2. Read the contents of the input file into the input buffer.
 * 3. Initialize the output buffer with an estimated size based on the input
 * size.
 * 4. Parse the input document into HTML format using the md_html function, with
 * callbacks provided via the md_renderer_t structure.
 * 5. Measure the time taken to parse the document.
 * 6. If parsing fails, print an error message and return.
 * 7. Write the parsed HTML contents to the output file.
 * 8. If time measurement was successful, print the time spent on parsing.
 * 9. Clean up and free the memory buffers.
 * 10. Return the result of the function.
 */
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

/**
 * Recursively proceeds through files in a directory and its subdirectories.
 *
 * @param basePath - the base path of the directory to start from
 *
 * Input:
 * - basePath: the base path of the directory to start from
 *
 * Output: None
 *
 * Steps:
 * 1. Set up variables for path, input_path, and output_path.
 * 2. Open the directory specified by basePath.
 * 3. Set up input and output file pointers as stdin and stdout.
 * 4. Create an array of string replacements for title formatting.
 * 5. If the directory cannot be opened, print an error message and return.
 * 6. While there are more files in the directory,
 *    a. If the file is not the current directory or the parent directory,
 *       - Build the complete path by concatenating basePath and the current
 * file name.
 *       - Check if the path corresponds to a directory.
 *       - If it is a directory, call the function recursively with the new
 * path.
 *       - If it is a file with a ".md" extension,
 *         - Copy the input path and output path to their respective variables.
 *         - Replace "content" with "public" and replace ".md" with ".html" in
 * the output path.
 *         - Open the input and output files.
 *         - For each string in the string_title_target_replacements array,
 * replace it with an empty string in the output path.
 *         - Process the file by calling the process_file function with the
 * input, output, and output path arguments.
 *         - Close the input and output files if they are not stdin and stdout.
 * 7. Close the directory.
 */
void proceedFilesRecursivelly(char* basePath) {
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

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            sprintf(path, "%s/%s", basePath, dp->d_name);
            if (stat(path, &filestat) == -1) {
                continue;
            }
            if (S_ISDIR(filestat.st_mode)) {
                proceedFilesRecursivelly(path);
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

void parse_txt(const char* filename, EntryMap entryMap[], int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    EntryMap currentEntry;
    int entryIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Check for the end of an entry
        if (line[0] == '\n') {
            /* Extract key from filename */
            char* dot = strrchr(currentEntry.key, '.');
            if (dot != NULL) {
                size_t keyLength = dot - currentEntry.key;
                currentEntry.key[keyLength] = '\0';
            }

            entryMap[entryIndex++] = currentEntry;
            // Reset currentEntry for the next iteration
            memset(&currentEntry, 0, sizeof(currentEntry));
        } else {
            // Parse each line of the entry
            char key[256], value[256];
            sscanf(line, "%s %255[^\n]", key, value);

            if (strcmp(key, "path:") == 0)
                strncpy(currentEntry.entry.path, value,
                        sizeof(currentEntry.entry.path));
            if (strcmp(key, "link:") == 0)
                strncpy(currentEntry.entry.link, value,
                        sizeof(currentEntry.entry.link));
            else if (strcmp(key, "title:") == 0)
                strncpy(currentEntry.entry.title, value,
                        sizeof(currentEntry.entry.title));
            else if (strcmp(key, "image:") == 0)
                strncpy(currentEntry.entry.image, value,
                        sizeof(currentEntry.entry.image));
            else if (strcmp(key, "date:") == 0)
                strncpy(currentEntry.entry.date, value,
                        sizeof(currentEntry.entry.date));

            // Save the key as well (filename)
            if (strcmp(key, "filename:") == 0)
                strncpy(currentEntry.key, value, sizeof(currentEntry.key));
        }
    }

    *count = entryIndex;

    fclose(file);
}
