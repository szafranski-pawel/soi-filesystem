#ifndef INODE_H
#define INODE_H
#include "const.h"
#include <stdbool.h> // bool library
#include <stdio.h>
#include <math.h>
#include <string.h>

struct iNode
{
        char fileName[32];
        int fileSize;
        int firstBlock;
};

bool checkFileName(char *fileName, FILE *virtualDisk, int diskSize)
{
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil(diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE, SEEK_SET);
        struct iNode iNodeTemp;
        for(int it = 0; it < NR_INODES; it++)
        {
                fread(&iNodeTemp, sizeof(iNodeTemp), 1, virtualDisk);
                //printf("%s\n", iNodeTemp.fileName);
                if(strcmp(fileName, iNodeTemp.fileName) == true)
                        return true;
        }
        return false;
}

void saveINode(struct iNode iNodeTemp, FILE *virtualDisk, int freeINodeNumber, int diskSize)
{
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil(diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + freeINodeNumber*sizeof(iNodeTemp), SEEK_SET);
        fwrite(&iNodeTemp, sizeof(iNodeTemp), 1, virtualDisk);
        printf("saveINode zakonczone\n");
}

struct iNode readINode(char *fileName, FILE *virtualDisk, int diskSize)
{
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil(diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE, SEEK_SET);
        struct iNode iNodeTemp;
        for(int it = 0; it < NR_INODES; it++)
        {
                fread(&iNodeTemp, sizeof(iNodeTemp), 1, virtualDisk);
                if(strcmp(fileName, iNodeTemp.fileName) == true)
                        return iNodeTemp;
        }
}

#endif
