#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int screen_rows;
static int screen_cols;

void display_init(int rows, int cols) {
    // 减去状态栏占用的两行（一行状态栏，一行空行）
    screen_rows = rows - 2;
    screen_cols = cols;
    
    // 确保最小尺寸
    if (screen_rows < 1) screen_rows = 1;
    if (screen_cols < 1) screen_cols = 1;
}

void display_clear(void) {
    write(STDOUT_FILENO, "\x1b[2J", 4); // 清屏
    write(STDOUT_FILENO, "\x1b[H", 3);  // 光标移到左上角
}

void display_file(TextBuffer *buffer, int row_offset, int col_offset) {
    // 安全计算输出缓冲区大小
    size_t output_size = (screen_rows * (screen_cols + 2)) + 1; // +2 为 \r\n
    char *output = malloc(output_size);
    if (!output) return;
    
    char *out_ptr = output;
    int current_row = 0;
    
    if (buffer->data && buffer->size > 0) {
        const char *p = buffer->data;
        size_t len = buffer->size;
        int row = 0;
        int col = 0;
        int in_line = 0;
        
        // 首先跳过不需要显示的行（row_offset）
        for (size_t i = 0; i < len; i++) {
            if (row < row_offset) {
                if (p[i] == '\n') {
                    row++;
                    col = 0;
                }
                continue;
            }
            break;
        }
        
        // 处理需要显示的行
        for (size_t i = 0; i < len && current_row < screen_rows; i++) {
            // 处理换行符
            if (p[i] == '\n') {
                // 确保不越界
                if (current_row < screen_rows) {
                    *out_ptr++ = '\r';
                    *out_ptr++ = '\n';
                    current_row++;
                    in_line = 0;
                }
                col = 0;
                continue;
            }
            
            // 跳过偏移列
            if (col < col_offset) {
                col++;
                continue;
            }
            
            // 处理制表符
            if (p[i] == '\t') {
                int spaces = 8 - (in_line % 8);
                for (int j = 0; j < spaces && in_line < screen_cols; j++) {
                    *out_ptr++ = ' ';
                    in_line++;
                }
                col++;
                continue;
            }
            
            // 处理可打印字符
            if (p[i] >= ' ' && in_line < screen_cols) {
                *out_ptr++ = p[i];
                in_line++;
                col++;
            }
        }
    }
    
    // 填充剩余行
    while (current_row < screen_rows) {
        *out_ptr++ = '\r';
        *out_ptr++ = '\n';
        current_row++;
    }
    
    *out_ptr = '\0';
    write(STDOUT_FILENO, output, out_ptr - output);
    free(output);
}

void display_status_bar(const char *filename, int row, int col, int total_rows) {
    char status[80];
    int len = snprintf(status, sizeof(status), "\x1b[7m%.20s - %d/%d (%d:%d)\x1b[0m",
                      filename ? filename : "[No Name]", row + 1, total_rows, row + 1, col + 1);
    
    write(STDOUT_FILENO, status, len);
    write(STDOUT_FILENO, "\r\n", 2);
}

void display_message_bar(const char *message) {
    write(STDOUT_FILENO, "\x1b[K", 3); // 清除行
    if (message) {
        write(STDOUT_FILENO, message, strlen(message));
    }
}

void set_cursor_position(int row, int col) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", row + 1, col + 1);
    write(STDOUT_FILENO, buf, len);
}