#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define NETDEVPATH "/proc/net/dev"
#define BUFSZ 128
#define DEVSZ 8

char* get_netdev_name(char *src)
{
    char *tmp = src;
    int i = 0;

    while(*tmp != ':')
    {
        if(isspace(*tmp))
        {
            tmp++;
            continue;
        }

        tmp++;
    }

    *tmp = '\0';

    return src;
}

int phase_netdev_info(FILE *fd)
{
    char buf[BUFSZ];
    int i = 0;
    char *devname[DEVSZ];

    fgets(buf, BUFSZ, fd);
    fgets(buf, BUFSZ, fd);

    while (fgets(buf, BUFSZ, fd))
    {
        char *tmp = buf;
        devname[i] = get_netdev_name(tmp);
        printf("devname[%d] : %s\n", i, devname[i]);
        i++;
    }

    return 0;
}

int main(void)
{
    FILE *fd = fopen(NETDEVPATH, "r");
    if(fd < 0)
    {
        perror("fopen :\n");
    }

    phase_netdev_info(fd);

    return 0;
}

