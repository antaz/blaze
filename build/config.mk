CC 		= clang
CFLAGS		=
CPPFLAGS 	=
LDLIBs 		= -lm

LDFLAGS 	=
TARGET_ARCH 	= -march=native
TAGS 		= /usr/bin/ctags

ifeq ($(MODE), dbg)
CFLAGS 		+= -O0 -g -Wall -Werror -fstack-protector
CPPFLAGS 	+= -DDEBUG
endif

ifeq ($(MODE), rel)
CFLAGS 		+= -O3
CPPFLAGS 	+= -DNDEBUG
endif
