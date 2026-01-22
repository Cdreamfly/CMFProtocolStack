#include "plat/sys_plat.h"

#include <stdlib.h>
#include <unistd.h>

sys_thread_t sys_thread_create(const sys_thread_func_t entry, void * arg)
{
    pthread_t thread = 0;
    return pthread_create(&thread, NULL, (void* (*)(void * arg))entry, arg);;
}

int32_t sys_mutex_init(sys_mutex_t *mutex)
{
    mutex = alloca(sizeof(sys_mutex_t));
    return pthread_mutex_init(mutex, NULL);
}

int32_t sys_mutex_lock(sys_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

int32_t sys_mutex_unlock(sys_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

int32_t sys_mutex_destroy(sys_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}

int32_t sys_sleep(const int ms)
{
    return usleep(ms * 1000);
}