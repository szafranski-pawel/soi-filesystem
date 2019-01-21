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
                (*superBlockTemp).dataBlocks++;
                (*superBlockTemp).freeDataBlocks--;
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
        (*superBlockTemp).dataBlocks++;
        (*superBlockTemp).freeDataBlocks--;
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
                (*superBlockTemp).dataBlocks++;
                (*superBlockTemp).freeDataBlocks--;
                it++;
        }
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + nextFreeBlock * sizeof(dataBlockTemp), SEEK_SET);
        fseek(sourceFile, it*DATABLOCKSIZE, SEEK_SET);
        fread(&dataBlockTemp.data, sizeof(char), iNodeTemp.fileSize, sourceFile);
        dataBlockTemp.nextBlock = -1;
        fwrite(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
        setDataBlockBitmap(dataBitmap, virtualDisk, nextFreeBlock, 1, ((*superBlockTemp).freeDataBlocks + (*superBlockTemp).dataBlocks));
        (*superBlockTemp).dataBlocks++;
        (*superBlockTemp).freeDataBlocks--;
        return;
}

void readDataFromVirtualDisk(FILE *virtualDisk, FILE *outputFile, struct superBlock *superBlockTemp, struct iNode iNodeTemp)
{
        struct dataBlock dataBlockTemp;
        int block = -1;
        int nextBlock = -1;
        int it = 1;
        if(iNodeTemp.fileSize <= DATABLOCKSIZE)
        {
                fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + iNodeTemp.firstBlock * sizeof(dataBlockTemp), SEEK_SET);
                fseek(outputFile, 0, SEEK_SET);
                fread(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
                fwrite(&dataBlockTemp.data, sizeof(char), iNodeTemp.fileSize, outputFile);
                return;
        }
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + iNodeTemp.firstBlock * sizeof(dataBlockTemp), SEEK_SET);
        fseek(outputFile, 0, SEEK_SET);
        fread(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
        fwrite(&dataBlockTemp.data, sizeof(char), DATABLOCKSIZE, outputFile);
        nextBlock = dataBlockTemp.nextBlock;
        iNodeTemp.fileSize -= DATABLOCKSIZE;
        while(iNodeTemp.fileSize > DATABLOCKSIZE)
        {
                block = nextBlock;
                fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + block * sizeof(dataBlockTemp), SEEK_SET);
                fseek(outputFile, it*DATABLOCKSIZE, SEEK_SET);
                fread(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
                fwrite(&dataBlockTemp.data, sizeof(char), DATABLOCKSIZE, outputFile);
                dataBlockTemp.nextBlock = nextBlock;
                iNodeTemp.fileSize -= DATABLOCKSIZE;
                it++;
        }
        fseek(virtualDisk, BLOCKSIZE + BLOCKSIZE + ceil(ceil((*superBlockTemp).diskSize/BLOCKSIZE)/BLOCKSIZE) * BLOCKSIZE + NR_INODES*sizeof(iNodeTemp) + nextBlock * sizeof(dataBlockTemp), SEEK_SET);
        fseek(outputFile, 0, SEEK_SET);
        fread(&dataBlockTemp, sizeof(dataBlockTemp), 1, virtualDisk);
        fwrite(&dataBlockTemp.data, sizeof(char), iNodeTemp.fileSize, outputFile);
        return;
}
