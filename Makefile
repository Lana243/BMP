all:hw_01

CFLAGS = -I include -Wall -Wextra -Werror -g

obj:
	mkdir obj

hw_01: obj/bmp.o obj/main.o
	gcc obj/bmp.o obj/main.o -o hw_01

obj/main.o: src/main.c include/bmp.h | obj
	gcc $(CFLAGS) -c src/main.c -o obj/main.o

obj/bmp.o: src/bmp.c include/bmp.h | obj
	gcc $(CFLAGS) -c src/bmp.c -o obj/bmp.o

clean:
	rm -rf obj hw_01

.PHONY: all clean
