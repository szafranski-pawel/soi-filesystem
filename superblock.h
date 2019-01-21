#include "const.h"
#include <stdlib.h>
#include <stdio.h>

struct superBlock
{
        int diskSize;
        int fileCounter;
        int dataBlocks;
        int freeDataBlocks;
};

struct superBlock readSuperBlock(FILE *virtualDisk)
{
        fseek(virtualDisk, 0, SEEK_SET);
        struct superBlock superBlockTemp;
        fread(&superBlockTemp, sizeof(superBlockTemp), 1, virtualDisk);
        return superBlockTemp;
}

void saveSuperBlock(struct superBlock *superBlockTemp, FILE *virtualDisk)
{
        fseek(virtualDisk, 0, SEEK_SET);
        fread(superBlockTemp, sizeof(superBlockTemp), 1, virtualDisk);
}
