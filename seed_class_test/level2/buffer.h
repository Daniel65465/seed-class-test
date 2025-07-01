#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

// 文本缓冲区结构
typedef struct {
    char *data;         // 文本数据
    size_t size;        // 数据大小
    int lines;          // 总行数 (修复为 int)
} TextBuffer;

// 缓冲区操作接口
void init_buffer(TextBuffer *buffer);
void free_buffer(TextBuffer *buffer);
int count_lines(TextBuffer *buffer);

#endif