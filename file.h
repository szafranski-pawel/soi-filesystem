#include "const.h"
#include "superblock.h"
#include "inode.h"
#include "bitmap.h"
#include <stdio.h>
struct dataBlock
{
        char data[DATABLOCKSIZE];
        int nextBlock;
};

void saveDataToVirtualDisk(FILE *virtualDisk, FILE *sourceFile, char *dataBitmap, struct superBlock *superBlockTemp, struct iNode iNodeTemp)
{
        struct dataBlock dataBlockTemp;
        int freeBlock = -1;
        int nextFreeBlock = -1;
        int it = 1;
        if(iNodeTemp.fileSize <= DATABLOCKSIZE)
        {
                fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + iNodeTemp.firstBlock * sizeof(dataBlockTemp), SEEK_SET);
                fseek(sourceFile, 0, SEEK_SET);
                fread(&dataBlockTemp.data, sizeof(char), iNodeTemp.fileSize, sourceFile);
                dataBlockTemp.nextBlock = -1;
                fwrite(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
                setDataBlockBitmap(dataBitmap, virtualDisk, iNodeTemp.firstBlock, 1, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
                return;
        }
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + iNodeTemp.firstBlock * sizeof(dataBlockTemp), SEEK_SET);
        fseek(sourceFile, 0, SEEK_SET);
        fread(&dataBlockTemp.data, sizeof(char), DATABLOCKSIZE, sourceFile);
        iNodeTemp.fileSize -= DATABLOCKSIZE;
        nextFreeBlock = findFreeBlock(dataBitmap, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
        dataBlockTemp.nextBlock = nextFreeBlock;
        fwrite(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
        setDataBlockBitmap(dataBitmap, virtualDisk, iNodeTemp.firstBlock, 1, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
        while(iNodeTemp.fileSize > DATABLOCKSIZE)
        {
                freeBlock = nextFreeBlock;
                nextFreeBlock = findFreeBlock(dataBitmap, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
                fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + freeBlock * sizeof(dataBlockTemp), SEEK_SET);
                fseek(sourceFile, it*DATABLOCKSIZE, SEEK_SET);
                dataBlockTemp.nextBlock = nextFreeBlock;
                fwrite(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
                setDataBlockBitmap(dataBitmap, virtualDisk, freeBlock, 1, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
                iNodeTemp.fileSize -= DATABLOCKSIZE;
                it++;
        }
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + nextFreeBlock * sizeof(dataBlockTemp), SEEK_SET);
        fseek(sourceFile, it*DATABLOCKSIZE, SEEK_SET);
        fread(&dataBlockTemp.data, sizeof(char), iNodeTemp.fileSize, sourceFile);
        dataBlockTemp.nextBlock = -1;
        fwrite(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
        setDataBlockBitmap(dataBitmap, virtualDisk, nextFreeBlock, 1, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
        return;
}
