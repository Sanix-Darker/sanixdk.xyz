Below is a *conceptual* guide and **minimal** sample in C showing how you might create a very basic web browser called **xs**, capable of:

- Fetching a URL (HTTP GET)
- Parsing *very* basic HTML structure
- Applying minimal (wireframe) CSS rules
- Handling extremely simple JavaScript with an embedded engine
- Rendering the result in a simple window

This is *not* a production-ready browser. Instead, it’s a skeleton or teaching example to illustrate how one might wire together libraries like **libcurl**, **Gumbo** (for HTML parsing), **Duktape** (for basic JS), and **SDL2** (for drawing). This code omits hundreds of edge cases and is meant purely for demonstration.

---

## Table of Contents

1. [Overview of the Architecture](#overview-of-the-architecture)
2. [Dependencies](#dependencies)
3. [Directory Structure](#directory-structure)
4. [Step-by-Step Walkthrough](#step-by-step-walkthrough)
5. [Sample Code](#sample-code)
   - [CMakeLists.txt](#cmakelists.txt)
   - [main.c](#main.c)
   - [network.c](#network.c)
   - [parser.c](#parser.c)
   - [layout.c](#layout.c)
   - [render.c](#render.c)
   - [javascript.c](#javascript.c)
6. [Build and Run](#build-and-run)
7. [Notes on Extension and Efficiency](#notes-on-extension-and-efficiency)
8. [References](#references)

---

## 1. Overview of the Architecture

A simplified view of how my **xs** operates:

```
┌────────────┐
│   main.c    │  <-- Entry point
└────────────┘
       │
       │
       ▼
┌─────────────────┐
│   network.c      │  <-- Responsible for fetching HTML over HTTP
└─────────────────┘
       │
       ▼
┌─────────────────┐
│   parser.c       │  <-- Uses Gumbo to parse HTML into a DOM-like structure
└─────────────────┘
       │
       ▼
┌─────────────────┐
│ javascript.c     │  <-- Uses Duktape to run <script> tags (extremely simple)
└─────────────────┘
       │
       ▼
┌─────────────────┐
│   layout.c       │  <-- Applies minimal "wireframe" CSS rules
└─────────────────┘
       │
       ▼
┌─────────────────┐
│   render.c       │  <-- Uses SDL2 to draw basic boxes, text, etc.
└─────────────────┘
```

- **network.c**: uses [libcurl](https://curl.se/libcurl/) to perform an HTTP GET request and retrieve HTML content.
- **parser.c**: uses [Gumbo parser](https://github.com/google/gumbo-parser) to parse HTML into a parse tree (DOM).
- **javascript.c**: uses [Duktape](https://duktape.org/) to run extremely simple JavaScript code found in `<script>` tags.
- **layout.c**: demonstrates a trivial approach to layout—only rendering block-level elements in a vertical flow.
- **render.c**: draws the layout boxes (wireframes) in a simple SDL2-based window.
- **main.c**: orchestrates everything—fetch, parse, layout, render.

---

## 2. Dependencies

1. **libcurl** – for HTTP requests
   - Install on Linux (Ubuntu/Debian):
     ```bash
     sudo apt-get update
     sudo apt-get install libcurl4-openssl-dev
     ```
2. **Gumbo parser** – for HTML parsing
   - [Gumbo GitHub repo](https://github.com/google/gumbo-parser)
   - Usually you can build from source, or use your distribution’s package if it’s available.
3. **Duktape** – for a simple embedded JavaScript engine
   - [Duktape.org](https://duktape.org/)
   - Download and build or place the single-include `.c` and `.h` in your project.

        # sudo apt update
        # sudo apt install build-essential
        # git clone https://github.com/svaarala/duktape.git
        #     > you will need python2.7 and pip2.7
        #         curl https://bootstrap.pypa.io/pip/2.7/get-pip.py -o get-pip.py
        #         python get-pip.py
        # cd duktape
        # make
        # sudo make install
        # duk -h

    sudo apt install duktape -y

4. **SDL2** – for basic 2D rendering
   - On Linux:
     ```bash
     sudo apt-get install libsdl2-dev
     ```
5. A C compiler (GCC, Clang, MSVC).

---

## 3. Directory Structure

```
xs/
 ├─ CMakeLists.txt
 ├─ main.c
 ├─ network.c
 ├─ network.h
 ├─ parser.c
 ├─ parser.h
 ├─ layout.c
 ├─ layout.h
 ├─ render.c
 ├─ render.h
 ├─ javascript.c
 ├─ javascript.h
 ├─ third_party/
 │   ├─ gumbo/     (Gumbo parser sources and includes)
 │   └─ duktape/   (Duktape sources and includes)
 └─ ...
```

---

## 4. Step-by-Step Walkthrough

1. **Fetch the HTML**
   - Use `libcurl` to GET a URL. Store the result in a memory buffer.

2. **Parse the HTML**
   - Feed the HTML string into Gumbo to parse.
   - Obtain a root `GumboNode`.
   - Recursively build an internal data structure (a “DOM”).

3. **Extract and Run JavaScript**
   - Traverse DOM, find `<script>` elements.
   - Use `Duktape` to run the script. (This example will just run them in a global context; no DOM integration.)

4. **Apply Minimal CSS**
   - For demonstration, we’ll just treat everything as a box and do a single vertical flow.
   - The “layout” step will produce rectangles for each block element (e.g., `<div>`, `<p>`).

5. **Render (SDL2)**
   - Create an SDL window.
   - Render rectangles (wireframes), and possibly text if desired, to represent each DOM block.

6. **Wait for exit**
   - Keep the SDL window open until the user closes it.
   - Clean up memory, SDL, and libraries.

---

## 5. Sample Code

Below is a simplified set of files. In reality, you’ll need to handle error checks, memory management, partial or broken HTML, etc. But this should illustrate the main approach.

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(xs LANGUAGES C)

# If you have gumbo in a standard path or installed, this might be simpler
# For demonstration, let's assume you have gumbo and duktape locally.

# Find external libraries
find_package(CURL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
    ${CURL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    third_party/gumbo/include
    third_party/duktape
)

add_executable(xs
    main.c
    network.c
    parser.c
    layout.c
    render.c
    javascript.c
    # If gumbo or duktape are built as separate libs, you can link them.
    # Otherwise, add .c from gumbo/duktape here.
)

target_link_libraries(xs
    ${CURL_LIBRARIES}
    ${SDL2_LIBRARIES}
    # If gumbo, duktape are separate shared libs, link them here
    # e.g. gumbo duktape
)
```

### main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "parser.h"
#include "javascript.h"
#include "layout.h"
#include "render.h"

// Our browser name
#define BROWSER_NAME "xs"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <url>\n", BROWSER_NAME);
        return EXIT_FAILURE;
    }

    const char* url = argv[1];
    printf("Fetching URL: %s\n", url);

    // 1. Fetch HTML
    char* html = fetch_url(url);
    if (!html) {
        fprintf(stderr, "Failed to fetch HTML from %s\n", url);
        return EXIT_FAILURE;
    }

    // 2. Parse HTML -> build DOM
    DOMNode* dom = parse_html(html);
    if (!dom) {
        fprintf(stderr, "Failed to parse HTML\n");
        free(html);
        return EXIT_FAILURE;
    }

    // 3. Execute any <script> tags (extremely simplified)
    run_scripts_in_dom(dom);

    // 4. Layout (compute minimal geometry)
    Layout* layout = layout_dom(dom);

    // 5. Render (SDL2)
    render_layout(layout);

    // Cleanup
    free_layout(layout);
    free_dom(dom);
    free(html);

    return EXIT_SUCCESS;
}
```

### network.c

```c
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct Memory {
    char* data;
    size_t size;
};

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    struct Memory* mem = (struct Memory*)userp;

    char* ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) {
        // out of memory
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = 0;

    return total_size;
}

char* fetch_url(const char* url) {
    CURL* curl_handle;
    CURLcode res;

    struct Memory chunk;
    chunk.data = malloc(1);  // will grow as needed
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle = curl_easy_init();
    if (!curl_handle) {
        curl_global_cleanup();
        return NULL;
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "xs/0.1");

    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(chunk.data);
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        return NULL;
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return chunk.data; // caller must free
}
```

### network.h

```c
#ifndef NETWORK_H
#define NETWORK_H

char* fetch_url(const char* url);

#endif
```

### parser.c

```c
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "third_party/gumbo/include/gumbo.h"

/*
    Minimal DOM structure:
    We define a DOMNode that has:
      - name (e.g. "html", "body", "p", "div", "#text")
      - textContent
      - children
      - next sibling? (we can store children in a dynamic array or linked list)
*/
DOMNode* create_dom_node(const char* name, const char* text) {
    DOMNode* node = (DOMNode*)malloc(sizeof(DOMNode));
    node->name = name ? strdup(name) : NULL;
    node->text = text ? strdup(text) : NULL;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

static void parse_gumbo_node(GumboNode* gumbo_node, DOMNode* parent) {
    if (gumbo_node->type == GUMBO_NODE_ELEMENT) {
        GumboElement* element = &gumbo_node->v.element;
        // Get tag name
        const char* tag_name = gumbo_normalized_tagname(element->tag);
        if (strcmp(tag_name, "UNKNOWN") == 0) {
            // Some custom or unknown tag, fallback
            tag_name = element->original_tag.data;
        }

        DOMNode* node = create_dom_node(tag_name, NULL);

        // attach to parent
        if (parent) {
            parent->children = (DOMNode**)realloc(parent->children,
                sizeof(DOMNode*) * (parent->children_count + 1));
            parent->children[parent->children_count] = node;
            parent->children_count++;
        }

        // Recurse for children
        GumboVector* children = &element->children;
        for (unsigned int i = 0; i < children->length; i++) {
            parse_gumbo_node(children->data[i], node);
        }

    } else if (gumbo_node->type == GUMBO_NODE_TEXT) {
        const char* text = gumbo_node->v.text.text;
        // create a text node
        if (parent) {
            DOMNode* node = create_dom_node("#text", text);
            parent->children = (DOMNode**)realloc(parent->children,
                sizeof(DOMNode*) * (parent->children_count + 1));
            parent->children[parent->children_count] = node;
            parent->children_count++;
        }
    }
    // we ignore comments, CDATA, etc. in this minimal example
}

DOMNode* parse_html(const char* html) {
    GumboOutput* output = gumbo_parse(html);
    if (!output) {
        return NULL;
    }
    // create a fake root
    DOMNode* root = create_dom_node("root", NULL);
    // parse recursively
    parse_gumbo_node(output->root, root);

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return root;
}

// Example free function
void free_dom(DOMNode* node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->text) free(node->text);

    for (int i = 0; i < node->children_count; i++) {
        free_dom(node->children[i]);
    }
    free(node->children);
    free(node);
}
```

### parser.h

```c
#ifndef PARSER_H
#define PARSER_H

typedef struct DOMNode {
    char* name;   // e.g. "div", "p", "#text"
    char* text;   // for text nodes
    struct DOMNode** children;
    int children_count;
} DOMNode;

DOMNode* parse_html(const char* html);
void free_dom(DOMNode* node);

#endif
```

### javascript.c

```c
#include "javascript.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Include duktape single-include
#include "third_party/duktape/duktape.h"

/*
  Very naive approach:
  - Recursively find all <script> nodes in the DOM
  - Evaluate their text content
  - No DOM integration (document, etc.), purely demonstrating script evaluation
*/

static void run_script(const char* script) {
    duk_context* ctx = duk_create_heap_default();
    if (!ctx) {
        fprintf(stderr, "Failed to create a Duktape heap.\n");
        return;
    }

    // Evaluate the JS
    if (duk_peval_string(ctx, script) != 0) {
        fprintf(stderr, "Script error: %s\n", duk_safe_to_string(ctx, -1));
    }

    duk_destroy_heap(ctx);
}

static void traverse_and_run_scripts(DOMNode* node) {
    if (!node) return;

    // Check if this is a <script>
    if (node->name && strcasecmp(node->name, "script") == 0) {
        // node->text is the JS content
        if (node->text) {
            run_script(node->text);
        }
    }

    // Recurse
    for (int i = 0; i < node->children_count; i++) {
        traverse_and_run_scripts(node->children[i]);
    }
}

void run_scripts_in_dom(DOMNode* root) {
    traverse_and_run_scripts(root);
}
```

### javascript.h

```c
#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H

#include "parser.h"

void run_scripts_in_dom(DOMNode* root);

#endif
```

### layout.c

```c
#include "layout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  We'll define a trivial Layout object to hold rectangles for each
  block-level element. We'll treat <div>, <p>, <h1>, etc. as "block"
  and <span>, <a>, etc. as "inline" (ignored or combined for simplicity).
*/

static int g_offset_y = 0; // global offset for stacked layout

static void layout_node(DOMNode* node, Layout* layout) {
    if (!node || !node->name) return;

    // We'll treat everything that's not "#text" as a block, ignoring advanced logic
    if (strcmp(node->name, "#text") != 0) {
        // Create a LayoutBox
        LayoutBox box;
        box.x = 10;
        box.y = g_offset_y;
        box.width = 600;  // fixed width for example
        box.height = 30;  // fixed height for demonstration
        box.node = node;

        // Add it to layout
        layout->boxes = (LayoutBox*)realloc(layout->boxes,
            sizeof(LayoutBox) * (layout->count + 1));
        layout->boxes[layout->count] = box;
        layout->count++;

        g_offset_y += 40; // increase offset for next block
    }

    // Recurse
    for (int i = 0; i < node->children_count; i++) {
        layout_node(node->children[i], layout);
    }
}

Layout* layout_dom(DOMNode* root) {
    Layout* layout = (Layout*)malloc(sizeof(Layout));
    layout->boxes = NULL;
    layout->count = 0;

    g_offset_y = 10; // start from 10px from top
    layout_node(root, layout);

    return layout;
}

void free_layout(Layout* layout) {
    if (!layout) return;
    free(layout->boxes);
    free(layout);
}
```

### layout.h

```c
#ifndef LAYOUT_H
#define LAYOUT_H

#include "parser.h"

typedef struct {
    int x, y, width, height;
    DOMNode* node;
} LayoutBox;

typedef struct {
    LayoutBox* boxes;
    int count;
} Layout;

Layout* layout_dom(DOMNode* root);
void free_layout(Layout* layout);

#endif
```

### render.c

```c
#include "render.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

void render_layout(Layout* layout) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("xs Browser",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // main loop
    bool running = true;
    SDL_Event e;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw each box in layout
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < layout->count; i++) {
            LayoutBox* box = &layout->boxes[i];
            SDL_Rect rect = { box->x, box->y, box->width, box->height };
            SDL_RenderDrawRect(renderer, &rect);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
```

### render.h

```c
#ifndef RENDER_H
#define RENDER_H

#include "layout.h"

// Creates an SDL window, draws layout boxes, and runs event loop
void render_layout(Layout* layout);

#endif
```

---

## 6. Build and Run

### 6.1 Build with CMake

1. Make sure you have `cmake`, `build-essential`, and the libraries installed.
2. From the root directory (where `CMakeLists.txt` is):
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
   This should produce an executable `xs` in the `build` directory.

### 6.2 Run

```bash
./xs https://example.com
```

- A window should pop up with black wireframe rectangles stacked vertically (one for each block-level node).
- Any `<script>` contents will be passed to Duktape, though we do not expose any DOM or browser APIs to JS in this minimal example.

---

## 7. Notes on Extension and Efficiency

- **Performance & Efficiency**:
  - A real browser does many optimizations. This code does a naive layout. You can optimize by caching resources, using more efficient data structures, etc.
  - For extremely efficient rendering, consider GPU acceleration or more advanced frameworks.
- **HTML Parsing**: Gumbo is fairly complete for HTML5, but you must handle advanced constructs and invalid markup.
- **CSS**: We only demonstrated a simplistic wireframe approach. A real CSS engine is complex—handling selectors, inheritance, cascade, box model, floats, flexbox, etc.
- **JavaScript**: We only run a raw script. Real browsers have a BOM (Browser Object Model), a DOM interface, event loops, etc. Implementing these is a large task.
- **Security**: Sandboxing, same-origin policy, cookie handling, etc. are not addressed.
- **Memory Management**: This sample code is missing some robust error handling and memory cleanup.

---

## 8. References

1. **libcurl**
   - [Documentation](https://curl.se/libcurl/)
2. **Gumbo Parser**
   - [GitHub Repo](https://github.com/google/gumbo-parser)
3. **Duktape**
   - [Official Site](https://duktape.org/)
4. **SDL2**
   - [SDL Homepage](https://www.libsdl.org/)
5. **Building a Browser from Scratch**
   - [Web Browser 101 Resources (various articles/tutorials)](https://limpet.net/mbrubeck/2014/08/08/toy-layout-engine-1.html)
   - [SerenityOS’s “Ladybird” browser project (C++), for a large real example](https://github.com/SerenityOS/serenity/tree/master/Ladybird)

---

### Final Thoughts

This toy browser, **xs**, is merely a starting template. To turn it into a more fully featured browser would require significant additional work—especially around JavaScript bindings to the DOM and implementing the complexities of real CSS layout. However, this minimal codebase should help you understand the high-level architecture: **fetch → parse → layout → render** (with optional script execution). Feel free to adapt, extend, and optimize it for your educational or experimental goals. Good luck building **xs**!
