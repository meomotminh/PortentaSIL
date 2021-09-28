// Data register (offset 0x00) data in (READ)/ out (WRITE)
// Flag register (offset 0x18) status of UART port
// TXFE = TX buffer empty, RXFF = RX buffer full, TXFF = TX buffer full, RXFE = RX buffer empty, BUSY = device busy


// bytes offset of UART registers from char base
#define UDR 0x00
#define UFR 0x18
#define TXFE 0x80
#define RXFE 0x40
#define TXFF 0x20
#define BUSY 0x08


typedef volatile struct uart{
    char *base;     // base address as char*
    int n;          // uart number 0 - 3
} UART;

UART uart[4];       // 4 UART structures

int uart_init()     // UART initialization function
{
    int i; UART *up;
    for (i=0; i<4; i++){
        // uart0 to uart2 are adjacent
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i*0x1000);
        up->n = i;
    }
    uart[3].base = (char *)(0x10009000);        // uart3 at 0x10009000
    return 0;
}

int ugetc(UART *up)         // input a char from UART pointed by up
{
    while (*(up->base+UFR) & RXFE); // loop if UFR is REFE
    return *(up->base+UDR);         // return a char in UDR 
}

int uputc(UART *up, char c)  // output a char to UART pointed by up
{
    while (*(up->base+UFR) & TXFF);     // loop if UFR is TXFF
    *(up->base+UDR) = c;             // write char to data register
    return 0;
}

int upgets(UART *up, char *s)           // input a string of chars
{
    while ((*s=ugetc(up)) != '\r'){
        uputc(up, *s);
        s++;
    }
    *s = 0;
    return 0;
}

int uprints(UART *up, char *s)      // output a string of chars
{
    while (*s)
        uputc(up, *s++);
    return 0;
}



