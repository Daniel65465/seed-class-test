#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>

int load_file(const char *filename, char **buffer, size_t *size) {
    FILE *file = fopen(filename, "r");
    if (!file) return FILE_OPEN_ERROR;
    
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        fclose(file);
        return FILE_READ_ERROR;
    }
    
    // 分配缓冲区
    *buffer = malloc(file_size + 1);
    if (!*buffer) {
        fclose(file);
        return FILE_READ_ERROR;
    }
    
    // 读取文件内容
    size_t bytes_read = fread(*buffer, 1, file_size, file);
    
    if (bytes_read != (size_t)file_size) {
        free(*buffer);
        fclose(file);
        return FILE_READ_ERROR;
    }
    
    (*buffer)[file_size] = '\0'; // 添加空终止符
    *size = file_size;
    fclose(file);
    return FILE_OK;
}

void free_file_buffer(char *buffer) {
    if (buffer) free(buffer);
}