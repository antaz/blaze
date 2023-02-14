# Toolchain
CC		= clang
LD		= ld.lld
AR		= ar
TAR		= tar

# Flags
CFLAGS		= -O3 -Wall -Werror -Wextra
CPPFLAGS	= -DNDEBUG
LDFLAGS		=

# TODO: allow for seperate debug and release builds

# Arch
ARCH		= -march=native

# Misc
BUILDDIR	= build

all: $(BUILDDIR)/blaze/blaze

$(BUILDDIR)/blaze/blaze: $(BUILDDIR)/blaze/blaze.o $(BUILDDIR)/blaze/blaze.a
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILDDIR)/test/perft: $(BUILDDIR)/test/perft.o $(BUILDDIR)/blaze/blaze.a
	$(CC) $(LDFLAGS) $(ARCH) $^ $(LDLIBS) -o $@

$(BUILDDIR)/blaze/blaze.a: $(filter-out %/blaze.o,$(patsubst blaze/%.c,$(BUILDDIR)/blaze/%.o,$(wildcard blaze/*.c)))

$(BUILDDIR)/%.a:
	rm -f $@
	$(AR) rcs $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH) -c -o $@ $<

check: $(BUILDDIR)/test/perft
	$(BUILDDIR)/test/perft

bench:

dist:

install:

clean:
	rm -rf $(BUILDDIR)

.PHONY: all test bench dist install clean
