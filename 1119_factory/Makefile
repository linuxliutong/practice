exe:=test
temp := $(wildcard test *~)

all:test.c factory.c factory.h product.h producta.c producta.h productb.c productb.h typedef.h
	gcc -g $^ -o $(exe) 

clean:
	rm $(temp)
