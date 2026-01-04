#pragma once
#include <stdlib.h>

int atoi(const char *s);

double atof(const char *ptr);

unsigned long strlen(const char *s);

char *strdup(const char *s);

char *strcpy(char *dest, const char *src);

int strcmp(const char *s1, const char *s2);

int strncmp(const char *s1, const char *s2, unsigned long size);

int strcasecmp(const char *s1, const char *s2);

unsigned long strlen(const char *s);

char *strdup(const char *s);

int memcmp(const void *s1, const void *s2, size_t n);

char *strncpy(char *dst, const char *src, size_t n);

int toupper(int c);

int tolower(int c);

int isspace(int c);
