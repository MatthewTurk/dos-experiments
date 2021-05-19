# So, what -fcf-protection=none does is avoid the generation of some assembly that
# dosbox/dos doesn't understand, specifically instructions like ENDBR32.
# There may be better ways around it, but this is what worked.  I think
# this is related to Spectre.
CFLAGS=-std=gnu99 -Os -nostdlib -m32 -march=i386 -ffreestanding -fno-pie \
		-fcf-protection=none \
		-Wl,--nmagic,--script=com.ld 

hello.com : hello.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: run clean

clean:
	rm -f *.com *.o

run : hello.com
	dosemu -t exec.bat
