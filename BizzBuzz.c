#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LEN 10000
#define MAX_WORD_LEN 10000

const char *bizz = "Bizz";
const char *buzz = "Buzz";
const char *bizzbuzz = "BizzBuzz";

void BizzBuzz(char *string, int dest);
int GetWord(char *string, char *word, int ind);
int IsNum(char *word);
int DigitSum(char *word);
void MakePrint(int dest, char *word);
int Mult_3(char *word);
int Mult_5(char *word);
int Mult_15(char *word);

int main(int argc, char **argv)
{

    char string[MAX_STRING_LEN];

    int source = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_RDWR | O_CREAT | O_APPEND | O_TRUNC, S_IRWXU | S_IRWXG);

    if (source == -1 || dest == -1)
    {
        printf("Open error\n");
        exit(0);
    }

    int source_size = read(source, string, MAX_STRING_LEN);
    if (source_size == -1)
    {
        printf("Read error\n");
        exit(0);
    }

    BizzBuzz(string, dest);

    if (close(source) == -1 || close(dest) == -1)
    {
        printf("File closing error\n");
        exit(0);
    }

    return 0;
}

void BizzBuzz(char *string, int dest)
{

    char last_char;
    char *word = (char *)calloc(MAX_WORD_LEN, sizeof(char));
    int ind = 0;

    while ((last_char = string[ind]) != EOF)
    {
        if (last_char == ' ' || last_char == '\t' || last_char == '\n')
        {
            ind++;
            write(dest, &last_char, 1);
        }
        else
        {
            ind = GetWord(string, word, ind);
            MakePrint(dest, word);
        }
    }
}

int GetWord(char *string, char *word, int ind)
{

    int len = 0;
    int start_ind = ind;
    while (string[ind] != '\n' && string[ind] != '\t' && string[ind] != ' ' && string[ind] != EOF)
    {
        ind++;
        len++;
    }
    if (string[ind] != EOF)
    {
        strncpy(word, string + start_ind, len);
        word[len] = '\0';
    }

    return ind;
}

int IsNum(char *word)
{
    if (!isdigit(word[0]) && word[0] != '-')
    {
        return 0;
    }

    else
    {
        for (int i = 1; i < strlen(word); i++)
        {
            if (!isdigit(word[i]))
                return 0;
        }
    }
    return 1;
}

void MakePrint(int dest, char *word)
{
    if (!IsNum(word))
    {
        write(dest, word, strlen(word));
    }
    else if (Mult_15(word))
    {
        write(dest, bizzbuzz, strlen(bizzbuzz));
    }
    else if (Mult_5(word))
    {
        write(dest, buzz, strlen(buzz));
    }
    else if (Mult_3(word))
    {
        write(dest, bizz, strlen(bizz));
    }
    else
    {
        write(dest, word, strlen(word));
    }
}

int DigitSum(char *word)
{
    int sum = 0;
    if (word[0] != '-')
    {
        sum += word[0] - '0';
    }

    for (int i = 1; i < strlen(word); i++)
    {
        sum += word[i] - '0';
    }

    return sum;
}

int Mult_3(char *word)
{
    return (DigitSum(word) % 3 == 0);
}

int Mult_5(char *word)
{
    return (word[strlen(word) - 1] == '5' || word[strlen(word) - 1] == '0');
}

int Mult_15(char *word)
{
    return (Mult_5(word) && Mult_3(word));
}