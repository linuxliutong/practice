.PHONY = all clean
OBJ_DIR = $(CUR_DIR)/obj
CUR_DIR = $(shell pwd)
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
HEADERS = 
TARGETS = test

all : $(OBJ_DIR)  $(TARGETS)
	@echo "$(TARGETS) compiled!"
$(OBJ_DIR):
	-mkdir $@

$(TARGETS) : $(OBJECTS)
	$(CC) -g -o $@ $^

$(OBJ_DIR)/%.o : %.c
	$(CC) -o $@ -c $^

clean:
	-rm -f $(OBJ_DIR)/*.o $(TARGETS)
