#ifndef TERMINAL_H
#define TERMINAL_H

#include <sys/ioctl.h>
#include <termios.h>

// 获取终端大小
void get_terminal_size(int *rows, int *cols);

// 启用原始模式
void enable_raw_mode(void);

// 恢复默认模式
void disable_raw_mode(void);

// 读取按键输入
int read_key(void); // 返回整数以便处理特殊键

#endif