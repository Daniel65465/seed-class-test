#include "editor.h"
#include "terminal.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void editor_draw() {

    write(STDOUT_FILENO, "Level 1(Press 'q' to quit):", 28);
    
    write(STDOUT_FILENO, "\r\n", 2);
    write(STDOUT_FILENO, "> ", 2);
}

void init_editor() {
    enable_raw_mode();
    editor_draw(); // 初始绘制界面
}

void editor_loop() {
    while (1) {
        char c = read_key();
        
        if (c == 'q') {
            write(STDOUT_FILENO, "\r\n", 2);
            break;
        }
        
        char s[2] = {c, '\0'};
        write(STDOUT_FILENO, s, 1);
    }
}