.PHONY: all install build start clean

all: build start

install:
	npm install

build:
	bsb -make-world

start:
	# bsb -make-world -w
	node ./bin/www

clean:
	bsb -clean-world
