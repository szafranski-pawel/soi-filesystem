#include "const.h"
#include <stdbool.h> // bool library
#include <stdio.h>

struct iNodeBitmap
{
        char isNodeFree[NR_INODES];
};

void readDataBitmap(char *pointerToBuffer, FILE *virtualDisk, int dataBlockNumber)
{
        fseek(virtualDisk, 2*BLOCKSIZE, SEEK_SET);
        fread(pointerToBuffer, sizeof(char), dataBlockNumber, virtualDisk);
}

int findFreeBlock(char *pointerToBuffer, int dataBlockNumber)
{
        for(int it = 0; it < dataBlockNumber; it++)
        {
                if(pointerToBuffer[it] == 0)
                        return it;
        }
        return -1;
}

void readINodeBitmap(struct iNodeBitmap *pointerToBitmap, FILE *virtualDisk)
{
        fseek(virtualDisk, BLOCKSIZE, SEEK_SET);
        fread(pointerToBitmap, sizeof(struct iNodeBitmap), 1, virtualDisk);
}

int findFreeINode(struct iNodeBitmap *pointerToBitmap)
{
        for(int it = 0; it < NR_INODES; it++)
        {
                if((*pointerToBitmap).isNodeFree[it] == 0)
                        return it;
        }
        return -1;
}

void setINodeBitmap(struct iNodeBitmap *pointerToBitmap, FILE *virtualDisk, int freeINodeNumber, char sign)
{
        (*pointerToBitmap).isNodeFree[freeINodeNumber] = sign;
        fseek(virtualDisk, BLOCKSIZE, SEEK_SET);
        fwrite(pointerToBitmap, sizeof(struct iNodeBitmap), 1, virtualDisk);
}

void setDataBlockBitmap(char *dataBitmap, FILE *virtualDisk, int dataBlockPosition, char sign, int dataBlockNumber)
{
        dataBitmap[dataBlockPosition] = sign;
        fseek(virtualDisk, 2*BLOCKSIZE, SEEK_SET);
        fwrite(dataBitmap, sizeof(char), dataBlockNumber, virtualDisk);
}
