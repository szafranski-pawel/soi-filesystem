#include "bitmap.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool create(int size, char *diskName)
{
        FILE *virtualDisk;
        struct superBlock superBlockTemp;
        struct iNode iNodeTemp;
        char zeroChar = 0;
        printf("Creating virtual disk. Size is %d\n", size);
        if((virtualDisk = fopen(diskName, "w+")) == NULL)
        {
                printf("Cannot open/create virtual disk\n");
                return false;
        }
        size = size * 1024;
        superBlockTemp.diskSize = size;
        superBlockTemp.fileCounter = 0;
        superBlockTemp.dataBlocks = 0;
        superBlockTemp.freeDataBlocks = ceil(size/BLOCKSIZE);
        for(int it = 0; it < 32; it++)
                iNodeTemp.fileName[it] = -1;
        iNodeTemp.fileSize = -1;
        iNodeTemp.firstBlock = -1;
        fseek(virtualDisk, 0, SEEK_SET);
        fwrite(&superBlockTemp, sizeof(superBlockTemp), 1, virtualDisk);
        fseek(virtualDisk, (ceil(size/BLOCKSIZE) + ceil(ceil(size/BLOCKSIZE)/BLOCKSIZE) + 3)*BLOCKSIZE - 1, SEEK_SET);
        fwrite(&zeroChar, sizeof(zeroChar), 1, virtualDisk);
        fseek(virtualDisk, BLOCKSIZE, SEEK_SET);
        fwrite(&zeroChar, sizeof(zeroChar), BLOCKSIZE + ceil(ceil(size/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE, virtualDisk);
        fwrite(&iNodeTemp, sizeof(iNodeTemp), NR_INODES, virtualDisk);

        struct dataBlock dataBlockTemp;
        for(int it = 0; it < DATABLOCKSIZE; it++)
                dataBlockTemp.data[it] = -1;
        dataBlockTemp.nextBlock = -1;
        fwrite(&dataBlockTemp, sizeof(dataBlockTemp), ceil(size/BLOCKSIZE), virtualDisk);
        fclose(virtualDisk);
        return true;
}
