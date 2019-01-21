#include "copy.h"
#include "create.h"

int main()
{
        int choice, size;
        char diskName[24];
        char fileName[24];
        printf("VSFS - Very Simple File System\n");
        while(1)
        {
                scanf("%d", &choice);
                if(choice == 1)
                {
                        scanf("%d %s", &size, &diskName);
                        create(size, diskName);
                }
                if(choice == 2)
                {
                        scanf("%s %s", &fileName, &diskName);
                        copyFileToDisk(fileName, diskName);
                }
                if(choice == 3)
                {
                        scanf("%s %s", &fileName, &diskName);
                        copyFileFromDisk(fileName, diskName);
                }
                if(choice == 0)
                        return 0;
        }
}
