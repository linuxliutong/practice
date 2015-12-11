/**
 * 实现向AU9540模块收发串口数据
 * 为了程序的可移植性，此文件的API和手持POS里串口部分API保持一致
 *
 * Greate Date:2013.1.4
 */
#include <stdio.h>      /*标准输入输出定义*/
#include <string.h>
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys types.h="">  /*数据类型，比如一些XXX_t的那种*/
#include <sys stat.h="">   /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/

#define UART_DEV    "/dev/ttyS0"
#define DELAY_TIME  50  /*延时时间50MS*/
#define M_Delay(mSec)   usleep(mSec*1000)   /*MS延时*/
static int g_UartFd = 0;    // 保存串口打开的文件句柄

/**
 * 功 能：设置串口通信速率
 * 输入参数：port->打开串口的文件句柄   speed->串口速度
 * 输出参数：
 * 返回值： 一直成功
 * 开发人员：
 * 修改记录：Lzy 2013-1-14
 */
void au_appuart_set_speed(unsigned int port, int speed)
{
    getc();
    int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B600, B300, };
    int name_arr[] = { 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600, 300, };
    int i, status, fd;
    struct termios Opt;

    fd = port;
    tcgetattr(fd, &Opt);

    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if (speed == name_arr[i])
        {
            /**
             * tcflush函数刷清(抛弃)输入缓存(终端驱动程序已接收到，但用户程序尚未读)或输出缓存(用户程序已经写，但尚未发送)。
             * queue参数应是下列三个常数之一：
             * TCIFLUSH刷清输入队列。
             * TCOFLUSH刷清输出队列。
             * TCIOFLUSH刷清输入、输出队列。
             */
            tcflush(fd, TCIOFLUSH);     //设置前flush
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);

            /**
             *通过tcsetattr函数把新的属性设置到串口上。
             * tcsetattr(串口描述符，立即使用或者其他标示，指向termios的指针)
             */
            status = tcsetattr(fd, TCSANOW, &Opt);
            if (status != 0)
            {
                perror("tcsetattr fd1");
                return;
            }
            tcflush(fd, TCIOFLUSH);  //设置后flush
        }
    }
}

/**
 * 功     能：设置串口数据位，停止位和效验位
 * 输入参数：
 *           fd-> 打开的串口文件句柄
 *          databits  数据位   取值 为 7 或者8
 *          stopbits  停止位   取值为 1 或者2
 *          parity    效验类型  取值为N,E,O,,S
 * 输出参数：
 * 返回值： 0 成功
 * 开发人员：
 * 修改记录：Lzy 2013-1-14
 */
int au_appuart_set_parity(unsigned int port, int databits, int stopbits, char parity)
{
    struct termios options;
    int fd;

    fd = port;
    if (tcgetattr(fd, &options) != 0)
        return -1;

    options.c_iflag = 0;
    options.c_cflag &= ~CSIZE;

    /*设置数据位数*/
    switch (databits)
    {
    case 5:
        options.c_cflag |= CS5;
        break;
    case 6:
        options.c_cflag |= CS6;
        break;
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        return -1;
    }

    switch (parity)
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB; /* Clear parity enable */
        options.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB); /* */
        options.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB; /* Enable parity */
        options.c_cflag &= ~PARODD; /* */
        options.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case 'S':
    case 's': /*as no parity*/
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        return -1;
    }

    /* 设置停止位*/
    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        return -1;
    }

    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IUCLC); /* 输入参数的设置 */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* 不需要回车直接发送*/
    options.c_oflag &= ~OPOST; /* 输出参数的设置 */

    tcflush(fd, TCIFLUSH);
    options.c_cc[VTIME] = 0; /*  设置超时15 seconds*/
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        perror("SetupSerial 3");
        return -1;
    }

    return 0;
}

/**
 * 功     能：设置串口
 * 输入参数：串口 速度 数据位  停止位 奇偶较检位
 * 输出参数：
 * 返回值： 0 成功
 * 开发人员：
 * 修改记录：Lzy 2013-1-14
 */
void au_appuart_set(unsigned int port, int speed, int databits, int stopbits, char parity)
{
    au_appuart_set_speed(port, speed);
    au_appuart_set_parity(port, databits, stopbits, parity);
}

/**
 * 函数功能： 打开串口模块
 * pcComName( 入口):"COM1","COM2","COM3","COM4",串口名称
 * phDevHandel( 出口):设备句柄
 * 返回值: 0x00-->打开成功
 *         : 0x89-->设备已经打开
 *         : 0x8E-->内存分配失败
 *         : 0x01-->其它错误
 */
int AU_OpenRS232(char *pcComName, int *phDevHandel)
{
    g_UartFd = open(UART_DEV, O_RDWR | O_NOCTTY/*| O_NONBLOCK , 0*/);   // 不阻塞打开串口
    if (g_UartFd < 0)    // 检查串口打开是否成功
    {
        perror("open uart");
        return 1;
    }
    *phDevHandel = g_UartFd;

    return 0x00;
}

/**
 *  函数功能： 关闭串口模块
 *  pcComName( 入口):设备句柄
 *  phDevHandel( 出口):设备句柄
 *  返回值: 0x00-->关闭成功
 */
int AU_CloseRS232(int *phDevHandle)
{
    close(g_UartFd);
    *phDevHandle = g_UartFd = -1;
    return 0x00;
}

/**
 * 函数功能：设置RS232口的通讯方式
 * hDevHandel:设备名称
 * ucBPS: 0x01-->对应1200波特率            *    : 0x02-->对应2400波特率      *    : 0x03-->对应4800波特率
 *    : 0x04-->对应9600波特率          *    : 0x05-->对应14400波特率         *    : 0x06-->对应28800波特率
 *    : 0x07-->对应19200波特率             *    : 0x08-->对应57600波特率         *    : 0x09-->对应115200波特率
 *    : 0x0A-->对应38400波特率
 *    ucPAR:'N'-->无效验（缺省）         :'E'-->偶效验         :'O'-->奇效验
 *    ucDBS: 0x07-->7位数据位, 0x08-->8位数据位（缺省）
 *    返回值: 0x00-->成功        : 0x8C-->设备未打开           : 0x8B-->参数错误(ucBPS,ucPAR或者ucDBS值错误)
 *         : 0x8D-->串口无法使用          : 0x01-->其他错误（操作系统错误等）
 */
int AU_InitRS232(int hDevHandel, unsigned char ucBPS, unsigned char ucPAR, unsigned char ucDBS)
{
    int gprs_baudrate[] = { 1200, 2400, 4800, 9600, 14400, 28800, 19200, 57600, 115200, 38400 };
    int baud, port;

    port = hDevHandel;
    baud = gprs_baudrate[ucBPS - 1];

    au_appuart_set(port, baud, 8, 1, 'N');

    return 0;
}

/**
 * 函数功能：从指定设备读数据。提供毫秒级读写，当时时间的精度为10Ms，即小于10Ms按照0处理
 * 入口hDevHandel:设备句柄        uiLength:计划接收长度，若接收长度为0默认接收成功
 * uiTimeout:超时时间(单位：毫秒)：0表示按缺省的超时等待时间。若为负数，则一直等待。缺省的超时为1小时
 * 出口pvBuf:接收缓冲区        *puiRetLen:实际接收的长度
 * 返回值: 0x00-->读设备或接收成功         : 0x8C-->设备未打开           : 0x8B-->参数错误（pvBuf或puiRetLen为空指针）           : 0x01-->读设备或接收失败或打开方式错误
 */
int AU_ReadRS232Ms(int hDevHandel, unsigned int uiLength, int uiTimeOut, unsigned char *pvBuf, unsigned int *puiRetLen)
{
    int rtn = 0, len = 0;

    /* 串口数据的读取，uiTimeOut为超时时间，池uiTimeOut＝0时则一直等待     */
    do
    {
        rtn = read(hDevHandel, pvBuf, uiLength);
        if (rtn < 0)
        {
            perror("read\n");
            return rtn;
        }
        else if (rtn == 0) /* 没有读到数据*/
        {
            if (len > 0) /* 数据接收完成 */
                break;

            if (uiTimeOut > 0) /* 超时时间处理*/
            {
                if (uiTimeOut-- <= 1) /* 超时时间到*/
                    break;
            }
            M_Delay(DELAY_TIME);
        }
        else
        {
            len += rtn; /* 长度增加 */
            pvBuf += rtn; /* 存储位置移动 */
            M_Delay(DELAY_TIME);
        }

    } while (uiTimeOut >= 0);

    *puiRetLen = len;

    return 0;
}

/**
 * 函数功能：往指定设备写数据
 * hDevHandel:设备句柄
 * pvBuf:发送缓冲区
 * uiLength:发送长度；若发送长度为0默认成功
 * uiTimeout:超时时间(单位：毫秒)  ：0表示按缺省的超时等待时间。若为负数，则一直等待。缺省的超时为1小时
 * 返回值: 0x00-->写设备或发送成功         : 0x8C-->设备未打开           : 0x8B-->参数错误（pvBuf为空）           : 0x01-->写设备或发送失败或打开方式错误
 *         : 'G'---->发送成功未确认
 */
int AU_WriteRS232Ms(int hDevHandel, unsigned int uiLength, int uiTimeOut, unsigned char *pvBuf)
{
    int rtn = 0;

    rtn = write(hDevHandel, pvBuf, uiLength);

    return rtn;
}
</errno.h></termios.h></fcntl.h></sys></sys></unistd.h></stdlib.h></string.h></stdio.h>
