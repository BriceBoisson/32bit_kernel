#ifndef LAUNCH_PROCESS_H
#define LAUNCH_PROCESS_H

#include <types.h>

void launch_process(u16 tss, int userland_stack, int userland_code, u16 userland_data);

#endif /*   !LAUNCH_PROCESS_H */