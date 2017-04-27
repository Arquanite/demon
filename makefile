SOURCES := $(wildcard sources/*.c)

all:
	gcc -o demon $(SOURCES) -I headers

clean:
	rm demon
	
install: all
	cp demon /usr/bin

uninstall:
	rm /usr/bin/demon

test:
	./test.sh
