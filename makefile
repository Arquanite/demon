SOURCES := $(wildcard sources/*.c)

all: build
	gcc -o build/demon $(SOURCES) -I headers #all in one

build:
	mkdir build

clean:
	rm -rf build
	
install: all
  cp demon /usr/bin

uninstall:
  rm /usr/bin/demon
