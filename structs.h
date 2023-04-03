#include "lib.h"

#define RENDER 1

struct Component {
    u_int8_t component_id;  // this will never reach > 10 we should be fine
    char content[100];
    bool need_rendering;
};

struct Node {
    struct Component content;
    int child;  // this should be a pointer to the next component_id ( i don't
                // know yet how am going to mixed that up tho )
};

struct Layout {
    struct Node nodes[20];  // I fix myself no never reach this limit
};
