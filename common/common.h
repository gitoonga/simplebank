// common.h
#define _POSIX_C_SOURCE 200809L

#ifndef _COMMON_H
#define _COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>
#include <regex.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct kv
{
    char *key;
    void *value;
    struct kv *next;
} kv;

typedef struct
{
    kv *table[MAX_SIZE];
    int size;
} AssocArray;

typedef struct
{
    int id;
    char phone[15];
    char fname[20];
    char lname[20];
    char residence[50];
    char balance[10];
    char date[20];
} Holder;

typedef struct
{
    int senderAcc;
    int amount;
    int receipientAcc;
} Transfer;

typedef struct
{
    char dateCreated[27]; // 2020:12:20-12:58:34
    char dateUpdated[27]; // 2020:12:20-12:58:34
    char editUser[8];
    char amount[10];
} AccountInfo;

char fgetsNULLEStr(char *string, size_t size);
int createAccount(Holder *holder);
int transferMoney(Transfer *transfer);
int parseAccountData(char *JsonText, AssocArray *pArr);
char *parseAccountText(FILE *accountFile, char *accountText, size_t max_size);
void writeAccountJson(FILE *userFile, Holder *Holder);
char *getCurrentTime();
unsigned int hash(const char *key);
AssocArray *assocArrayCreate();
void AssocArrayPut(AssocArray *a, const char *key, void *value);
void *AssocArrayGet(AssocArray *a, const char *key);
void AssocArrayFree(AssocArray *a);

#endif
