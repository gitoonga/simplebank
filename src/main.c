// main.c
#include "../common/common.h"
void guser();
int cuser();

int main(void)
{
    // cuser();
    guser();
}
void guser()
{
    Transfer *transfer = malloc(sizeof(Transfer));

    transfer->amount = 100;
    transfer->receipientAcc = 1779672153;
    transfer->senderAcc = 982397341;

    transferMoney(transfer);
}

int cuser()
{
    Holder *HolderInfo = malloc(sizeof(Holder));

    if (HolderInfo == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Create An Account:\n");

    printf("Enter First name:");
    fgetsNULLEStr(HolderInfo->fname, sizeof(HolderInfo->fname));

    printf("Enter LastName:");
    fgetsNULLEStr(HolderInfo->lname, sizeof(HolderInfo->lname));

    printf("Enter Phone Number:");
    fgetsNULLEStr(HolderInfo->phone, sizeof(HolderInfo->phone));

    printf("Enter Residence:");
    fgetsNULLEStr(HolderInfo->residence, sizeof(HolderInfo->residence));

    srand(time(NULL));
    const char *dtm = getCurrentTime();

    HolderInfo->id = rand();
    strcpy(HolderInfo->date_created, dtm);
    strcpy(HolderInfo->date_updated, dtm);

    createAccount(HolderInfo);

    free(HolderInfo);
    return 0;
}