#ifndef PLAT_H
#define PLAT_H

#include "plat/type.h"

#include <pthread.h>

typedef pthread_t sys_thread_t;
typedef void(*sys_thread_func_t)(void *);

typedef pthread_mutex_t sys_mutex_t;

sys_thread_t sys_thread_create(sys_thread_func_t entry, void* arg);

int32_t sys_mutex_init(sys_mutex_t *mutex);
int32_t sys_mutex_lock(sys_mutex_t *mutex);
int32_t sys_mutex_unlock(sys_mutex_t *mutex);
int32_t sys_mutex_destroy(sys_mutex_t *mutex);

int32_t sys_sleep(int ms);

#endif //PLAT_H


