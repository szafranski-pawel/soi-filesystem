#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool moveFileToDisk(char *fileName, char *diskName)
{
        FILE *virtualDisk, *sourceFile;
        struct iNode iNodeTemp;
        struct superBlock superBlockTemp;
        printf("Copying file '%s' to virtual disk\n", fileName);
        if((virtualDisk = fopen(diskName, "r+b")) == NULL)
        {
                printf("Cannot open virtual disk\n");
                return false;
        }
        if((sourceFile = fopen(fileName, "r+")) == NULL)
        {
                printf("Cannot open file '%s'\n",fileName);
                return false;
        }
        superBlockTemp = readSuperBlock(virtualDisk);
        if(superBlockTemp.fileCounter + 1 > NR_INODES)
        {
                printf("File limit reached\n");
                return false;
        }
        fseek(sourceFile, 0, SEEK_END);
        strcpy(iNodeTemp.fileName, fileName);
        iNodeTemp.fileSize = ftell(sourceFile);
        if(ceil(iNodeTemp.fileSize/4096) > superBlockTemp.freeDataBlocks)
        {
                printf("Not enough space on virtual disk\n");
                return false;
        }
        char *dataBitmap = malloc(sizeof(char)*superBlockTemp.dataBlocks);
        readDataBitmap(dataBitmap, virtualDisk, superBlockTemp.dataBlocks);
        iNodeTemp.firstBlock = findFreeBlock(dataBitmap, superBlockTemp.dataBlocks);
        struct iNodeBitmap iNodeBitmapTemp;
        readINodeBitmap(&iNodeBitmapTemp, virtualDisk);
        int freeINodeNumber = findFreeINode(&iNodeBitmapTemp);
        if(checkFileName(fileName, virtualDisk, superBlockTemp.diskSize) != false)
        {
                printf("File '%s' already exists on virtual drive\n", fileName);
                return false;
        }
        saveINode(iNodeTemp, virtualDisk, freeINodeNumber, superBlockTemp.diskSize);
        setINodeBitmap(&iNodeBitmapTemp, virtualDisk, freeINodeNumber, 1);
        saveDataToVirtualDisk(virtualDisk, sourceFile, dataBitmap, &superBlockTemp, iNodeTemp);
        fclose(virtualDisk);
        fclose(sourceFile);
        return true;
}
