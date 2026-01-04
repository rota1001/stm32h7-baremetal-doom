#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void putchar(char c)
{
    if (c == '\n')
        usart_putc('\r');
    usart_putc(c);
}

void vprintf(const char *fmt, va_list arg)
{
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            putchar(*fmt);
            continue;
        }
        fmt++;
        switch (*fmt) {
        case '\0':
            goto DONE;
        case 's': {
            const char *s = va_arg(arg, const char *);
            while (*s)
                putchar(*s++);
            break;
        }
        case 'c': {
            int c = va_arg(arg, int);
            putchar(c);
            break;
        }
        case 'd': {
            int x = va_arg(arg, int);
            int i = 0;
            if (x < 0) {
                putchar('-');
                x = -x;
            }
            print_uint(x);
            break;
        }
        case 'u':
            unsigned int y = va_arg(arg, unsigned int);
            print_uint(y);
            break;
        case 'x':
            unsigned int z = va_arg(arg, unsigned int);
            for (int i = 7; i >= 0; i--) {
                putchar("0123456789abcdef"[(z >> (i << 2)) & 0xf]);
            }
            break;
        }
    }
DONE:
}

void printf(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}

void snprintf(char *dst, unsigned long size, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vsnprintf(dst, size, fmt, arg);
    va_end(arg);
}

int vsnprintf(char *dst, unsigned long size, const char *fmt, va_list arg)
{
    unsigned long cnt = 0;
    for (; *fmt; fmt++) {
        if (cnt == size)
            goto DONE;
        if (*fmt != '%') {
            dst[cnt++] = *fmt;
            continue;
        }
        fmt++;
        switch (*fmt) {
        case '\0':
            goto DONE;
        case 's': {
            const char *s = va_arg(arg, const char *);
            while (*s && cnt < size)
                dst[cnt++] = *s++;

            if (cnt == size)
                goto DONE;
            break;
        }
        case 'c': {
            int c = va_arg(arg, int);
            dst[cnt++] = c;
            break;
        }
        case 'd': {
            int x = va_arg(arg, int);
            if (!x) {
                dst[cnt++] = '0';
                continue;
            }
            if (x < 0) {
                dst[cnt++] = '-';
                x = -x;
            }
            char buf[12];
            int i = 0;
            while (x > 0) {
                buf[i++] = (x % 10) + '0';
                x /= 10;
            }
            while (i && cnt < size) {
                dst[cnt++] = buf[--i];
            }
            break;
        }
        case 'u': {
            unsigned int x = va_arg(arg, unsigned int);
            if (!x) {
                dst[cnt++] = '0';
                continue;
            }
            char buf[12];
            int i = 0;
            while (x > 0) {
                buf[i++] = (x % 10) + '0';
                x /= 10;
            }
            while (i && cnt < size) {
                dst[cnt++] = buf[--i];
            }
            break;
        }
        case 'x':
            unsigned int z = va_arg(arg, unsigned int);
            for (int i = 7; i >= 0 && cnt < size; i--)
                dst[cnt++] = "0123456789abcdef"[(z >> (i << 2)) & 0xf];
            break;
        }
    }
    if (!(*fmt) && cnt < size) {
        dst[cnt++] = *fmt;
    }
DONE:
    dst[size - 1] = 0;
}


int sscanf(const char *str, const char *format, ...)
{
    printf("sscanf not implemented");
    while (1)
        ;
    return 0;
}

FILE *fopen(const char *filename, const char *mode)
{
    printf("fopen not implemented");
    while (1)
        ;
    return 0;
}

void fclose(FILE *fp)
{
    printf("fclose not implemented");
    while (1)
        ;
}

int fseek(FILE *stream, long offset, int whence)
{
    printf("fseek not implemented");
    while (1)
        ;
    return -1;
}

long ftell(FILE *stream)
{
    printf("ftell not implemented");
    while (1)
        ;
    return -1;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    printf("fread not implemented");
    while (1)
        ;
    return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    printf("fwrite not implemented");
    while (1)
        ;
    return 0;
}

int fflush(FILE *stream)
{
    return 0;
}

int remove(const char *pathname)
{
    printf("remove not implemented");
    while (1)
        ;
    return -1;
}

int rename(const char *oldpath, const char *newpath)
{
    printf("rename not implemented");
    while (1)
        ;
    return -1;
}

int mkdir(const char *pathname, size_t mode)
{
    printf("mkdir not implemented");
    while (1)
        ;
    return -1;
}



int puts(const char *s)
{
    printf("%s\n", s);
    return 0;
}


void *calloc(size_t nmemb, size_t size)
{
    size_t total = nmemb * size;
    void *p = malloc(total);
    if (p)
        memset(p, 0, total);
    return p;
}

void *realloc(void *ptr, size_t size)
{
    printf("realloc not implemented");
    while (1)
        ;
    return 0;
}


void exit(int status)
{
    printf("EXIT called with status %d", status);
    while (1)
        ;
}

int system(const char *command)
{
    printf("Do not call system\n");
    while (1)
        ;
    return -1;
}

int *__errno(void)
{
    printf("__errno not implemented");
    while (1)
        ;
    return 0;
}


int *__errno_location(void)
{
    printf("__errno_location not implemented");
    while (1)
        ;
    return 0;
}

void *memset(void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char *) s;
    while (n--)
        *p++ = (unsigned char) c;
    return s;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    char *d = (char *) dst;
    const char *s = (const char *) src;
    while (n--)
        *d++ = *s++;
    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    char *d = (char *) dst;
    const char *s = (const char *) src;
    if (d < s) {
        while (n--)
            *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--)
            *--d = *--s;
    }
    return dst;
}

unsigned long strlen(const char *s)
{
    unsigned long len = 0;
    while (*s++)
        len++;
    return len;
}

char *strcpy(char *dest, const char *src)
{
    char *tmp = dest;
    while (*dest++ = *src++)
        ;
    return tmp;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    return 0;
    char *tmp = dst;
    while (n--) {
        if (!(*tmp = *src))
            break;
        dst++;
        src++;
    }
    return tmp;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

int strncmp(const char *s1, const char *s2, unsigned long n)
{
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return 0;
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

int strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && (tolower(*s1) == tolower(*s2))) {
        s1++;
        s2++;
    }
    return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    printf("strncasecmp not implemented");
    while (1)
        ;
    return 0;
    if (n == 0)
        return 0;
    while (n-- != 0 && tolower(*s1) == tolower(*s2)) {
        if (n == 0 || *s1 == '\0' || *s2 == '\0')
            break;
        s1++;
        s2++;
    }
    return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}

char *strchr(const char *s, int c)
{
    printf("strchr not implemented");
    while (1)
        ;
    return 0;
    while (*s != (char) c) {
        if (!*s++)
            return 0;
    }
    return (char *) s;
}

char *strrchr(const char *s, int c)
{
    printf("strrchr not implemented");
    while (1)
        ;
    return 0;
    const char *last = 0;
    do {
        if (*s == (char) c)
            last = s;
    } while (*s++);
    return (char *) last;
}

char *strstr(const char *haystack, const char *needle)
{
    printf("strstr not implemented");
    while (1)
        ;
    return 0;
    size_t n = strlen(needle);
    while (*haystack) {
        if (!memcmp(haystack, needle, n))
            return (char *) haystack;
        haystack++;
    }
    return 0;
}

char *strdup(const char *s)
{
    printf("strdup not implemented");
    while (1)
        ;
    return 0;
    size_t len = strlen(s) + 1;
    void *new = malloc(len);
    if (new == 0)
        return 0;
    return (char *) memcpy(new, s, len);
}

int atoi(const char *s)
{
    int sign = 1;
    int res = 0;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s) {
        res = res * 10 + *s - '0';
        s++;
    }
    return sign * res;
}

double atof(const char *ptr)
{
    if (*ptr)
        printf("atof not implemented");
    while (1)
        ;
    return 0;
}

int abs(int j)
{
    return (j < 0) ? -j : j;
}

double fabs(double x)
{
    return __builtin_fabs(x);
}

int toupper(int c)
{
    if (c < 'a' || c > 'z')
        return c;
    return c & ~32;
}

int tolower(int c)
{
    if (c < 'A' || c > 'Z')
        return c;
    return c | 32;
}

int isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
            c == '\r');
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *x = s1;
    const unsigned char *y = s2;
    while (n && (*x++ == *y++))
        n--;

    if (!n)
        return 0;
    return *(const unsigned char *) x - *(const unsigned char *) y;
}
