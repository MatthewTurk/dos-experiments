CFLAGS=-std=gnu99 -Os -nostdlib -m32 -march=i386 -ffreestanding -fno-pie \
	   -Wl,--nmagic,--script=com.ld 

hello.com : hello.c com.ld
	gcc $(CFLAGS) -o $@ $^
