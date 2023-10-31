#include "lib.h"

static unsigned parser_flags = 0;
static unsigned renderer_flags =
    MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;

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

    FILE* commentFile = fopen("./content/components/comment-footer.md", "r");
    if (commentFile == NULL) {
        perror("Error opening footer-comment file");
        free(content);
        fclose(outputFile);
        exit(1);
    }
    // we add comments as footer only if its from a blog-post
    if (strstr(filename, ".md") != NULL &&
        (strstr(filename, "blogs/") != NULL ||
         strstr(filename, "projects/") != NULL)) {
        while (fgets(line, sizeof(line), commentFile) != NULL) {
            fputs(line, outputFile);
        }
    }
    fclose(commentFile);

    // we write the footer component
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

    fclose(outputFile);
    free(content);
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
 * 6. Call the processFile function, passing the file path as an argument to
 * process the file.
 * 7. Repeat steps 3-6 for all entries in the directory.
 * 8. Close the directory.
 *
 */
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

void createStyleFileAndCopyFavicon() {
    // FIXME: change this to be C oriented code.
    // Yeah, i know, messy messy messy
    // I don't care, will change the loggic when i will be happy
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
 * Builds previews for files in a given directory.
 *
 * @param dir_name The directory name where the files are located.
 * @return A pointer to a char array containing the previews of the files, or
 * NULL if an error occurs.
 *

 * Open the given directory
 * If the directory cannot be opened, return NULL
 * Calculate the total size of the previews by iterating through the files in
 * the directory Allocate memory for the previews If memory allocation fails,
 * return NULL Concatenate the first 300 characters of each file into the
 * previews Close the directory Return the previews as a char array
 *
*/
// char* buildPreviews(const char* dir_name) { */
//     DIR* dir; */
//     struct dirent* entry; */
//     char* preview; */
//     int preview_size = 0; */
//     int character_size = 300; */

//     dir = opendir(dir_name); */
//     if (dir == NULL) { */
//         perror("opendir"); */
//         return NULL; */
//     } */

//     // Calculate the total size of the previews */
//     while ((entry = readdir(dir)) != NULL) { */
//         if (entry->d_type == DT_REG) { */
//             char file_path[1024]; */
//             snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, */
//                      entry->d_name); */
//             FILE* fp = fopen(file_path, "r"); */
//             if (fp == NULL) { */
//                 perror("fopen"); */
//                 closedir(dir); */
//                 return NULL; */
//             } */
//             fseek(fp, 0, SEEK_END); */
//             int file_size = ftell(fp); */
//             fclose(fp); */

//             int preview_len = */
//                 (file_size > character_size) ? character_size : file_size; */
//             preview_size += preview_len; */
//         } */
//     } */

//     // Allocate memory for the previews */
//     preview = (char*)malloc((preview_size + 1) * sizeof(char)); */
//     if (preview == NULL) { */
//         perror("malloc"); */
//         closedir(dir); */
//         return NULL; */
//     } */
//     preview[0] = '\0';  // initialize the string to the empty string */

//     // Concatenate the first 1000 characters of each file into the previews
//
//     rewinddir(dir);  // reset directory stream */
//     while ((entry = readdir(dir)) != NULL) { */
//         if (entry->d_type == DT_REG) { */
//             char file_path[1024]; */
//             snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, */
//                      entry->d_name); */
//             FILE* fp = fopen(file_path, "r"); */
//             if (fp == NULL) { */
//                 perror("fopen"); */
//                 closedir(dir); */
//                 free(preview); */
//                 return NULL; */
//             } */
//             char buffer[character_size + 1]; */
//             int nread = fread(buffer, sizeof(char), character_size, fp); */
//             if (nread > 0) { */
//                 buffer[nread] = '\0'; */
//                 strcat(preview, buffer); */
//             } */
//             fclose(fp); */
//         } */
//     } */

//     closedir(dir); */

//     return preview; */
// } */

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

    /* build_blog_page(); */
    /* char* recent_projects = preview_recent_projects(); */

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

/**
 * This function takes a template string, an array of KeyValue pairs, and the
 * number of parameters, and applies the parameters to the template string by
 * replacing occurrences of {{key}} with the corresponding value. The resulting
 * string is returned.
 *
 * Example :
 *
 *  const char* template_str = "Hello, {{ name }}! Today is {{ day }}.";
 *  KeyValue params[] = {{"name", "John"}, {"day", "Monday"}};
 *
 *  const char* result = apply_template(template_str, params);
 *  printf("%s\n", result);
 *  free(result); // because c (fml)
 */
const char* apply_template(const char* template_str, KeyValue* params) {
    // how to fix this, just give me the code :
    int params_count = (int)sizeof(params) / (int)sizeof(params[0]);
    char* result = malloc(strlen(template_str) * sizeof(char));
    strcpy(result, template_str);

    for (int i = 0; i < params_count; i++) {
        char* open_acolads = "{{";
        char* close_acolads = "}}";

        char* token = strchr(result, *open_acolads);
        if (token == NULL) break;

        char* end = strchr(token + 2, *close_acolads);
        if (end == NULL) break;

        int token_length = end - token + 2;
        char* key = malloc((token_length - 4) * sizeof(char));
        strncpy(key, token + 2, token_length - 4);
        key[token_length - 4] = '\0';

        for (int j = 0; j < params_count; j++) {
            if (strcmp(params[j].key, key) == 0) {
                strcat(result, params[j].value);
                break;
            }
        }

        char* remaining = end + 2;
        strcat(result, remaining);
        free(key);
    }

    return result;
}
