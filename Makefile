# Toolchain
CC		= clang
LD		= ld
AR		= ar
TAR		= tar

# Flags
CFLAGS		= 
CPPFLAGS	=
LDFLAGS		=

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

test: $(BUILDDIR)/test/perft

bench:

dist:

install:

clean:
	rm -rf $(BUILDDIR)

.PHONY: all test bench dist install clean
