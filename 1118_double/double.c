#include <stdio.h>
#include <string.h>
#include "double.h"

#define FILE_MAX 4
#define WORD_MAX 16

int check_same_word(FILE *fp)
{
    int c;
    int i = 0;
    char word[WORD_MAX];
    char oldword[WORD_MAX];
    memset(word, 0, sizeof(word));
    memset(word, 0, sizeof(oldword));

    while((c=getc(fp)) != EOF)
    {
        if((c == ' ') || (c == ',') || (c == '.') || (c == '?') || (c == '!'))
        {
            word[i] = '\0';
            if(strlen(word))//strlen(word)为防止连续出现特殊字符
            {
                if(strcmp(oldword, word) == 0)
                {
                    printf("The same word : %s\n", oldword);
                }
                strcpy(oldword, word);
            }

            i = 0;
            memset(word, 0, sizeof(word));
            continue;
        }

        word[i] = c;
        i++;
    }
}

int main(int argc, char*argv[])
{
    FILE *fparray[FILE_MAX];
    FILE *fp;
    int i = 0;
    int j = 0;

    if(argc > 5)
    {
        printf("enter parameter error!\n");
        return HI_FAILURE;
    }
    else
    {
        for(i=1, j=0; i<argc; i++)
        {
            fp = fopen(argv[i], "r");
            if(fp == NULL)
            {
                printf("%s");
                continue;
            }
            fparray[j] = fp;
            j++;
        }
    }

    for(i=0; i<j; i++)
    {
        check_same_word(fparray[i]);
    }

    return 0;
}

