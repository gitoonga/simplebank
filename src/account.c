#include "../common/common.h"

int createAccount(Holder *Holder)
{
    FILE *userFile;
    char filename[16];
    char foldername[16];
    char filepath[42];
    char dirpath[26];

    snprintf(filename, sizeof(filename), "%d.json", Holder->id);
    snprintf(foldername, sizeof(foldername), "%d", Holder->id);

    snprintf(dirpath, sizeof(dirpath), "accounts/%s", foldername);
    mkdir(dirpath, 0777);
    snprintf(filepath, sizeof(filepath), "accounts/%s/%s", foldername, filename);

    userFile = fopen(filepath, "w");

    if (userFile == NULL)
    {
        return -1;
    }

    writeAccountJson(userFile, Holder);

    fclose(userFile);

    return 0;
}

int transferMoney(Transfer *transfer)
{
    char rfilepath[69];
    char sfilepath[69];
    char Rjsontext[255] = {0};
    char Sjsontext[255] = {0};

    FILE *RaccountFile;
    FILE *SaccountFile;

    snprintf(rfilepath, sizeof(rfilepath), "accounts/%d/%d.json", transfer->receipientAcc, transfer->receipientAcc);
    snprintf(sfilepath, sizeof(sfilepath), "accounts/%d/%d.json", transfer->senderAcc, transfer->senderAcc);

    RaccountFile = fopen(rfilepath, "r");
    SaccountFile = fopen(sfilepath, "r");

    if (RaccountFile == NULL || SaccountFile == NULL)
    {
        printf("file missing \n");
        return -1;
    }

    char *Rejsontext = parseAccountText(RaccountFile, Rjsontext, sizeof(Rjsontext));
    char *Sejsontext = parseAccountText(SaccountFile, Sjsontext, sizeof(Sjsontext));

    fclose(RaccountFile);
    fclose(SaccountFile);

    AssocArray *RArr = assocArrayCreate();
    AssocArray *SArr = assocArrayCreate();
    parseAccountData(Rejsontext, RArr);
    printf("------------------------------------\n");
    parseAccountData(Sejsontext, SArr);

    void *phoneValue = AssocArrayGet(RArr, "phone");
    printf("Phone: %s\n", (char *)phoneValue);

    // printf("Receipient %s \n", Rejsontext);
    // printf("Sender %s \n", Sejsontext);

    return 0;
}
