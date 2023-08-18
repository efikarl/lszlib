.PHONY: install uninstall clean

# 强制bash
SHELL=/bin/bash

# 库类定义
LIB := lszlib
OUT := lib$(LIB).a
INS_LIB_DIR := ${HOME}/.local/lib/$(LIB)
INS_HDR_DIR := ${HOME}/.local/include/$(LIB)
SLK_LIB_DIR := /usr/local/lib
SLK_HDR_DIR := /usr/local/include

# 源与目标
SRC := $(wildcard *.c)
HDR := $(wildcard *.h)
OBJ := $(SRC:%.c=%.o)

# 编译参数
CC      := gcc
DEFINES :=
CFLAGS  := -g
LDFLAGS :=
HDR_INC :=
LIB_INC :=

# 安装
install: $(OUT)
	@if [ ! -d $(INS_LIB_DIR) ]; then mkdir -p $(INS_LIB_DIR); fi
	@if [ ! -d $(INS_HDR_DIR) ]; then mkdir -p $(INS_HDR_DIR); fi
	@cp -f $(LIB).h $(INS_HDR_DIR)
	@cp -f $(OUT)   $(INS_LIB_DIR)
	@if [ ! -L $(SLK_LIB_DIR)/$(OUT)   ] && [ -f $(INS_LIB_DIR)/$(OUT)   ]; then sudo ln -sf $(INS_LIB_DIR)/$(OUT)   $(SLK_LIB_DIR)/$(OUT)  ; fi
	@if [ ! -L $(SLK_HDR_DIR)/$(LIB).h ] && [ -f $(SLK_HDR_DIR)/$(LIB).h ]; then sudo ln -sf $(INS_HDR_DIR)/$(LIB).h $(SLK_HDR_DIR)/$(LIB).h; fi

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
	@$(RM) -rf $(INS_LIB_DIR)
	@$(RM) -rf $(INS_HDR_DIR)
	@if [ -L $(SLK_LIB_DIR)/$(OUT)   ]; then sudo $(RM) -f $(SLK_LIB_DIR)/$(OUT)   ; fi
	@if [ -L $(SLK_HDR_DIR)/$(LIB).h ]; then sudo $(RM) -f $(SLK_HDR_DIR)/$(LIB).h ; fi

# 清理
clean:
	@$(RM) -f $(OBJ)
	@$(RM) -f $(OUT)
