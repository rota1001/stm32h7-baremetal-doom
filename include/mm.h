#include <stddef.h>
#include <stdint.h>

#define ALIGN 4
#define ALIGN_DOWN(size) ((size) & ~(ALIGN - 1))
#define ALIGN_UP(size) ALIGN_DOWN((size) + (ALIGN - 1))

typedef struct block {
    size_t size;
    size_t prev_size;
    char mem[0];
} block_t;

extern unsigned long _heap_start, _heap_end;

void mm_init(void);
