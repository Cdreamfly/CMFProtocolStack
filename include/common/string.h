#ifndef STRING_H
#define STRING_H

#include "common/type.h"

#define _U	0x01	/* upper：大写字母 */
#define _L	0x02	/* lower：小写字母 */
#define _D	0x04	/* digit：数字 */
#define _C	0x08	/* cntrl：控制字符（如换行、制表符） */
#define _P	0x10	/* punct：标点符号 */
#define _S	0x20	/* white space：空白符（空格/换行/制表符） */
#define _X	0x40	/* hex digit：十六进制字符（A-F/a-f） */
#define _SP	0x80	/* hard space：硬空格（ASCII 0x20） */

extern const unsigned char _cType[];

#define __ismask(x) (_cType[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)	((__ismask(c)&(_P)) != 0)
#define isspace(c)	((__ismask(c)&(_S)) != 0)
#define isupper(c)	((__ismask(c)&(_U)) != 0)
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

static inline unsigned char __tolower(unsigned char c)
{
    if (isupper(c)) c -= 'A' - 'a';
    return c;
}

static inline unsigned char __toupper(unsigned char c)
{
    if (islower(c)) c -= 'a' - 'A';
    return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

extern unsigned int simple_strtoul(const char* cp, char** endp, unsigned int base);
extern long simple_strtol(const char* cp, char** endp, unsigned int base);
extern long long simple_strtoll(const char* cp, char** endp, unsigned int base);
extern unsigned long long simple_strtoull(const char* cp, char** endp, unsigned int base);

extern char* ___strtok;
extern char* strpbrk(const char*, const char*);
extern char*strtok(char*, const char*);
extern char* strsep(char**, const char*);
extern unsigned int strspn(const char*, const char*);
extern char* strcpy(char*, const char*);
extern char* strncpy(char*, const char*, unsigned int);
extern char* strcat(char*, const char*);
extern char* strncat(char*, const char*, unsigned int);
extern int strcmp(const char*, const char*);
extern int strncmp(const char*, const char*, unsigned int);
extern char* strchr(const char*, int);
extern char* strrchr(const char*, int);
extern char* strstr(const char*, const char*);
extern char* strnstr(const char*, const char*, unsigned int);
extern unsigned int strlen(const char*);
extern unsigned int strnlen(const char*, unsigned int);
extern char* strswab(const char*);

extern void* memmove(void*, const void*, unsigned int);
extern void* memscan(void*, int, unsigned int);
extern void* memchr(const void*, int, unsigned int);
extern void* __textram memcpy(void* des, void* src, unsigned int len);
extern void* __textram memset(void* des, unsigned char value, unsigned int len);
extern int __textram memcmp(void* des, const void* src, unsigned int len);

#endif //STRING_H
