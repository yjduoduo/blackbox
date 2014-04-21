#include "includeall.h"

void printbuf(uint8 *buf,uint16 len)
{
    uint32 i;
    for(i=0;i<len;i++)
    {
        Debug("buf[%d]:0x%02x\n",i,*buf++);
    }
}

