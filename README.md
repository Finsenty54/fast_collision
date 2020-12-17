# fast_collision
1. Stevens M. Fast Collision Attack on MD5[J]. IACR Cryptol.  ePrint Arch., 2006, 2006: 104.

https://www.win.tue.nl/hashclash/

1. Klima V. Tunnels in Hash Functions: MD5 Collisions Within a Minute[J]. IACR Cryptol. ePrint Arch., 2006, 2006: 105.

http://cryptography.hyperlink.cz/MD5_collisions.html

# 使用
需要安装boost

https://www.jianshu.com/p/0634e610d519

1. `make` 编译链接
1. `./first_try` 运行
1. `make clean` 清理对象文件与ELF文件 
# 修改
1. 对源码添加了注释
1. 创建一个新的my_first_try.cpp，使用简单的功能，简单修改确保在最新的boost下可用，最后输出结果验证。
1. 使用make编译
-make
-make clean
1. 实现linux下 VSCodium gdb调试

# 教训
1. 想要调试，是生成对象文件时加-g ， 而不是最后链接时
1. launch.json中："stopAtEntry": false, //别设置成true,报找不到./csu/../csu/libc-start.c错误
1. task.json 编译任务
launch.json 调试任务
