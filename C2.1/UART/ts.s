c:
int v[] = {1,2,3,4,5,6,7,8,9,10};   // data array
int sum;

#include "string.c"     // contains strlen(), strcmp(), etc.
#include "uart.c"       // UART driver code file

int main()
{
    int i;
    char string[64];
    UART *up;
    uart_init();    // initialize UARTs
    up = &uart[0];  // test UART0
    uprints(up, "Enter lines from serial terminal 0\n\r");
    while (1){
        ugets(up, string);
        uprints(up, "   ");
        uprints(up, string);
        uprints(up, "\n\r");
        if (strcmp(string, "end")==0)
            break;
    }
    uprints(up, "Complete sum of array:\n\r");
    sum = 0;
    for (i=0; i<10;i++){
        sum += v[i];
    }

    uprints(up, "sum = ");
    uputc(up, (sum/10)+'0'); uputc(up, (sum%10)+'0');
    uprints(up, "\n\rEND OF RUN\n\r");
}

asm:
    .global start, stack_top
start:
    ldr sp, =stack_top  // set SVC mode stack pointer
    bl main             // call main() in C
    b .                 // if main() returns, just loop


