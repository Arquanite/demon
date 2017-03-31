SOURCES := $(wildcard sources/*.c)

all: build
	gcc -o build/demon666 $(SOURCES) -I headers #all in one

build:
	mkdir build

clean:
	rm -rf build
