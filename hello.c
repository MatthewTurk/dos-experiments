/* http://masm32.com/board/index.php?topic=1902.0 */
asm (".code16gcc\n"
     "mov %ss, %bx\n" /* Next bit resizes our program */
     "mov %es, %ax\n"
     "sub %ax, %bx\n"
     "mov %sp, %ax\n"
     "shr $0x04, %ax\n"
     "inc %ax\n"
     "add %ax, %bx\n"
     "mov $0x4A, %ah\n"
     "int   $0x21\n"
     "call  dosmain\n"
     "mov   $0x4C, %ah\n"
     "int   $0x21\n");

#define INPUT_BUFSIZE 255

static void print(volatile char *string)
{
    asm volatile ("mov   $0x09, %%ah\n"
                  "int   $0x21\n"
                  : /* no output */
                  : "d"(string)
                  : "ah");
}

volatile char* readline(volatile char *buff)
{
    volatile char input_buffer[INPUT_BUFSIZE + 2];
    volatile char *_buff = input_buffer;
    input_buffer[0] = INPUT_BUFSIZE - 1; /* Leave room for $ terminator */
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

volatile void* allocate_mcb(int size)
{
    /*
     * https://stackoverflow.com/questions/2301556/in-gcc-style-extended-inline-asm-is-it-possible-to-output-a-virtualized-boole
   * */
    volatile void *buff;
    volatile char carry_flag = 0;
    asm volatile ("mov   $0x48, %%ah\n"
                  "int   $0x21\n"
                  : "+b"(size), "+a"(buff), "=@ccc" (carry_flag)
                  : 
                  : );
    if (carry_flag == 1) {
        print("Carry flag set\n$");
    }
    return buff;
}

int dosmain(void)
{
    volatile char input_buffer[INPUT_BUFSIZE];
    print("Hello, World!\n$");
    readline(input_buffer);
    print("\nReceived:\n$");
    print(input_buffer);
    print("\nEnding.\n$");
    volatile void* ptr = allocate_mcb(16);
    return 0;
}
