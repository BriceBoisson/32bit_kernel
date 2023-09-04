#ifndef PIC_CONTROLER_H
#define PIC_CONTROLER_H

void PIC_sendEOI(unsigned char irq);
void pic_init(void);

#endif  /* !PIC_CONTROLER_H */