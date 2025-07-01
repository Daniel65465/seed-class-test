#ifndef TERMINAL_H
#define TERMINAL_H

// 启用原始模式
void enable_raw_mode(void);

// 恢复默认模式
void disable_raw_mode(void);

// 读取按键输入
char read_key(void);

#endif