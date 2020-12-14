#make -f Make.Linux 或 make --file Make.AIX
#% 的意思是匹配零或若干字符
#$(patsubst %.c,%.o,$(wildcard *.c))


#安装boost
#./b2 install 后
#执行sudo ldconfig
#运行程序后就能加载共享库


#想要调试，是生成对象文件时加-g ， 而不是最后链接时

objects :=my_first_try.o block0.o block1.o md5.o block1stevens00.o \
block1stevens01.o block1stevens10.o block1stevens11.o block1wang.o


first_try: $(objects) 
	g++ -o first_try $(objects) -lboost_timer 

my_first_try.o: my_first_try.cpp main.hpp
	g++ -g -c my_first_try.cpp
block0.o: block0.cpp main.hpp
	g++ -g -c block0.cpp
block1.o: block1.cpp main.hpp
	g++ -g -c block1.cpp
md5.o: main.hpp md5.cpp
	g++ -g -c md5.cpp
block1stevens00.o: main.hpp block1stevens00.cpp
	g++ -g -c block1stevens00.cpp
block1stevens01.o: main.hpp block1stevens01.cpp
	g++ -g -c block1stevens01.cpp
block1stevens10.o: main.hpp block1stevens10.cpp
	g++ -g -c block1stevens10.cpp
block1stevens11.o: main.hpp block1stevens11.cpp
	g++ -g -c block1stevens11.cpp
block1wang.o: main.hpp block1wang.cpp
	g++ -g -c block1wang.cpp

.PHONY : clean
clean :
	rm $(objects) first_try