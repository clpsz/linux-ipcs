#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "debug.h"



#if defined(USING_MUTEX)
pthread_mutex_t mutex;
#endif



void print0(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "0");
    }
    fprintf(stderr, "\n");
}

void print1(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "1");
    }
    fprintf(stderr, "\n");
}

void *thread0(void *cmd)
{
    while(1)
    {
#if defined(USING_MUTEX)
        pthread_mutex_lock(&mutex);
        print0(100);
        pthread_mutex_unlock(&mutex);
#else
        print0(100);
#endif
    }

    return NULL;
}

void *thread1(void *cmd)
{
    while(1)
    {
#if defined(USING_MUTEX)
        pthread_mutex_lock(&mutex);
        print1(100);
        pthread_mutex_unlock(&mutex);
#else
        print1(100);
#endif
    }

    return NULL;
}



int main(void)
{
    pthread_t tid0, tid1;
    int ret;

#if defined(USING_MUTEX)
    pthread_mutex_init(&mutex, NULL);
#endif

    ret = pthread_create(&tid0, NULL, thread0, NULL);
    if (ret != 0)
    {
        perror("pthread_create");
    }
    ret = pthread_create(&tid1, NULL, thread1, NULL);
    if (ret != 0)
    {
        perror("pthread_create");
    }

    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);

    exit(0);
}

