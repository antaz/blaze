VERSION		= 0.1

# toolchain
CC		= cc
TAR		= tar
TAG		= etags

# vpath
VPATH		= src

debug: build/debug/blaze
release: build/release/blaze

build/debug/blaze: build/debug/blaze.o $(filter-out %/blaze.o,$(patsubst src/%.c,build/debug/%.o,$(wildcard src/*.c)))
	$(CC) -fsanitize=address $^ -o $@

build/debug/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c -Wall -Wextra -g3 -O0 -fsanitize=address -fno-omit-frame-pointer -Iinclude -DDEBUG $< -o $@

build/release/blaze: build/release/blaze.o $(filter-out %/blaze.o,$(patsubst src/%.c,build/release/%.o,$(wildcard src/*.c)))
	$(CC) -flto -pthread $^ -o $@

build/release/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c -Wall -Wextra -Werror -Ofast -static -flto -Iinclude -DNDEBUG $< -o $@

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
