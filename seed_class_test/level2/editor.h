#ifndef EDITOR_H
#define EDITOR_H

#include "buffer.h"

// 特殊键定义
#define KEY_UP    0x100
#define KEY_DOWN  0x101
#define KEY_RIGHT 0x102
#define KEY_LEFT  0x103
#define HOME_KEY  0x104
#define END_KEY   0x105
#define PAGE_UP   0x106
#define PAGE_DOWN 0x107

// 编辑器状态
typedef struct {
    TextBuffer buffer;
    char *filename;
    int cursor_row;
    int cursor_col;
    int row_offset;
    int col_offset;
    int screen_rows;
    int screen_cols;
} EditorState;

// 初始化编辑器
void init_editor(const char *filename);

// 主编辑器循环
void editor_loop(void);

// 清理编辑器资源
void editor_cleanup(void);

#endif