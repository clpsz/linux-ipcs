#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"


void _daemon_printf(const char *format, ...)
{
    va_list vlist;
    char buf[512];
    char buf1[512];


    va_start(vlist, format);
    vsprintf(buf, format, vlist);
    va_end(vlist);

    sprintf(buf1, "echo %s > "CONSOLE_DEVICE" 2>&1", buf);                                                                                                          
    system(buf1);
}


#define _DEBUG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
void _hex_dump(char* buf, int len)
{
    int i;
    _DEBUG("**********************start**********************\n");
    for(i = 0; i < len; i++)
    {
        _DEBUG("%02X ", (unsigned char)buf[i]);
        if(i%8 == 7)
        {
            _DEBUG(" ");
        }
        if(i%16 == 15)
        {
            _DEBUG("\n");
        }
    }
    if (! ((--i)%16 == 15))
    {
        _DEBUG("\n");                                                                                                                       
    }
    _DEBUG("********************** end **********************\n");
}


/*
static void test_hexdump()
{
    char buf[128];

    memset(buf, 0, 128);

    buf[0] = 'Z';
    buf[125] = 'P';
    buf[126] = 'P';

    HEX_DUMP(buf, 127);
}

static void test_daemon()
{
    DAEMON_PRINTF("%d %s\n", 33, "kkk");
}
*/
