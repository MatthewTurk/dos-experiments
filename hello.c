asm (".code16gcc\n"
     "call  dosmain\n"
     "mov   $0x4C,%ah\n"
     "int   $0x21\n");

static void print(char *string)
{
    asm volatile ("mov   $0x09, %%ah\n"
                  "int   $0x21\n"
                  : /* no output */
                  : "d"(string)
                  : "ah");
}

static void readline(volatile char *buff)
{
    volatile char input_buffer[257];
    volatile char *_buff = input_buffer;
    input_buffer[0] = 254; /* Leave room for $ terminator */
    asm volatile ("mov   $0x0c, %%ah\n"
                  "mov   $0x0a, %%al\n"
                  "int   $0x21\n"
                  : "+d"(_buff)
                  :
                  : "al", "ah");
    for(int i = 0; i < (int) input_buffer[1]; i++) {
        buff[i] = _buff[2 + i];
    }
    buff[input_buffer[1] + 2] = '$';
}

int dosmain(void)
{
    volatile char input_buffer[255];
    print("Hello, World!\n$");
    readline(input_buffer);
    print("\nReceived:\n$");
    print(input_buffer);
    print("\nEnding.\n$");
    return 0;
}
