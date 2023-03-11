VERSION		= 0.1

# toolchain
CC		= clang
LD		= clang
AR		= ar
TAR		= tar
AWK		= awk
DIFF		= diff
TAG		= etags

# vpath
VPATH		= src:test

# build dir
BDIR		= build

# Flags
CFLAGS		= -Wall -Wextra
CPPFLAGS	= 
LDFLAGS		=
DFLAGS	       	= -MT $@ -MMD -MP -MF $(BDIR)/$*.d 

# architecture
ARCH		= -march=native

# build mode
ifneq ($(DEBUG), )
	CFLAGS  	+= -O0 -g
	CPPFLAGS	+= -DDEBUG
	BDIR	 	= build/debug
else
	CFLAGS  	+= -O3
	CPPFLAGS	+= -DNDEBUG
	BDIR	 	= build/release
endif

all: $(BDIR)/blaze

$(BDIR)/blaze: $(BDIR)/blaze.o $(BDIR)/blaze.a
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BDIR)/test/perft: $(BDIR)/test/perft.o $(BDIR)/blaze.a
	$(LD) $(LDFLAGS) $(ARCH) $^ $(LDLIBS) -o $@

$(BDIR)/blaze.a: $(filter-out %/blaze.o,$(patsubst src/%.c,$(BDIR)/%.o,$(wildcard src/*.c)))

$(BDIR)/%.a:
	$(AR) rcs $@ $^

$(BDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DFLAGS) $(CPPFLAGS) $(ARCH) -c -o $@ $<

# alternatively supress errors with `-include`
include $(wildcard $(patsubst src/%.c, $(BDIR)/%.d, $(wildcard src/*.c)))

# use bash for process substitution <()
check: SHELL :=/bin/bash

# TODO: add test pass/fail information
check: $(BDIR)/test/perft
	@echo perft rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 1' test/results | head -n 6) \
	<($(BDIR)/test/perft 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1' 5)	\

	@echo perft r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 2' test/results | head -n 3) \
	<($(BDIR)/test/perft 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -' 2) \

	@echo perft 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 3' test/results | head -n 3) \
	<($(BDIR)/test/perft '8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -' 2) \

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
	@rm -f $(BDIR)/*.o
	@rm -f $(BDIR)/*.a
	@rm -f $(BDIR)/*.d
	@rm -f $(BDIR)/blaze

.PHONY: all check dist install dist-clean tags clean
