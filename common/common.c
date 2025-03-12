#include "common.h"

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

Holder *parseAccountData(char *JsonText)
{
    regex_t regex_keys;
    regex_t regex_values;
    regmatch_t kmatches[2];
    regmatch_t vmatches[2];

    // AccountInfo *accInfo = sizeof(AccountInfo);
    Holder *HolderInfo = malloc(sizeof(Holder));

    const char *key_pattern = "\"([^\"]+)\":";
    const char *value_pattern = ": *\"([^\"]+)\"";
    // Removed the trailing comma from the regex so it matches the last key-value pair.
    // The old pattern missed the "date" key because it expected a comma after the value.
    // Regex Would fail when parsing the last key-value pair in the JSON
    // Since the last pair typically doesn't have a trailing comma
    int regvk = regcomp(&regex_keys, key_pattern, REG_EXTENDED);
    int regvv = regcomp(&regex_values, value_pattern, REG_EXTENDED);

    if (regvk != 0 || regvv != 0)
    {
        printf("Error compiling regex patterns\n");
        return NULL;
    }

    const char *searchString = JsonText;
    // int key_count = 0;

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

        // printf("Found key %d: %s => %s\n", ++key_count, key, value);

        searchString += vmatches[0].rm_eo;

        buildHolderInfo(HolderInfo, key, value);

        free(key);
        free(value);
    }

    regfree(&regex_keys);
    regfree(&regex_values);
    return HolderInfo;
}

void buildHolderInfo(Holder *HolderInfo, const char *key, const char *value)
{

    char *nqkey = removeQ(key);

    if (strcmp(nqkey, "id") == 0)
    {
        HolderInfo->id = atoi(value);
    }
    else if (strcmp(nqkey, "phone") == 0)
    {
        strncpy(HolderInfo->phone, value, sizeof(HolderInfo->phone) - 1);
    }
    else if (strcmp(nqkey, "first_name") == 0)
    {
        strncpy(HolderInfo->fname, value, sizeof(HolderInfo->fname) - 1);
    }
    else if (strcmp(nqkey, "last_name") == 0)
    {
        strncpy(HolderInfo->lname, value, sizeof(HolderInfo->lname) - 1);
    }
    else if (strcmp(nqkey, "residence") == 0)
    {
        strncpy(HolderInfo->residence, value, sizeof(HolderInfo->residence) - 1);
    }
    else if (strcmp(nqkey, "balance") == 0)
    {
        strncpy(HolderInfo->balance, value, sizeof(HolderInfo->balance) - 1);
    }
    else if (strcmp(nqkey, "date_created") == 0)
    {
        strncpy(HolderInfo->date_created, value, sizeof(HolderInfo->date_created) - 1);
    }
    else if (strcmp(nqkey, "date_updated") == 0)
    {
        const char *dtm = getCurrentTime();
        strncpy(HolderInfo->date_updated, dtm, sizeof(HolderInfo->date_updated) - 1);
    }

    free(nqkey);
}

char *removeQ(const char *str)
{
    int i, j = 0;
    int len = strlen(str);
    char *newstr = (char *)malloc(sizeof(len + 1));

    if (newstr == NULL)
        return NULL;

    for (i = 0; i < len; i++)
    {
        if (str[i] != '"' && str[i] != '/')
        {
            newstr[j++] = str[i];
        }
    }

    newstr[j] = '\0';

    return newstr;
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
    fprintf(userFile, "{\n");
    fprintf(userFile, "  \"id\": \"%d\",\n", Holder->id);
    fprintf(userFile, "  \"first_name\": \"%s\",\n", Holder->fname);
    fprintf(userFile, "  \"last_name\": \"%s\",\n", Holder->lname);
    fprintf(userFile, "  \"phone\": \"%s\",\n", Holder->phone);
    fprintf(userFile, "  \"residence\": \"%s\",\n", Holder->residence);
    fprintf(userFile, "  \"balance\": \"%s\",\n", Holder->balance);
    fprintf(userFile, "  \"date_created\": \"%s\",\n", Holder->date_created);
    fprintf(userFile, "  \"date_updated\": \"%s\"\n", Holder->date_updated);
    fprintf(userFile, "}\n");
}

void writeEmptyAccountJson(FILE *userFile)
{
    fprintf(userFile, "/0");
}