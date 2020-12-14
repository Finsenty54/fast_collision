#make -f Make.Linux 或 make --file Make.AIX
#% 的意思是匹配零或若干字符
#$(patsubst %.c,%.o,$(wildcard *.c))


#安装boost
#./b2 install 后
#执行sudo ldconfig
#运行程序后就能加载共享库


objects :=my_first_try.o block0.o block1.o md5.o block1stevens00.o \
block1stevens01.o block1stevens10.o block1stevens11.o block1wang.o


first_try: $(objects) 
	g++ -o first_try $(objects) -lboost_timer 

my_first_try.o: main.hpp
block0.o: main.hpp
block1.o: main.hpp
md5.o: main.hpp
block1stevens00.o: main.hpp
block1stevens01.o: main.hpp
block1stevens10.o: main.hpp
block1stevens11.o: main.hpp
block1wang.o: main.hpp

.PHONY : clean
clean :
	rm $(objects) first_try