#include "const.h"
#include <stdbool.h> // bool library
#include <stdio.h>
#include <math.h>

struct iNode
{
        char fileName[32];
        int fileSize;
        int firstBlock;
};

bool checkFileName(char *fileName, FILE *virtualDisk, int diskSize)
{
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil(diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE, SEEK_SET);
        for(int it = 0; it < NR_INODES; it++)
        {

        }
}
