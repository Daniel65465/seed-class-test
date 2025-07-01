#include "terminal.h"
#include "editor.h"

int main() {
    init_editor();
    editor_loop();
    return 0;
}