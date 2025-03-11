#include "common.h"
#include <stddef.h>
#include <regex.h>

char fgetsNULLEStr(char *string, size_t size)
{
    fgets(string, size, stdin);
    size_t len = strcspn(string, "\n");

    if (string[len] == '\n')
    {
        string[len] = '\0';
    }
    return *string;
}

int parseAccountData(char *JsonText)
{
    regex_t regex_keys;
    regex_t regex_values;
    regmatch_t kmatches[2];
    regmatch_t vmatches[2];

    // AccountInfo *accInfo = sizeof(AccountInfo);
    // Holder *HolderInfo = malloc(sizeof(Holder));

    const char *key_pattern = "\"([^\"]+)\":";
    const char *value_pattern = ": *\"([^\"]+)\",";

    int regvk = regcomp(&regex_keys, key_pattern, REG_EXTENDED);
    int regvv = regcomp(&regex_values, value_pattern, REG_EXTENDED);

    if (regvk != 0 || regvv != 0)
    {
        printf("Error compiling regex patterns\n");
        return -1;
    }

    const char *searchString = JsonText;
    int key_count = 0;

    while (1)
    {
        int rexec_rtk = regexec(&regex_keys, searchString, 2, kmatches, 0);
        if (rexec_rtk)
        {
            break;
        }

        size_t key_len = kmatches[1].rm_eo - kmatches[1].rm_so;
        char *key = (char *)malloc(key_len + 1);
        if (!key)
        {
            printf("Memory allocation failed\n");
            break;
        }
        strncpy(key, searchString + kmatches[1].rm_so, key_len);
        key[key_len] = '\0';

        searchString += kmatches[0].rm_eo - 1;

        int rexec_rtv = regexec(&regex_values, searchString, 2, vmatches, 0);
        if (rexec_rtv)
        {
            free(key);
            break;
        }

        size_t value_len = vmatches[1].rm_eo - vmatches[1].rm_so;
        char *value = (char *)malloc(value_len + 1);
        if (!value)
        {
            free(key);
            printf("Memory allocation failed\n");
            break;
        }
        strncpy(value, searchString + vmatches[1].rm_so, value_len);
        value[value_len] = '\0';

        printf("Found key %d: %s => %s\n", ++key_count, key, value);

        searchString += vmatches[0].rm_eo;

        free(key);
        free(value);
    }

    regfree(&regex_keys);
    regfree(&regex_values);
    return 0;
}

char *parseAccountText(FILE *accountFile, char *accountText, size_t max_size)
{
    int currentChar = 0;
    long unsigned int index = 0;

    while ((currentChar = fgetc(accountFile)) != EOF && index < max_size - 1)
    {
        accountText[index++] = (char)currentChar;
    }
    accountText[index] = '\0';

    return accountText;
}

char *getCurrentTime()
{
    time_t now = time(NULL);
    if (now == (time_t)-1)
    {
        return NULL;
    }

    struct tm *tm_info = localtime(&now);
    if (tm_info == NULL)
    {
        return NULL;
    }

    char *dtm = malloc(27);
    if (dtm == NULL)
    {
        return NULL;
    }

    snprintf(dtm, 27, "%d:%02d:%02d-%02d:%02d:%02d",
             tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
             tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

    return dtm;
}

void writeAccountJson(FILE *userFile, Holder *Holder)
{
    const char *dtm = getCurrentTime();
    fprintf(userFile, "{\n");
    fprintf(userFile, "  \"id\": \"%d\",\n", Holder->id);
    fprintf(userFile, "  \"first_name\": \"%s\",\n", Holder->fname);
    fprintf(userFile, "  \"last_name\": \"%s\",\n", Holder->lname);
    fprintf(userFile, "  \"phone\": \"%s\",\n", Holder->phone);
    fprintf(userFile, "  \"residence\": \"%s\",\n", Holder->residence);
    fprintf(userFile, "  \"balance\": \"%d\",\n", 0);
    fprintf(userFile, "  \"date\": \"%s\"\n", dtm);
    fprintf(userFile, "}\n");
}