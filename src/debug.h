#ifndef DEBUG_H
#define DEBUG_H

#include <types.h>

#define DEBUG

#ifdef DEBUG
	#define DEBUG_INFO(...) debug_info(__func__, ##__VA_ARGS__)
	#define DEBUG_WARN(...) debug_warn(__func__, ##__VA_ARGS__)
	#define DEBUG_ERR(...) debug_err(__func__, ##__VA_ARGS__)
#else
	#define DEBUG_INFO(...)
	#define DEBUG_WARN(...)
	#define DEBUG_ERR(...)
#endif

int debug_info(char *fnt, char *msg, ...);
int debug_warn(char *fnt, char *msg, ...);
int debug_err(char *fnt, char *msg, ...);

#endif /*   DEBUG_H */
