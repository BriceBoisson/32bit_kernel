#!/bin/sh

mkdir -p iso/boot/grub
echo "menuentry \"kernel\" {
	multiboot /kernel
}" > iso/boot/grub/grub.cfg

grub-mkrescue -o kernel.iso iso
