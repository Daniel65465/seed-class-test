#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "terminal.h"

static struct termios orig_termios; // 保存原始终端设置

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    // 获取当前终端设置
    tcgetattr(STDIN_FILENO, &orig_termios);
    // 程序退出时自动恢复原始模式
    atexit(disable_raw_mode);
    
    struct termios raw = orig_termios;
    // 关闭回显/规范模式/信号处理
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // 关闭特殊输入处理
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    // 关闭特殊输出处理
    raw.c_oflag &= ~(OPOST);
    // 设置字符大小为8位
    raw.c_cflag |= (CS8);
    // 设置最小读取字节数和超时
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    // 应用新设置
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char read_key() {
    char c = '\0';
    // 阻塞读取单个字符
    while (read(STDIN_FILENO, &c, 1) != 1);
    return c;
}