#ifndef DISPLAY_H
#define DISPLAY_H

#include "buffer.h"

// 显示操作接口
void display_init(int screen_rows, int screen_cols);
void display_clear(void);
void display_file(TextBuffer *buffer, int row_offset, int col_offset);
void display_status_bar(const char *filename, int row, int col, int total_rows); // 修复为 int
void set_cursor_position(int row, int col);

#endif