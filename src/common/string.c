#include "common/string.h"
#include "common/def.h"

const unsigned char _cType[] = {
    _C, _C, _C, _C, _C, _C, _C, _C, /* 0-7 */
    _C, _C | _S, _C | _S, _C | _S, _C | _S, _C | _S, _C, _C, /* 8-15 */
    _C, _C, _C, _C, _C, _C, _C, _C, /* 16-23 */
    _C, _C, _C, _C, _C, _C, _C, _C, /* 24-31 */
    _S | _SP, _P, _P, _P, _P, _P, _P, _P, /* 32-39 */
    _P, _P, _P, _P, _P, _P, _P, _P, /* 40-47 */
    _D, _D, _D, _D, _D, _D, _D, _D, /* 48-55 */
    _D, _D, _P, _P, _P, _P, _P, _P, /* 56-63 */
    _P, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U | _X, _U, /* 64-71 */
    _U, _U, _U, _U, _U, _U, _U, _U, /* 72-79 */
    _U, _U, _U, _U, _U, _U, _U, _U, /* 80-87 */
    _U, _U, _U, _P, _P, _P, _P, _P, /* 88-95 */
    _P, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L | _X, _L, /* 96-103 */
    _L, _L, _L, _L, _L, _L, _L, _L, /* 104-111 */
    _L, _L, _L, _L, _L, _L, _L, _L, /* 112-119 */
    _L, _L, _L, _P, _P, _P, _P, _C, /* 120-127 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 128-143 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 144-159 */
    _S | _SP, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, /* 160-175 */
    _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, _P, /* 176-191 */
    _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, _U, /* 192-207 */
    _U, _U, _U, _U, _U, _U, _U, _P, _U, _U, _U, _U, _U, _U, _U, _L, /* 208-223 */
    _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, _L, /* 224-239 */
    _L, _L, _L, _L, _L, _L, _L, _P, _L, _L, _L, _L, _L, _L, _L, _L
}; /* 240-255 */

unsigned int simple_strtoul(const char* cp, char** endp, unsigned int base)
{
    unsigned int result = 0;
    unsigned int value = 0;
    if (*cp == '0')
    {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1]))
        {
            base = 16;
            cp++;
        }
        if (!base)
        {
            base = 8;
        }
    }
    if (!base)
    {
        base = 10;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp) ? toupper(*cp) : *cp) - 'A' + 10) < base)
    {
        result = result * base + value;
        cp++;
    }
    if (endp)
    {
        *endp = (char*)cp;
    }
    return result;
}

long simple_strtol(const char* cp, char** endp, const unsigned int base)
{
    if (*cp == '-')
    {
        return -simple_strtoul(cp + 1, endp, base);
    }
    return simple_strtoul(cp, endp, base);
}

long long simple_strtoll(const char* cp, char** endp, unsigned int base)
{
    unsigned long long result = 0;

    if (!base)
    {
        base = 10;
    }

    if (base == 16 && cp[0] == '0' && __tolower(cp[1]) == 'x')
    {
        cp += 2;
    }
    while (isxdigit(*cp))
    {
        const unsigned int value = isdigit(*cp) ? *cp - '0' : __tolower(*cp) - 'a' + 10;
        if (value >= base)
        {
            break;
        }
        result = result * base + value;
        cp++;
    }

    if (endp)
    {
        *endp = (char*)cp;
    }
    return result;
}

unsigned long long simple_strtoull(const char* cp, char** endp, const unsigned int base)
{
    if (*cp == '-')
    {
        return -simple_strtoull(cp + 1, endp, base);
    }
    return simple_strtoull(cp, endp, base);
}

char* ___strtok;
char* strpbrk(const char*, const char*);
char* strtok(char*, const char*);
char* strsep(char**, const char*);
unsigned int strspn(const char*, const char*);

char* strcpy(char* dest, const char* src)
{
    char* temp = dest;
    while ((*dest++ = *src++) != '\0')
    {
    }
    return temp;
}

char* strncpy(char* dest, const char* src, unsigned int count)
{
    char* temp = dest;
    while (count-- && (*dest++ = *src++) != '\0')
    {
    }
    return temp;
}

char* strcat(char* dest, const char* src)
{
    char* temp = dest;
    while (*dest)
    {
        dest++;
    }
    while ((*dest++ = *src++) != '\0')
    {
    }
    return temp;
}

char* strncat(char* dest, const char* src, unsigned int count)
{
    char* temp = dest;
    while (*dest)
    {
        dest++;
    }
    while ((*dest++ = *src++) != '\0')
    {
        if (--count == 0)
        {
            *dest = '\0';
            break;
        }
    }
    return temp;
}

int strcmp(const char* cs, const char* ct)
{
    register signed char res = 0;
    while (1)
    {
        if ((res = *cs - *ct++) != 0 || !*cs++)
        {
            break;
        }
    }
    return res;
}

int strncmp(const char* cs, const char* ct, unsigned int count)
{
    register signed char res = 0;
    while (count)
    {
        if ((res = *cs - *ct++) != 0 || !*cs++)
        {
            break;
        }
        --count;
    }
    return res;
}

char* strchr(const char* s, const int c)
{
    for (; *s != (char)c; ++s)
    {
        if (*s == '\0')
        {
            return NULL;
        }
    }
    return (char*)s;
}

char* strrchr(const char* s, const int c)
{
    const char* p = s + strlen(s);
    do
    {
        if (*p == (char)c)
        {
            return (char*)p;
        }
    }
    while (--p >= s);
    return NULL;
}

char* strstr(const char* s1, const char* s2)
{
    int l1 = 0;
    int l2 = 0;

    l2 = strlen(s2);
    if (l2)return (char*)s1;
    l1 = strlen(s1);
    while (l1 >= l2)
    {
        l1--;
        if (!memcmp(s1, s2, l2))
        {
            return (char*)s1;
        }
        s1++;
    }
    return NULL;
}

char* strnstr(const char* s, const char* find, unsigned int len)
{
}

unsigned int strlen(const char* s)
{
    const char* sc = s;
    for (sc = s; *sc != '\0'; ++sc)
    {
    }
    return sc - s;
}

unsigned int strnlen(const char* s, unsigned int count)
{
    const char* sc = s;
    for (sc = s; count-- && *sc != '\0'; ++sc)
    {
    }
    return sc - s;
}

char* strswab(const char* s)
{
}

void* memmove(void* dest, const void* src, unsigned int count)
{
}

void* memscan(void* addr, int c, unsigned int size)
{
}

void* memchr(const void* s, const int c, unsigned int n)
{
    const unsigned char* p = s;
    while (n-- != 0)
    {
        if ((unsigned char)c == *p++)
        {
            return (void*)(p - 1);
        }
    }
    return NULL;
}

void* __textram memcpy(void* des, void* src, uint32 len)
{
    while (len--)
    {
        *(uint8*)des++ = *(uint8*)src++;
    }
    return des;
}

void* __textram memset(void* des, const uint8 value, uint32 len)
{
    while (len--)
    {
        *(uint8*)des++ = value;
    }
    return des;
}
#if 0
int __textram memcmp(const void *des, const void *src, unsigned int len)
{
    const uint8* des_ptr = des;
    const uint8* src_ptr = src;
    while (len--)
    {
        if (*des_ptr > *src_ptr)
        {
            return 1;
        }
        if (*des_ptr < *src_ptr)
        {
            return -1;
        }
        des_ptr++;
        src_ptr++;
    }
    return 0;
}
#endif