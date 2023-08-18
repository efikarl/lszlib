MAKEFLAGS += --silent --no-print-directory

SUBDIRS := base list tree path sort perm comb

# 运行
run:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir   run; done

# 构建
build:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir build; done

# 清理
clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

.PHONY: run build clean
