#include "buffer.h"
#include <string.h>
#include <stdlib.h>

void init_buffer(TextBuffer *buffer) {
    buffer->data = NULL;
    buffer->size = 0;
    buffer->lines = 0;
}

void free_buffer(TextBuffer *buffer) {
    if (buffer->data) free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->lines = 0;
}

int count_lines(TextBuffer *buffer) {
    if (!buffer->data || buffer->size == 0) {
        buffer->lines = 1;
        return 1;
    }
    
    int lines = 1;
    for (size_t i = 0; i < buffer->size; i++) {
        if (buffer->data[i] == '\n') {
            lines++;
        }
    }
    
    // 如果文件不以换行符结束，但包含内容，则增加一行
    if (buffer->size > 0 && buffer->data[buffer->size-1] != '\n') {
        lines++;
    }
    
    buffer->lines = lines;
    return lines;
}