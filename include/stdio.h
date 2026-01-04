#pragma once
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <usart.h>

void putchar(char c);

void vprintf(const char *fmt, va_list arg);

void printf(const char *fmt, ...);

#define fprintf(ignore, fmt, ...) printf(fmt, ##__VA_ARGS__)

void snprintf(char *dst, unsigned long size, const char *fmt, ...);

int vsnprintf(char *dst, unsigned long size, const char *fmt, va_list arg);

#define FILE void

#define stdout NULL
#define stderr NULL

#define SEEK_END 0
#define SEEK_SET 0

FILE *fopen(const char *filename, const char *mode);
void fclose(FILE *fp);
