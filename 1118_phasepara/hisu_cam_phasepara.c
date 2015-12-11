#include "hi_type.h"
#include <assert.h>
#include <string.h>
#define BUF_LENGTH       64
#define ARRAY_LENGTH       32
#define CMD_ARG_MAX       6

typedef struct hiRECT_S
{
    int s32X;
    int s32Y;
    int s32Width;
    int s32Height;
} HI_RECT_S;

HI_S32 ipanel_porting_phase_parameter(const char *buf, char *cmd, char *port, HI_RECT_S *rect, char *ipaddr)
{
    if(!buf || !cmd || !port || !rect || !ipaddr)
    {
        return HI_FAILURE;
    }
    char srcbuf[BUF_LENGTH];
    char *argv[CMD_ARG_MAX];
    char *parabuf[ARRAY_LENGTH] = {NULL};
    char *desbuf;
    char *tmp = srcbuf;
    char *saveptr = srcbuf;
    int i = 0;

    /***************通过空格解析命令行*****************/
    strcpy(srcbuf, buf);
    while(*tmp != '\0')
    {
        if(*tmp == ' ')
        {
            *tmp = '\0';
            argv[i] = saveptr;
            i++;
            tmp++;
            saveptr = tmp;
        }
        else
        {
            ++tmp;
        }
    }
    argv[i] = saveptr;

    /***************通过特殊字符解析命令块*****************/
    strcpy(cmd, argv[0]);
    if(strchr(argv[i], '.'))//包含ipaddr
    {
        char *delim = ":";
        tmp = argv[1];
        i = 0;

        while(desbuf = strtok(tmp, delim))
        {
            tmp = NULL;
            parabuf[i] = desbuf;
            i++;
        }

        strcpy(ipaddr, parabuf[0]);
        *port = strtol(parabuf[1],NULL,10);
    }
    else if(strchr(argv[i], ','))//包含VO坐标
    {
        char *delim = ",";
        *port = strtol(argv[1],NULL,10);
        tmp = argv[2];
        i = 0;

        while(desbuf = strtok(tmp, delim))
        {
            tmp = NULL;
            parabuf[i] = desbuf;
            i++;
        }

        rect->s32X = strtol(parabuf[0],NULL,10);
        rect->s32Y = strtol(parabuf[1],NULL,10);
        rect->s32Width = strtol(parabuf[2],NULL,10);
        rect->s32Height = strtol(parabuf[3],NULL,10);
    }
    else
    {
        printf("Phase parameter error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
