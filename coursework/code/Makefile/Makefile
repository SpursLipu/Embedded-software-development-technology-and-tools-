OBJS = hello.o sum.o

hello:$(OBJS)
hello.o:hello.c
	gcc -c $^ -o $@
sum.o:sum.c
	gcc -c $^ -o $@
.PHONY:clean
clean:
	rm *.o
	rm hello
