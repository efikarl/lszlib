.PHONY: run build clean

# 程序定义
APP := a.out

# 源与目标
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o) $(LIB:.c=.o)

# 编译参数
CC      := gcc
DEFINES := 
CFLAGS  := -g -fsanitize=address -O0 -fno-omit-frame-pointer
LDFLAGS := -l lszlib
LIB_INC := -L ${HOME}/.local/lib
HDR_INC := -I ${HOME}/.local/include/lszlib

# 运行
run: clean build
	@./$(APP)

# 构建
build: $(APP)

# 链接
$(APP):$(OBJ)
	@$(CC) $(LIB_INC) $(CFLAGS) -o $@ $^	$(LDFLAGS)

# 编译
%.o : %.c
	@$(CC) $(HDR_INC) $(CFLAGS) $(DEFINES) -o $@ -c $^

# 清理
clean:
	@$(RM) -f $(OBJ)
	@$(RM) -f $(APP)
