# Toolchain
CC		= clang
LD		= clang
AR		= ar
TAR		= tar
AWK		= awk
DIFF		= diff

# Flags
CFLAGS		= -Wall -Wextra
CPPFLAGS	=
LDFLAGS		=

# Arch
ARCH		= -march=native

# Misc
BUILDDIR	= build

# Test for build mode
ifeq ($(DEBUG),1)
	CFLAGS  	+= -O0 -g
	CPPFLAGS	+= -DDEBUG
	BUILDDIR	 = build/debug
else
	CFLAGS  	+= -O3
	CPPFLAGS	+= -DNDEBUG
	BUILDDIR	 = build/release
endif

all: $(BUILDDIR)/blaze/blaze

$(BUILDDIR)/blaze/blaze: $(BUILDDIR)/blaze/blaze.o $(BUILDDIR)/blaze/blaze.a
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILDDIR)/test/perft: $(BUILDDIR)/test/perft.o $(BUILDDIR)/blaze/blaze.a
	$(LD) $(LDFLAGS) $(ARCH) $^ $(LDLIBS) -o $@

$(BUILDDIR)/blaze/blaze.a: $(filter-out %/blaze.o,$(patsubst blaze/%.c,$(BUILDDIR)/blaze/%.o,$(wildcard blaze/*.c)))

$(BUILDDIR)/%.a:
	rm -f $@
	$(AR) rcs $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH) -c -o $@ $<

# TODO: add test pass/fail information
check: $(BUILDDIR)/test/perft
	@echo perft rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 1' test/results | head -n 6) \
	<($(BUILDDIR)/test/perft 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1' 5)	\

	@echo perft r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 2' test/results | head -n 3) \
	<($(BUILDDIR)/test/perft 'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -' 2) \

	@echo perft 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -
	@time $(DIFF) \
	<($(AWK) -v RS= -v FS=\n 'NR == 3' test/results | head -n 3) \
	<($(BUILDDIR)/test/perft '8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -' 2) \

dist:

install:

clean:
	rm -rf $(BUILDDIR)

.PHONY: all check dist install clean
