#include "terminal.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define KEY_UP    0x100
#define KEY_DOWN  0x101
#define KEY_RIGHT 0x102
#define KEY_LEFT  0x103
#define HOME_KEY  0x104
#define END_KEY   0x105
#define PAGE_UP   0x106
#define PAGE_DOWN 0x107

static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void get_terminal_size(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        // 后备方案 - 使用环境变量或默认值
        char *row_str = getenv("LINES");
        char *col_str = getenv("COLUMNS");
        
        if (row_str && col_str) {
            *rows = atoi(row_str);
            *cols = atoi(col_str);
        } else {
            // 最终后备方案
            *cols = 80;
            *rows = 24;
        }
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
    }
    
    // 确保最小尺寸
    if (*rows < 3) *rows = 3;  // 至少3行（内容+状态栏）
    if (*cols < 10) *cols = 10; // 至少10列
}

int read_key() {
    char c;
    while (read(STDIN_FILENO, &c, 1) != 1);
    
    // 处理特殊键（如方向键）
    if (c == '\x1b') {
        char seq[3] = {0};
        
        // 读取可能的转义序列
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
        
        // 处理多字符序列
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                // 读取第三个字符（如果有）
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                
                // 处理 PageUp/PageDown
                if (seq[1] == '5' && seq[2] == '~') return PAGE_UP;
                if (seq[1] == '6' && seq[2] == '~') return PAGE_DOWN;
                
                // 处理 Home/End 
                if (seq[1] == '1' && seq[2] == '~') return HOME_KEY;
                if (seq[1] == '4' && seq[2] == '~') return END_KEY;
            } else {
                // 处理其他单字符序列
                switch (seq[1]) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'C': return KEY_RIGHT;
                    case 'D': return KEY_LEFT;
                    case 'H': return HOME_KEY; 
                    case 'F': return END_KEY;     
                }
            }
        } 
        
        return '\x1b';
    }
    
    return c;
}