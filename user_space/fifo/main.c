#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "debug.h"

#define FIFO_PATH "/tmp/test_fifo"

void *read_fifo1(void *dummy)
{
    int fd = open(FIFO_PATH, O_RDONLY);
    int ret;
    char read_buf[1024];

    debug("fifo open for reading ok\n");
    while ((ret = read(fd, read_buf, 1024)) > 0)
    {
        debug_info("reader got: %s\n", read_buf);
    }
    close(fd);

    return NULL;
}

void *read_fifo2(void *_fd)
{
    int fd = dup((int)_fd);
    int ret;
    char read_buf[1024];

    if ((ret = read(fd, read_buf, 1024)) > 0)
    {
        debug_info("reader got: %s\n", read_buf);
    }
    close(fd);

    return NULL;
}


void test1()
{
    pthread_t tid;
    int fd;

    if (access(FIFO_PATH, F_OK) != 0)
    {
        mkfifo(FIFO_PATH, 0666);
    }

    pthread_create(&tid, NULL, read_fifo1, NULL);

    fd = open(FIFO_PATH, O_WRONLY);
    debug("fifo open for writing ok\n");
    write(fd, "111", 4);
    close(fd);

    pthread_join(tid, NULL);
}

void test2()
{
    pthread_t tid;
    int fd;

    fd = open(FIFO_PATH, O_RDWR);
    pthread_create(&tid, NULL, read_fifo2, (void *)fd);
    write(fd, "222", 4);
    sleep(1);
    close(fd);
    pthread_join(tid, NULL);
}


void test3()
{
    int fd;
    int ret;

    if (access(FIFO_PATH, F_OK) != 0)
    {
        mkfifo(FIFO_PATH, 0666);
    }

    if (fork() > 0)
    {
        fd = open(FIFO_PATH, O_WRONLY);
        write(fd, "333", 4);
        close(fd);
    }
    else
    {
        char read_buf[1024];
        fd = open(FIFO_PATH, O_RDONLY);
        if ((ret = read(fd, read_buf, 1024)) > 0)
        {
            debug_info("reader got: %s\n", read_buf);
        }
        close(fd);
    }
}



int main(void)
{
    test1();
    test2();
    test3();

    return 0;
}

