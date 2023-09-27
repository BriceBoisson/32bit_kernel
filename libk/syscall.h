#ifndef SYSCALL_H
#define SYSCALL_H

int __syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4);
int write(int fd, char *buf, int count);

#endif  /*  !SYSCALL_H */