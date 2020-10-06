#!/bin/bash

for f in ./asm/*
do
    if \
        nasm -f elf64 $f -o ${f::-4}.o &&\
        yasm -f elf64 -g dwarf2 $f -o ${f::-4}.o
    then
        true
    else
        exit
    fi
done

if gcc -Wall -m64 -no-pie -gdwarf-2 -o ./filerm ./*.c ./asm/*.o
then
    rm -rf ./asm/*.o
    rm -rf ./*.o
    mkdir -p package/usr/sbin
    cp filerm package/usr/sbin
    dpkg-deb --build ./package filerm_c.deb
    rm -f ./filerm
    rm -rf ./package/usr
fi
