#!/bin/bash

if [ -s sysroot/boot/moonlightkrnld ]
	then
		mkdir -p iso/Debug/boot/grub
		cp kernel/grub.cfg iso/Debug/boot/grub/grub.cfg
		cp sysroot/boot/moonlightkrnld iso/Debug/boot/moonlightkrnl
		grub-mkrescue -o iso/moonlightd.iso iso/Debug
fi


if [ -s sysroot/boot/moonlightkrnl ]
	then
		mkdir -p iso/Release/boot/grub
		cp kernel/grub.cfg iso/Release/boot/grub/grub.cfg
		cp sysroot/boot/moonlightkrnl iso/Release/boot/moonlightkrnl
		grub-mkrescue -o iso/moonlight.iso iso/Release
fi