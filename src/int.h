#ifndef INT_H
#define INT_H

void _asm_default_int(void);
void _asm_irq_0(void);
void _asm_irq_1(void);
int _asm_sycall_handler(int eax, int ebx);

#endif /*   !INT_H */
