#include "editor.h"
#include "terminal.h"
#include "file_io.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 全局编辑器状态
EditorState E = {0};
void display_refresh(void);

// 自定义字符串复制函数
char *my_strdup(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy) memcpy(copy, s, len + 1);
    return copy;
}

// 创建空缓冲区
void create_empty_buffer(TextBuffer *buffer) {
    buffer->data = malloc(1);
    if (buffer->data) {
        buffer->data[0] = '\0';
        buffer->size = 0;
    }
}

void move_cursor(int key) {
    switch (key) {
        case KEY_UP:
            if (E.cursor_row > 0) E.cursor_row--;
            break;
        case KEY_DOWN:
            if (E.cursor_row < (int)E.buffer.lines - 1) E.cursor_row++;
            break;
        case KEY_LEFT:
            if (E.cursor_col > 0) E.cursor_col--;
            break;
        case KEY_RIGHT:
            E.cursor_col++;
            break;
    }
    
    // 滚动处理
    if (E.cursor_row < E.row_offset) {
        E.row_offset = E.cursor_row;
    }
    if (E.cursor_row >= E.row_offset + E.screen_rows) {
        E.row_offset = E.cursor_row - E.screen_rows + 1;
    }
    if (E.cursor_col < E.col_offset) {
        E.col_offset = E.cursor_col;
    }
    if (E.cursor_col >= E.col_offset + E.screen_cols) {
        E.col_offset = E.cursor_col - E.screen_cols + 1;
    }
}

void init_editor(const char *filename) {
    // 确保完全初始化
    memset(&E, 0, sizeof(E));
    
    // 复制文件名
    if (filename) {
        E.filename = my_strdup(filename);
    } else {
        E.filename = NULL;
    }
    
    // 初始化缓冲区
    init_buffer(&E.buffer);
    
    // 获取终端尺寸
    get_terminal_size(&E.screen_rows, &E.screen_cols);
    display_init(E.screen_rows, E.screen_cols);
    
    // 启用原始模式
    enable_raw_mode();
    
    // 加载文件
    if (filename) {
        size_t size = 0;
        int status = load_file(filename, &E.buffer.data, &size);
        E.buffer.size = size;
        
        if (status != FILE_OK) {
            // 创建空缓冲区
            create_empty_buffer(&E.buffer);
        }
    } else {
        // 创建空缓冲区
        create_empty_buffer(&E.buffer);
    }
    
    // 计算行数
    count_lines(&E.buffer);
    
    // 初始化光标位置
    E.cursor_row = 0;
    E.cursor_col = 0;
    E.row_offset = 0;
    E.col_offset = 0;
    
    // 初始绘制
    display_refresh();
}

void display_refresh(void) {

    // 确保显示模块已正确初始化
    if (E.screen_rows == 0 || E.screen_cols == 0) {
        get_terminal_size(&E.screen_rows, &E.screen_cols);
        display_init(E.screen_rows, E.screen_cols);
    }
    
    display_clear();
    
    // 显示文件内容（根据当前滚动偏移）
    display_file(&E.buffer, E.row_offset, E.col_offset);
    
    // 显示状态栏（包含文件名、光标位置和行数信息）
    display_status_bar(E.filename, E.cursor_row, E.cursor_col, E.buffer.lines);
    
    // 计算光标在屏幕上的可见位置
    int visible_row = E.cursor_row - E.row_offset;
    int visible_col = E.cursor_col - E.col_offset;
    
    // 边界检查 - 确保光标位置在有效范围内
    if (visible_row < 0) visible_row = 0;
    if (visible_col < 0) visible_col = 0;
    if (visible_row >= E.screen_rows) visible_row = E.screen_rows - 1;
    if (visible_col >= E.screen_cols) visible_col = E.screen_cols - 1;
    
    // 设置光标在屏幕上的实际位置
    set_cursor_position(visible_row, visible_col);
    
    // 刷新输出缓冲区，确保所有内容显示
    fflush(stdout);
}

void editor_loop() {
    while (1) {
        int c = read_key();
        
        switch (c) {
            case 'q':
                return;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                move_cursor(c);
                display_refresh();
                break;
            case HOME_KEY: // 转到文件开头
                E.cursor_row = 0;
                E.cursor_col = 0;
                E.row_offset = 0;
                E.col_offset = 0;
                display_refresh();
                break;
            case END_KEY: // 转到文件末尾
                E.cursor_row = E.buffer.lines - 1;
                E.cursor_col = 0;
                if (E.cursor_row >= E.screen_rows) {
                    E.row_offset = E.buffer.lines - E.screen_rows;
                }
                E.col_offset = 0;
                display_refresh();
                break;
            case PAGE_UP: //pg up(4行)
                if(!E.cursor_row-4){
                    E.cursor_row -= 4;
                }
                else E.cursor_row = 0;
                display_refresh();
                break;
            case PAGE_DOWN: //pg dn(4行)
                if(E.cursor_row + 4 < E.buffer.lines - 1){
                    E.cursor_row += 4;
                }
                else E.cursor_row = E.buffer.lines - 1;
                display_refresh();
                break;
        }
    }
}

void editor_cleanup() {
    free_buffer(&E.buffer);
    if (E.filename) free(E.filename);
    disable_raw_mode();
}