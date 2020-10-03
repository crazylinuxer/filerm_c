#!/bin/bash

for f in ./asm/*
do
    yasm -f elf64 -g dwarf2 $f -o ${f::-4}.o
done

gcc -Wall -m64 -no-pie -gdwarf-2 -o ./filerm ./*.c ./asm/*.o

rm -rf ./asm/*.o
