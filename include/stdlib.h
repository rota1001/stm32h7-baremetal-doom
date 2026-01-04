#pragma once
#include <stddef.h>

void *malloc(size_t size);

void *realloc(void *ptr, size_t size);


int free(void *ptr);

void *memset(void *s, int c, size_t n);

void *memcpy(void *dst, const void *src, size_t n);

void *memmove(void *dst, const void *src, size_t n);

void exit(int err);
