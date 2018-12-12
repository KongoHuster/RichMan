OBJ = diff.o
SOURCE_FILE = diff.c
TARGET = diff

RM = rm -r
GCC = gcc

diff:$(OBJ)
	$(GCC) -o $(TARGET) $(OBJ)

$(OBJ):$(SOURCE_FILE)
	$(GCC) -c $(SOURCE_FILE) -o $(OBJ)

.PHONY:clean
clean :
	$(RM) -r $(TARGET) $(OBJ_DIR) *.o
