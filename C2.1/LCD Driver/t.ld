ENTRY (reset_start)
SECTIONS
{
    .= 0x10000;
    .text= { ts.o *( .text) }
    .data= { *(.data) }
    .bss= { *(.bss) }
    .data= { *(image.o) } /* include image.o as a data section*/
    /* stack areas */ 
}