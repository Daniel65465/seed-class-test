#ifndef FILE_IO_H
#define FILE_IO_H

#include <stddef.h> 

// 文件操作状态码
#define FILE_OK 0
#define FILE_OPEN_ERROR 1
#define FILE_READ_ERROR 2

// 文件操作接口
int load_file(const char *filename, char **buffer, size_t *size);
void free_file_buffer(char *buffer);

#endif