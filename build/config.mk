CC = clang
CFLAGS +=				\
	-g				\
	-Wall				\
	-Werror				\
	-O2				\
	-fstack-protector		

LDLIBs +=				\
	-lm		

LDFLAGS +=
TARGET_ARCH = -march=native
TAGS ?= /usr/bin/ctags
