.extern isr_default_int, isr_clock_int, isr_kbd_int, syscall_handler, isr_page_fault
.global _asm_default_int, _asm_irq_0, _asm_irq_1, _asm_sycall_handler, _asm_page_fault

_asm_default_int:
	pushal
	push %ds
	push %es
	push %fs
	push %gs 
	push %ebx
	mov $0x10,%bx
	mov %bx,%ds
	pop %ebx
    call isr_default_int
    pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
    iret

_asm_page_fault:
	pushal
	push %ds
	push %es
	push %fs
	push %gs 
	push %ebx
	mov $0x10,%bx
	mov %bx,%ds
	pop %ebx
    call isr_page_fault
    pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
    iret

_asm_irq_0:
	pushal
	push %ds
	push %es
	push %fs
	push %gs 
	push %ebx
	mov $0x10,%bx
	mov %bx,%ds
	pop %ebx
    call isr_clock_int
    movb $0x20,%al
    outb %al,$0x20
    pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
    iret

_asm_irq_1:
	pushal
	push %ds
	push %es
	push %fs
	push %gs 
	push %ebx
	mov $0x10,%bx
	mov %bx,%ds
	pop %ebx
    call isr_kbd_int
    movb $0x20,%al
    outb %al,$0x20
    pop %gs
	pop %fs
	pop %es
	pop %ds
	popal
    iret

_asm_sycall_handler:
	pushal
	push %ds
	push %es
	push %fs
	push %gs 
	push %ebx
	mov $0x10,%bx
	mov %bx,%ds
	pop %ebx
	push %ebx
	push %eax
    call syscall_handler
	pop %ebx
	pop %ebx
    pop %gs
	pop %fs
	pop %es
	pop %ds
	mov %eax,28(%esp)
	popal
    iret
