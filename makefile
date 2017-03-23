main: main.c build
	gcc -o build/main main.c

run: main
	build/main &

build:
	mkdir build

clean:
	rm -rf build
