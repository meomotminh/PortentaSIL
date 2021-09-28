#
#arm-none-eabi-as -o ts.o ts.s               # assemble ts.s to ts.o
#arm-none-eabi-ld -T t.ld -o t.elf ts.o      # link ts.o to t.elf file
#arm-none-eabi-nm t.elf                      # show symbols in t.elf
#arm-none-eabi-objcopy -O binary t.elf t.bin # objcopy t.elf to t.bin
#

arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm image1 image1.o 
arm-none-eabi-objcopy -I binary -O elf32-littlearm -B arm image2 image2.o 

arm-none-eabi-as -mcpu=arm926ej-s ts.s -o ts.o 
arm-none-eabi-gcc -c -mcpu=arm926ej-s t.c -o t.o 
arm-none-eabi-ld -T t.ld ts.o t.o -o t.elf 
arm-none-eabi-objcopy -O binary t.elf t.bin 
echo ready to go?
read dummy

qemu-system-arm -M versatilepb -m 128 M -kernel t.bin -serial mon:stdio