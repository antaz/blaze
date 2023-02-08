all: blaze

blaze: o/$(MODE)/blaze

test: o/$(MODE)/test

clean:
	rm -rf o

include build/config.mk
include build/rules.mk
include blaze/blaze.mk
include test/test.mk

.PHONY: o all test clean
