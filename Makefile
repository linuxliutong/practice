BASE_DIR = $(shell pwd)
CC = gcc

export BASE_DIR CC

.PHONY: all clean 

SUB_DIR = $(BASE_DIR)/1118_double $(BASE_DIR)/test

all :
	make -C $(BASE_DIR)/1118_double
	make -C $(BASE_DIR)/test

clean:
	make -C $(BASE_DIR)/1118_double clean
	make -C $(BASE_DIR)/test clean
