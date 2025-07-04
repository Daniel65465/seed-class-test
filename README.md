# seed-class-test
代码的文件结构，自己的解题思路，历程，对每个问题的理解分析。
##  level 1
###  代码结构：  
├── main.c            # 程序入口  
├── editor.h          # 编辑器接口声明  
├── editor.c          # 编辑器主逻辑  
├── terminal.h        # 终端控制接口  
└── terminal.c        # 终端控制实现  
###  解题思路，历程，对每个问题的理解分析：  
level 1 的核心目标是建立基本编辑器框架：  
终端原始模式切换​​：禁用行缓冲和回显，实现逐字符输入处理  
基本输入输出​​：实现按键读取和简单回显功能  
安全退出机制​​：确保程序退出时恢复终端设置  
####  实现历程：  
​​终端原始模式实现​​：  
使用 termios API 修改终端设置，禁用 ECHO、ICANON、ISIG 等标志位，设置 VMIN 和 VTIME 控制读取行为  
按键处理设计​​：  
实现 read_key() 函数处理原始输入，添加方向键特殊处理逻辑，按 'q' 退出  
####  问题理解分析：   
我认为原始模式是十分重要的，同时编辑其可以立即对键盘上面的按键作出相应，同时避免缓冲来带的延时，并且可以防止特殊信号的键入。出于安全角度，程序崩溃时也能恢复终端设置，避免终端处于原始模式无法使用。与此同时，我让终端控制与业务逻辑分离，使每一个模块只专注于一个功能，通过将.c中的子程序放入.h，并在其他的文件之中使用，使模块间通过接口交互。  

##  level 2  

###  代码结构：  
├── main.c            # 程序入口  
├── editor.h          # 扩展编辑器接口  
├── editor.c          # 增强编辑器逻辑  
├── terminal.h        # 扩展终端控制接口  
├── terminal.c        # 增强终端控制  
├── buffer.h          # 缓冲区接口  
├── buffer.c          # 缓冲区管理  
├── file_io.h         # 文件I/O接口  
├── file_io.c         # 文件操作实现  
├── display.h         # 显示接口  
└── display.c         # 显示实现  
###  解题思路，历程，对每个问题的理解分析：  
Level 2 的核心目标是构建完整的文本显示系统：  
文件加载与显示​​：读取文件内容并正确渲染  
​​光标导航​​：实现方向键移动和滚动  
​​状态显示​​：显示文件名和位置信息  
###  实现历程：  
​​文件加载模块​​：  
实现文件读写接口，处理文件不存在情况，支持空文件创建  
​​缓冲区管理​​：  
设计文本缓冲区结构，实现高效行数计算算法，确保内存安全操作  
显示系统​​：  
使用 ANSI 转义序列控制终端，实现滚动显示逻辑，添加状态栏和光标定位  
​​导航系统​​：  
实现光标移动逻辑，支持 Home/End/PageUp/PageDown  
###  问题理解分析
对于​​文本显示​​，要求我正确处理长行和制表符，支持不同终端尺寸，若发生错误，要求我创建文件不存在时创建空缓冲区，终端尺寸获取失败的后备方案，内存分配错误处理策略。我将文件想象恒为一个矩阵，通过矩阵内部的行列以及offset来找到所需要的内容。与此同时，我也使每一个模块只专注于一个功能，通过将.c中的子程序放入.h，并在其他的文件之中使用，使模块间通过接口交互


