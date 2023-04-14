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

// the builder of the top level of the webpage with necessary stuffs
void build_header_web_page(FILE* out, char* page_title);
