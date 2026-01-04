#include <test.h>

#define PASS_COLOR "\033[32m"
#define FAIL_COLOR "\033[31m"
#define RESET_COLOR "\033[0m"

#define LOG_PASS(msg) printf(PASS_COLOR "[+] PASS: %s" RESET_COLOR "\n", msg)
#define LOG_FAIL(msg) printf(FAIL_COLOR "[!] FAIL: %s" RESET_COLOR "\n", msg)

int abs(int j);
double fabs(double x);
int toupper(int c);
int tolower(int c);
int isspace(int c);
int memcmp(const void *s1, const void *s2, size_t n);
int strcasecmp(const char *s1, const char *s2);
int atoi(const char *s);


void test_string_ops(void)
{
    char buf[32];

    if (strlen("hello") != 5 || strlen("") != 0)
        LOG_FAIL("strlen basic");
    else
        LOG_PASS("strlen basic");

    strcpy(buf, "test");
    if (strcmp(buf, "test") != 0)
        LOG_FAIL("strcpy");
    else
        LOG_PASS("strcpy");

    if (strcmp("abc", "abc") != 0 || strcmp("", "") != 0)
        LOG_FAIL("strcmp equal");
    else if (strcmp("abc", "abd") >= 0 || strcmp("abc", "abcd") >= 0)
        LOG_FAIL("strcmp less");
    else if (strcmp("abd", "abc") <= 0 || strcmp("abcd", "abc") <= 0)
        LOG_FAIL("strcmp greater");
    else
        LOG_PASS("strcmp");

    if (strncmp("abc", "abd", 2) != 0)
        LOG_FAIL("strncmp limit");
    else if (strncmp("abc", "abd", 3) >= 0)
        LOG_FAIL("strncmp diff");
    else
        LOG_PASS("strncmp");

    if (strcasecmp("AbC", "aBc") != 0 || strcasecmp("Z", "a") <= 0)
        LOG_FAIL("strcasecmp");
    else
        LOG_PASS("strcasecmp");
}

void test_memory_ops(void)
{
    char buf[20];
    char overlap[] = "123456789";

    memset(buf, 'A', 5);
    buf[5] = 0;
    if (strcmp(buf, "AAAAA") != 0)
        LOG_FAIL("memset");
    else
        LOG_PASS("memset");

    memcpy(buf, "12345", 6);
    if (strcmp(buf, "12345") != 0)
        LOG_FAIL("memcpy");
    else
        LOG_PASS("memcpy");

    strcpy(overlap, "123456789");
    memmove(overlap + 1, overlap, 3);
    if (strncmp(overlap, "11235", 5) != 0)
        LOG_FAIL("memmove overlap (src < dst)");
    else
        LOG_PASS("memmove overlap (src < dst)");

    strcpy(overlap, "123456789");
    memmove(overlap, overlap + 1, 3);
    if (strncmp(overlap, "23445", 5) != 0)
        LOG_FAIL("memmove overlap (dst < src)");
    else
        LOG_PASS("memmove overlap (dst < src)");
}

void test_ctype_ops(void)
{
    if (toupper('a') != 'A' || toupper('1') != '1')
        LOG_FAIL("toupper");
    else
        LOG_PASS("toupper");

    if (tolower('A') != 'a' || tolower('[') != '[')
        LOG_FAIL("tolower");
    else
        LOG_PASS("tolower");

    if (!isspace(' ') || !isspace('\t') || !isspace('\n') || isspace('A'))
        LOG_FAIL("isspace");
    else
        LOG_PASS("isspace");
}

void test_conversion(void)
{
    if (atoi("123") != 123)
        LOG_FAIL("atoi positive");
    else if (atoi("-456") != -456)
        LOG_FAIL("atoi negative");
    else if (atoi("0") != 0)
        LOG_FAIL("atoi zero");
    else
        LOG_PASS("atoi");
}

void test_utils_ops(void)
{
    if (abs(-5) != 5 || abs(5) != 5 || abs(0) != 0)
        LOG_FAIL("abs");
    else
        LOG_PASS("abs");

    if (fabs(-5.5) != 5.5 || fabs(5.5) != 5.5)
        LOG_FAIL("fabs");
    else
        LOG_PASS("fabs");
}

void test_snprintf_ops(void)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "%d %d", 123, -123);
    if (strcmp(buf, "123 -123") != 0)
        LOG_FAIL("snprintf int");
    else
        LOG_PASS("snprintf int");

    snprintf(buf, sizeof(buf), "%x", 0x1A2b);
    if (strcasecmp(buf, "00001a2b") != 0)
        LOG_FAIL("snprintf hex");
    else
        LOG_PASS("snprintf hex");

    snprintf(buf, sizeof(buf), "%u", 2147483648U);
    if (strcmp(buf, "2147483648") != 0)
        LOG_FAIL("snprintf unsigned");
    else
        LOG_PASS("snprintf unsigned");

    memset(buf, 0xFF, sizeof(buf));
    snprintf(buf, 4, "123456");
    if (buf[3] != '\0' || strcmp(buf, "123") != 0)
        LOG_FAIL("snprintf truncation");
    else
        LOG_PASS("snprintf truncation");
}

void test_mm(void)
{
    char *arr[256];

    for (int i = 0; i < 256; i++)
        arr[i] = 0;
    int seed = 0xdead;
    int a = 123, b = 321;
    for (int i = 0; i < 10000; i++) {
        seed = (a * seed + b) & 255;
        if (arr[seed]) {
            for (int i = 0; i < (seed & 31); i++) {
                if (arr[seed][i] != (seed & 31)) {
                    LOG_FAIL("mm write integrity failed");
                    return;
                }
            }
            free(arr[seed]);
            arr[seed] = 0;
        } else {
            arr[seed] = malloc(seed & 31);
            for (int i = 0; i < (seed & 31); i++) {
                arr[seed][i] = (seed & 31);
            }
        }
    }
    for (seed = 0; seed < 256; seed++) {
        if (!arr[seed])
            continue;
        for (int i = 0; i < (seed & 31); i++) {
            if (arr[seed][i] != (seed & 31)) {
                LOG_FAIL("write integrity failed");
                return;
            }
        }
        free(arr[seed]);
        arr[seed] = 0;
    }
    LOG_PASS("memory test");
}

void test_mm_merging(void) {}

void test_libc(void)
{
    test_string_ops();
    test_ctype_ops();
    test_conversion();
    test_utils_ops();
    test_snprintf_ops();
    test_mm();
    printf(PASS_COLOR "[+] ALL TESTS DONE!" RESET_COLOR "\n");
}
