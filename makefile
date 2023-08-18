.PHONY: test install uninstall clean

# 强制环境
SHELL := /bin/bash

# 制作标志
MAKEFLAGS += --no-silent --no-print-directory

# 库类定义
LIB := lszlib
HDR := include/$(LIB).h
OUT := lib$(LIB).a
LIB_INS_DIR := ${HOME}/.local/lib/$(LIB)
HDR_INS_DIR := ${HOME}/.local/include/$(LIB)
LIB_SLK_DIR := /usr/local/lib
HDR_SLK_DIR := /usr/local/include

# 源与目标
SRC := $(shell find src -name "*.c")
OBJ := $(SRC:%.c=%.o)

# 编译参数
CC      := gcc
DEFINES :=
CFLAGS  := -g
LDFLAGS :=
HDR_INC := $(addprefix -I,$(SRC_DIRS)) -Iinclude
LIB_INC :=

# 测试
test: install
	@make -C test

# 安装
install: uninstall $(OUT)
	@if [ ! -d $(LIB_INS_DIR) ]; then mkdir -p $(LIB_INS_DIR); fi
	@if [ ! -d $(HDR_INS_DIR) ]; then mkdir -p $(HDR_INS_DIR); fi
	@cp -f $(HDR) $(HDR_INS_DIR)
	@cp -f $(OUT) $(LIB_INS_DIR)
	@if [ ! -L $(LIB_SLK_DIR)/$(OUT)   ] && [ -f $(LIB_INS_DIR)/$(OUT)   ]; then sudo ln -sf $(LIB_INS_DIR)/$(OUT)   $(LIB_SLK_DIR)/$(OUT)  ; fi
	@if [ ! -L $(HDR_SLK_DIR)/$(LIB).h ] && [ -f $(HDR_INS_DIR)/$(LIB).h ]; then sudo ln -sf $(HDR_INS_DIR)/$(LIB).h $(HDR_SLK_DIR)/$(LIB).h; fi

# 链接
$(OUT): $(OBJ)
ifeq ($(suffix $(OUT)), .a)
	@$(AR) -crs $@ $^
else ifeq ($(suffix $(OUT)), .so)
	@$(CC) $(DEFINES) $(CFLAGS) $(HDR_INC) -shared -o $@ $^
else
	$(error $(OUT)'s suffix: '$(suffix $(OUT))' is either '.a' or '.so')
endif

# 编译
%.o : %.c
ifeq ($(suffix $(OUT)), .a)
	@$(CC) $(DEFINES) $(CFLAGS) $(HDR_INC) -o $@ -c $^
else ifeq ($(suffix $(OUT)), .so)
	@$(CC) $(DEFINES) $(CFLAGS) $(HDR_INC) -o $@ -c $^ -fPIC
else
	$(error $(OUT)'s suffix: '$(suffix $(OUT))' is either '.a' or '.so')
endif

# 卸载
uninstall:
	@$(RM) -rf $(LIB_INS_DIR)
	@$(RM) -rf $(HDR_INS_DIR)
	@if [ -L $(LIB_SLK_DIR)/$(OUT)   ]; then sudo $(RM) -f $(LIB_SLK_DIR)/$(OUT)   ; fi
	@if [ -L $(HDR_SLK_DIR)/$(LIB).h ]; then sudo $(RM) -f $(HDR_SLK_DIR)/$(LIB).h ; fi

# 清理
clean:
	@$(RM) -f $(OBJ)
	@$(RM) -f $(OUT)
	@make -C test clean
