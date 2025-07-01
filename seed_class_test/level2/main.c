#include "terminal.h"
#include "editor.h"
#include <stdio.h> 

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }
    
    const char *filename = argc == 2 ? argv[1] : NULL;
    
    init_editor(filename);
    editor_loop();
    editor_cleanup();
    
    return 0;
}