VERSION		= 0.1

# toolchain
CC		= clang
LD		= clang
TAR		= tar
TAG		= etags

# vpath
VPATH		= src:test

# include dir
INCLUDE     = include

# build dir
BDIR		= build

# Flags
CFLAGS		= -Wall -Wextra
CPPFLAGS	= -I$(INCLUDE)
LDFLAGS		=

# architecture
ARCH		= -march=native

# build mode
ifneq ($(DEBUG), )
	CFLAGS  	+= -Werror \
				   -g -O0 -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
				   -fsanitize=address
	CPPFLAGS	+= -DDEBUG
	BDIR	 	= build/debug
else
	CFLAGS  	+= -O3
	CPPFLAGS	+= -DNDEBUG
	BDIR	 	= build/release
endif

all: $(BDIR)/blaze

$(BDIR)/blaze: $(BDIR)/blaze.o $(filter-out %/blaze.o,$(patsubst src/%.c,$(BDIR)/%.o,$(wildcard src/*.c)))
	$(LD) $(LDFLAGS) $(CFLAGS) $^ $(LDLIBS) -o $@

$(BDIR)/test/perft: $(BDIR)/test/perft.o $(BDIR)/blaze.a
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BDIR)/%.o: %.c $(BDIR)/%.d
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(ARCH) $(CPPFLAGS) -c -o $@ $<

$(BDIR)/%.d: %.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PRECIOUS: $(BDIR)/%.d

# alternatively supress errors with `-include`
include $(wildcard $(patsubst src/%.c, $(BDIR)/%.d, $(wildcard src/*.c)))

dist:
	@mkdir blaze-$(VERSION)
	@cp -r blaze -r man -r test Makefile README.md LICENSE blaze-$(VERSION)
	@$(TAR) cvzf blaze-$(VERSION).tar.gz blaze-$(VERSION)
	@rm -rf blaze-$(VERSION)

# TODO: use PREFIX instead of hardcoded path
install: $(BDIR)/blaze/blaze
	@cp $(BDIR)/blaze/blaze /usr/local/bin/
	@gzip < man/blaze.1 > /usr/local/man/man1/blaze.1.gz

dist-clean:
	@rm -rf $(BDIR)
	@rm -f blaze-$(VERSION).tar.gz

tags:
	$(TAG) **/*.c **/*.h

format:
	clang-format -i -style=file **/*.c **/*.h

clean:
	@rm -rf build/*

.PHONY: all dist install dist-clean tags clean
