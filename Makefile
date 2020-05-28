
.PHONY: default cleanup clean

default: usemem

usemem:
	gcc -g -O0 ./usemem.c -o usemem

cleanup: clean
	rm -f usemem

clean:
	rm -f *~
