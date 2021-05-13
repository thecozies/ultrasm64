#ifndef S2D_BUFFER_H
#define S2D_BUFFER_H
#include <ultra64.h>

extern void s2d_print_deferred(int x, int y, int align, u8 alpha, const char *str);
extern void s2d_handle_deferred(void);
extern int s2d_charBuffer_index;

#endif