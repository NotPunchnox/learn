nasm -f elf32 main.asm -o main.o
ld -m elf_i386 main.o -o main
rm main.o
./main
