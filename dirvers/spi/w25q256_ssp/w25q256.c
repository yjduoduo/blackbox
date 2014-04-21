#include "includeall.h"
#include "w25q256.h"
#include "ops_w25qxx.h"

//地址是否合法
#define ADDR_ERR (1)
#define ADDR_OK  (0)




void spi_init(uint32 port)
{
    if( 0 == port )
    {
        PCONP 	|= (0x1<<21);
        FIO0MASK = 0x00000000;
        FIO2MASK = 0x00000000;
        //wp
        PINSEL4 &= ~(0x3<<12);//clear P2.6
        PINSEL4 |= (0x0<<12);//P2.6,Func[0]
        FIO2DIR |= (0x1<<6);//dir P2.6,output
        FIO2CLR &= ~(0x1<<6);//level P2.6,low
        //spi rst
        PINSEL4 &= ~(0x3<<14);//clear P2.7
        PINSEL4 |= (0x0<<14);//P2.7,Func[0]
        FIO2DIR |= (0x1<<7);//dir P2.7,output
        FIO2SET |= (0x1<<7);//level P2.7,high
        //hold
        PINSEL4 &= ~(0x3<<16);//clear P2.8
        PINSEL4 |= (0x0<<16);//P2.8,Func[0]
        FIO2DIR |= (0x1<<8);//dir P2.8,output
        FIO2SET |= (0x1<<8);//level P2.8,high

#if 1
        //cs---GPIO,[ok good]
        PINSEL1 &= ~(0x3<<0);//clear P0.16
        PINSEL1 |= (0x0<<0);//P0.16,Func[0]
        FIO0DIR |= (0x1<<16);//dir P0.16,output
        FIO0SET |= (0x1<<16);//level P0.16,high
#else  //cs---SSEL0,not good
        PINSEL1 &= ~(0x3<<0);//clear P0.16
        PINSEL1 |= (0x2<<0);//P0.16,Func[2]
        FIO0SET |= (0x1<<16);//level P0.16,high


#endif

        //sck0
        PINSEL0 &= ~(0x1<<30);//clear P0.15
        PINSEL0 |= (0x2UL<<30);//P0.15,Func[2]
        //miso0
        PINSEL1 &= ~(0x3<<2);//clear P0.17
        PINSEL1 |= (0x2<<2);//P0.17,Func[2]
        //mosi0
        PINSEL1 &= ~(0x3<<4);//clear P0.18
        PINSEL1 |= (0x2<<4);//P0.18,Func[2]

        SPI_WR_LOW();//WR拉低,防止状态寄存器被改写
        SPI_HOLD_HIGH();//HLOD拉高
        SPI_CS_HIGH();//CS拉高 不选

        SSP0CR0 = (0x00 << 8)               	/* SCR  设置SPI时钟分频	        */
                | (0x00 << 7)                	/* CPHA 时钟输出相位	        */
                | (0x00 << 6)                   /* CPOL 时钟输出极性            */
                | (0x00 << 4)                   /* FRF  帧格式 00=SPI           */
                | (0x07 << 0);                  /* DSS  数据长度,0111=8位       */


        SSP0CR1 = (0x00 << 3)                    /* SOD  从机输出禁能,0=允许	    */
                | (0x00 << 2)                    /* MS   主从选择,0=主机,1=从机  */
                | (0x01 << 1)                    /* SSE  SSP使能                 */
                | (0x00 << 0);                   /* LBM  回写模式	            */


        /* 设置SSP从PCLK获得的分频值,  48M/0x52=585365 */
        SSP0CPSR = 0x62;                         /* 主模式下有效, 最小值为0x02   */

        SPI_CS_HIGH();//CS拉高
    }


    Init_OpsFlash();


}

/*********************************************************************************************************
** Function name:		SSP_SendData
** Descriptions:		SSP接口向SSP总线发送数据
** input parameters:	data        待发送的数据
** output parameters:	返回值为读取的数据
*********************************************************************************************************/
uint8 spi_send(uint8 tdata)
{
    /* Wait FIFO Empty */
    //    while (!(SSP0SR & TFE));
    /* Send the byte */
    SSP0DR = tdata;
    /* Wait until a data is received */
    while (SSP0SR & BSY);
    /* Get the received data */
    return (SSP0DR);
}

/*****************************************************************
 ** function name  :	 spi_flash_receive_byte
 ** description    :  SPI1 receive byte
 ******************************************************************/
uint8 spi_recv(void)
{
    /* Wait FIFO Empty */
    //    while (!(SSP0SR & TFE));
    /* Send the byte */
    SSP0DR = 0xff;
    /* Wait until a data is received */
    while (SSP0SR & BSY);
    /* Get the received data */
    return (SSP0DR);
}


void w25qxx_wait_busy(void)
{
    while( w25qxx_read_status_reg1(0) & (0x01));
}
void w25xx_send_1cmd(uint8 cmd)
{
    Flash_CS_0;
    spi_send(cmd);
    Flash_CS_1;
}

void w25qxx_write_enable(void)
{
    uint16 tmp;
    //SRP0 TB BP3 BP2 BP1 BP0 WEL BUSY
    //清除Block Protect位： TB BP3 BP2 BP1 BP0
    tmp = w25qxx_read_status_reg1(0) & (~0x7c);//1000 0011,取反~0111 1100
    w25qxx_write_status_reg1(tmp);

    //清 CMP
    //SUS CMP LB3 LB2 LB1 (R) QE SPR1
    tmp = w25qxx_read_status_reg2(0) & (~0x40);//1011 1111,取反~0100 0000（~40）
    w25qxx_write_status_reg2(tmp);

    w25xx_send_1cmd(SPI_WR_EN);
    SPI_WR_HIGH();

}

void w25qxx_write_disable(void)
{
    uint16 tmp;
    //SRP0 TB BP3 BP2 BP1 BP0 WEL BUSY
    //置位 Block Protect位： TB BP3 BP2 BP1 BP0
    tmp = w25qxx_read_status_reg1(0) | (0x7c);//0111 1100
    w25qxx_write_status_reg1(tmp);

    //置位 CMP
    //SUS CMP LB3 LB2 LB1 (R) QE SPR1
    tmp = w25qxx_read_status_reg2(0) | (0x40);//0100 0000
    w25qxx_write_status_reg2(tmp);
    w25xx_send_1cmd(SPI_WR_DIS);
    SPI_WR_LOW();
}
void w25qxx_SR_write_enable(void)
{
    w25xx_send_1cmd(SPI_VSR_WR_EN);
}


static void w25xx_in_qpi_mode(void)
{
    w25xx_send_1cmd(SPI_ENTER_QPI_MOD);
}
static void w25xx_in_4byte(void)
{
    w25xx_send_1cmd(SPI_ENT_ADDRMOD_4B);
}
static void w25xx_out_4byte(void)
{
    w25xx_send_1cmd(SPI_EXIT_ADDRMOD_4B);
}

void get_all_status_reg(void)
{
    w25qxx_read_status_reg1(1);
    w25qxx_read_status_reg2(1);
    w25qxx_read_status_reg3(1);
}



//十进制打印成二进制
void show_decTobin(uint8 rdata)
{
    uint8 bin[8];
    int8 i=0;
    for(i=0;i<8;i++)
    {
        bin[i]=(rdata>>i)&0x01;
    }

    for(i=0;i<8;i++)
    {
        Debug("%d\t",bin[7-i]);
    }
    Debug("\n");
}



void w25qxx_read_id_3(uint8 *buffer)
{
    Flash_CS_0;
    spi_send(SPI_JEDEC_ID);
    *buffer = spi_recv();
    buffer++;
    *buffer = spi_recv();
    buffer++;
    *buffer = spi_recv();
    Flash_CS_1;
}
void w25qxx_read_unique_id_3(uint8 *buffer)
{
    Flash_CS_0;
    spi_send(SPI_3B_RD_UNIQUE_ID);
    spi_recv();
    spi_recv();
    spi_recv();
    spi_recv();
    *buffer = spi_recv();buffer++;
    *buffer = spi_recv();
    Flash_CS_1;
}

/*
  合法的地址必须为4K的整数倍开始的地址
有余数，开始地址为非法
*/
int32 judge_erase_addr(uint32 byte_addr)
{
    //    Debug("byte_addr:0x%x\n",byte_addr);
    if(byte_addr > 0x01FF0000)
        return ADDR_ERR;
    if(byte_addr%0x1000%4)
        return ADDR_ERR;
    else
        return ADDR_OK;

}


/*******************************************
*function : write a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_write_3(uint32 sector, uint8 * dstbuf, uint8 sector_number)
{
    uint8 send_buf[4];
    uint32 index,page_number,byte_addr,byte_number,buf_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    //    SPI_WR_HIGH();
    for(index=0 ; index<page_number ; index++)
    {
        send_buf[0] = SPI_3B_PAGE_PROG;
        send_buf[1] = (uint8)(byte_addr>>16);
        send_buf[2] = (uint8)(byte_addr>>8);
        send_buf[3] = (uint8)(byte_addr);
        w25qxx_write_enable();
        Flash_CS_0;
        spi_send(send_buf[0]);
        spi_send(send_buf[1]);
        spi_send(send_buf[2]);
        spi_send(send_buf[3]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            spi_send(dstbuf[buf_index++]);
        }
        Flash_CS_1;

        byte_addr += 256;
        w25qxx_wait_busy();
    }
    //    SPI_WR_LOW();

}

/*******************************************
*function : read a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_read_3(uint32 sector, uint8 * buffer, uint8 sector_number)
{
    uint8 send_buf[4];
    uint32 index,page_number,byte_addr,byte_number,buffer_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    for(index=0 ; index<page_number ; index++)
    {

        send_buf[0] = SPI_3B_RD_DAT;
        send_buf[1] = (uint8)(byte_addr>>16);
        send_buf[2] = (uint8)(byte_addr>>8);
        send_buf[3] = (uint8)(byte_addr);

        Flash_CS_0;
        spi_send(send_buf[0]);
        spi_send(send_buf[1]);
        spi_send(send_buf[2]);
        spi_send(send_buf[3]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            buffer[buffer_index++] = spi_recv();
        }
        Flash_CS_1;

        byte_addr += 256;
        w25qxx_wait_busy();
    }
}


void w25qxx_sector_erase_3(uint32 sector)
{
    uint8 send_buf[4];
    uint32 byte_addr;

    byte_addr = sector*4096;
    //the starting byte_addr must be
    if(ADDR_ERR == judge_erase_addr(byte_addr))
    {
        Debug("err,erase addr ivalid!\n");
        return;
    }else{
        //         Debug("erase addr valid!\n");
    }

    //    SPI_WR_HIGH();
    w25qxx_write_enable();
    Flash_CS_0;
    send_buf[0] = SPI_3B_SECTOR_ERS_4K;
    send_buf[1] = (uint8)(byte_addr >> 16);
    send_buf[2] = (uint8)(byte_addr >> 8);
    send_buf[3] = (uint8)(byte_addr);
    spi_send(send_buf[0]);
    spi_send(send_buf[1]);
    spi_send(send_buf[2]);
    spi_send(send_buf[3]);
    Flash_CS_1;
    //    SPI_WR_LOW();

    w25qxx_wait_busy();

}






//////////////////////////////////////////
//////////[4 bytes mode]//////////////////
//////////////////////////////////////////




void w25qxx_read_id_4(uint8 *buffer)
{
    w25xx_in_4byte();
    Flash_CS_0;
    spi_send(SPI_JEDEC_ID);
    *buffer = spi_recv();buffer++;

    *buffer = spi_recv();buffer++;

    *buffer = spi_recv();

    Flash_CS_1;
    w25xx_out_4byte();
}
void w25qxx_read_unique_id_4(uint8 *buffer)
{
    w25xx_in_4byte();
    //    get_all_status_reg();
    Flash_CS_0;
    spi_send(SPI_4B_RD_UNIQUE_ID);
    spi_recv();
    spi_recv();
    spi_recv();
    spi_recv();
    spi_recv();
    *buffer = spi_recv();buffer++;
    *buffer = spi_recv();buffer++;
    *buffer = spi_recv();buffer++;
    *buffer = spi_recv();buffer++;
    Flash_CS_1;
    w25xx_out_4byte();
    //    get_all_status_reg();
}




/*******************************************
*function : write a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_write_4(uint32 sector, uint8 * dst, uint8 sector_number)
{
    uint8 send_buf[5];
    uint32 index,page_number,byte_addr,byte_number,buf_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    w25xx_in_4byte();
    //    SPI_WR_HIGH();
    for(index=0 ; index<page_number ; index++)
    {
        send_buf[0] = SPI_4B_PAGE_PROG;
        send_buf[1] = (uint8)(byte_addr>>24);
        send_buf[2] = (uint8)(byte_addr>>16);
        send_buf[3] = (uint8)(byte_addr>>8);
        send_buf[4] = (uint8)(byte_addr);
        w25qxx_write_enable();

        Flash_CS_0;
        spi_send(send_buf[0]);
        spi_send(send_buf[1]);
        spi_send(send_buf[2]);
        spi_send(send_buf[3]);
        spi_send(send_buf[4]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            spi_send(dst[buf_index++]);
        }
        Flash_CS_1;

        delay10cyc();


        byte_addr += 256;
        w25qxx_wait_busy();
        //        w25qxx_write_disable();
    }
    //    SPI_WR_LOW();
    w25xx_out_4byte();

}

void w25qxx_write_1byte_4(uint32 addr , uint8 data)
{
    uint8 send_buf[5] ;
    send_buf[0] = SPI_4B_PAGE_PROG;
    send_buf[1] = (uint8)(addr>>24);
    send_buf[2] = (uint8)(addr>>16);
    send_buf[3] = (uint8)(addr>>8);
    send_buf[4] = (uint8)(addr);

    w25xx_in_4byte();
    w25qxx_write_enable();
    Flash_CS_0;
    spi_send(send_buf[0]);
    spi_send(send_buf[1]);
    spi_send(send_buf[2]);
    spi_send(send_buf[3]);
    spi_send(send_buf[4]);
    spi_send(data);

    Flash_CS_1;
    w25qxx_wait_busy();
    w25xx_out_4byte();
}

/*******************************************
*function : read a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_read_4(uint32 sector, uint8 * buffer, uint8 sector_number)
{
    uint8 send_buf[5];
    uint32 index,page_number,byte_addr,byte_number,buffer_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    w25xx_in_4byte();
    for(index=0 ; index<page_number ; index++)
    {

        send_buf[0] = SPI_4B_RD_DAT;
        send_buf[1] = (uint8)(byte_addr>>24);
        send_buf[2] = (uint8)(byte_addr>>16);
        send_buf[3] = (uint8)(byte_addr>>8);
        send_buf[4] = (uint8)(byte_addr);

        Flash_CS_0;
        spi_send(send_buf[0]);
        spi_send(send_buf[1]);
        spi_send(send_buf[2]);
        spi_send(send_buf[3]);
        spi_send(send_buf[4]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            buffer[buffer_index++] = spi_recv();
        }
        Flash_CS_1;

        delay10cyc();

        byte_addr += 256;
        w25qxx_wait_busy();
    }
    w25xx_out_4byte();
}

uint8 w25qxx_read_1byte_4(uint32 addr)
{
    uint8 ad[5] , data;

    ad[0] = SPI_4B_RD_DAT;
    ad[1] = (uint8)(addr>>24);
    ad[2] = (uint8)(addr>>16);
    ad[3] = (uint8)(addr>>8);
    ad[4] = (uint8)(addr);

    w25xx_in_4byte();
    Flash_CS_0;
    spi_send(ad[0]);
    spi_send(ad[1]);
    spi_send(ad[2]);
    spi_send(ad[3]);
    spi_send(ad[4]);
    data = spi_recv();
    Flash_CS_1;
    w25qxx_wait_busy();
    w25xx_out_4byte();
    return data;
}

void w25qxx_sector_erase_4(uint32 sector)
{
    uint8 send_buf[5];
    uint32 byte_addr;

    byte_addr = sector*4096;
    //the starting byte_addr must be
    if(ADDR_ERR == judge_erase_addr(byte_addr))
    {
        Debug("err,erase addr ivalid!\n");
        return;
    }else{
        //         Debug("erase addr valid!\n");
    }

    w25xx_in_4byte();
    SPI_WR_HIGH();
    w25qxx_write_enable();
    Flash_CS_0;
    send_buf[0] = SPI_4B_SECTOR_ERS_4K;
    send_buf[1] = ((byte_addr >>24 )&0x0F);
    send_buf[2] = (uint8)(byte_addr>>16);
    send_buf[3] = (uint8)(byte_addr>>8);
    send_buf[4] = (uint8)(byte_addr);
    //    Debug("erase address test:byte_addr:0x%08x\n",byte_addr);
    //    printbuf(send_buf,5);

    spi_send(send_buf[0]);
    spi_send(send_buf[1]);
    spi_send(send_buf[2]);
    spi_send(send_buf[3]);
    spi_send(send_buf[4]);
    Flash_CS_1;
    w25qxx_wait_busy();
    w25qxx_write_disable();
    SPI_WR_LOW();
    w25xx_out_4byte();

}

void w25qxx_section_erase_4(uint32 addr)
{
    uint8 send_buf[5];

    send_buf[0] = SPI_4B_SECTOR_ERS_4K;
    send_buf[1] = ((addr >>24 )&0x0F);
    send_buf[2] = (uint8)(addr>>16);
    send_buf[3] = (uint8)(addr>>8);
    send_buf[4] = (uint8)(addr);


    w25xx_in_4byte();
    SPI_WR_HIGH();
    w25qxx_write_enable();
    Flash_CS_0;
    spi_send(send_buf[0]);
    spi_send(send_buf[1]);
    spi_send(send_buf[2]);
    spi_send(send_buf[3]);
    spi_send(send_buf[4]);
    Flash_CS_1;
    w25qxx_wait_busy();
    w25qxx_write_disable();
    SPI_WR_LOW();
    w25xx_out_4byte();

}

/*
    Status Register-1
S7   S6 S5  S4  S3  S2  S1  S0
SRP0 TB BP3 BP2 BP1 BP0 WEL BUSY
*/
uint8 w25qxx_read_status_reg1(uint8 yesflag)
{
    uint8 rdata;

    Flash_CS_0;
    spi_send(SPI_RD_SR_1);
    rdata = spi_recv();
    Flash_CS_1;
    if(yesflag)
    {
        Debug("status_reg1:0x%02x\n",rdata);
        Debug("SRP0\tTB\tBP3\tBP2\tBP1\tBP0\tWEL\tBUSY\n");
        show_decTobin(rdata);
    }

    return(rdata);
}
/*
    Status Register-2
S15 S14 S13 S12 S11 S10 S9 S8
SUS CMP LB3 LB2 LB1 (R) QE SPR1
*/
uint8 w25qxx_read_status_reg2(uint8 yesflag)
{
    uint8 rdata;

    Flash_CS_0;
    spi_send(SPI_RD_SR_2);
    rdata = spi_recv();
    Flash_CS_1;
    if(yesflag)
    {
        Debug("status_reg2:0x%02x\n",rdata);
        Debug("SUS\tCMP\tLB3\tLB2\tLB1\t(R)\tQE\tSPR1\n");
        show_decTobin(rdata);
    }
    return(rdata);
}
/*
  Status Register-3
S23      S22  S21  S20 S19 S18 S17 S16
HOLD/RST DRV1 DRV0 (R) (R) WPS ADP ADS
*/
uint8 w25qxx_read_status_reg3(uint8 yesflag)
{
    uint8 rdata;

    Flash_CS_0;
    spi_send(SPI_RD_SR_3);
    rdata = spi_recv();
    Flash_CS_1;
    if(yesflag)
    {
        Debug("status_reg3:0x%02x\n",rdata);
        Debug("HOLD/RST DRV1\tDRV0\t(R)\t(R)\tWPS\tADP\tADS\n");
        show_decTobin(rdata);
    }
    return(rdata);
}
/*
    Status Register-1
S7   S6 S5  S4  S3  S2  S1  S0
SRP0 TB BP3 BP2 BP1 BP0 WEL BUSY
*/
void w25qxx_write_status_reg1(uint8 wdata)
{
    w25qxx_SR_write_enable();
    Flash_CS_0;
    spi_send(SPI_WR_SR_1);
    spi_send(wdata);
    Flash_CS_1;
}

/*
    Status Register-2
S15 S14 S13 S12 S11 S10 S9 S8
SUS CMP LB3 LB2 LB1 (R) QE SPR1
*/
void w25qxx_write_status_reg2(uint8 wdata)
{
    w25qxx_SR_write_enable();
    Flash_CS_0;
    spi_send(SPI_WR_SR_2);
    spi_send(wdata);
    Flash_CS_1;
}
/*
  Status Register-3
S23      S22  S21  S20 S19 S18 S17 S16
HOLD/RST DRV1 DRV0 (R) (R) WPS ADP ADS
*/
void w25qxx_write_status_reg3(uint8 wdata)
{
    w25qxx_SR_write_enable();
    Flash_CS_0;
    spi_send(SPI_WR_SR_3);
    spi_send(wdata);
    Flash_CS_1;
}

void set_SR_CMP(void)
{
    w25qxx_write_status_reg2(0x40);
}
void clr_SR_CMP(void)
{
    w25qxx_write_status_reg2(0x00);
}
void set_SR_WPS(void)
{
    w25qxx_write_status_reg3(0x04);
}
void clr_SR_WPS(void)
{
    w25qxx_write_status_reg3(0x00);
}

void w25qxx_chip_erase(void)
{
    Debug("start chip erase\n");


    EnterNoInt();
    SPI_WR_HIGH();
    w25qxx_write_enable();
    Flash_CS_0;
    spi_send(SPI_CHIP_ERS);
    Flash_CS_1;
    SPI_WR_LOW();
    w25qxx_wait_busy();
    ExitNoInt();


    Debug("chip erase done!!!!!!!!!!!\n");
    Debug("chip erase done!!!!!!!!!!!\n");
    Debug("chip erase done!!!!!!!!!!!\n");
    Debug("chip erase done!!!!!!!!!!!\n");
    Debug("chip erase done!!!!!!!!!!!\n");
    Debug("chip erase done!!!!!!!!!!!\n");
}

uint16 w25qxx_get_sector_num(void)
{

    uint8 id_recv[3];
    uint16 memory_type_capacity ,sector_count;


    /* read flash id */
    w25qxx_read_id(id_recv);

    if(id_recv[0] != MF_ID)
    {
        Debug("!!warning!manufacturer err!!!\n");
        /* Manufacturer’s ID error */
        return 0;
    }

    /* get memory type and capacity */
    memory_type_capacity = (id_recv[1] << 8) | id_recv[2];

    if(memory_type_capacity == MTC_W25Q128_BV)
    {

        sector_count = 4096;
    }
    else if(memory_type_capacity == MTC_W25Q64_BV_CV)
    {

        sector_count = 2048;
    }
    else if(memory_type_capacity == MTC_W25Q64_DW)
    {

        sector_count = 2048;
    }
    else if(memory_type_capacity == MTC_W25Q32_BV)
    {

        sector_count = 1024;
    }
    else if(memory_type_capacity == MTC_W25Q32_DW)
    {

        sector_count = 1024;
    }
    else if(memory_type_capacity == MTC_W25Q16_BV_CL_CV)
    {

        sector_count = 512;
    }
    else if(memory_type_capacity == MTC_W25Q16_DW)
    {

        sector_count = 512;
    }
    else if(memory_type_capacity == MTC_W25Q80_BV)
    {

        sector_count = 256;
    }
    else if(memory_type_capacity == MTC_W25Q256_FV)
    {

        sector_count = 512;//
    }

    else
    {
        sector_count = 0;

    }
    Debug("memory_type_capacity:0x%x\n",memory_type_capacity);
    sector_count *=8;
    return  sector_count;

}





void wx_read_jedec(void)
{
    unsigned char count = 0;
    unsigned char ids[3] = {0};

    Flash_CS_0;
    spi_send(SPI_JEDEC_ID);
    while(count < 3)
    {
        ids[count] =spi_recv();
        ++count;
    }
    Flash_CS_1;

    Debug("jedec :%x %x %x \n",ids[0],ids[1],ids[2]);

}

static void SPI_WR_HIGH(void)
{
    FIO2SET |= 1<<6;
}
static void SPI_WR_LOW(void)
{
    FIO2CLR |= 1<<6;
}
static void SPI_HOLD_HIGH(void)
{
    FIO2SET |= 1<<8;
}
static void SPI_HOLD_LOW(void)
{
    FIO2CLR |= 1<<8;
}
void SPI_CS_HIGH(void)
{
    FIO0MASK = 0x00000000;
    FIO0SET |= (0x1<<16);
}
void SPI_CS_LOW(void)
{
    FIO0MASK = 0x00000000;
    FIO0CLR |= (0x1<<16);
}
////FIOPIN为32位数
//uint32 pin016_status(void)
//{
//    Debug("0x1<<16 is:0x%x\n",0x1<<16);
//    return (FIO0PIN & (uint32)0x1<<16);
//}

void EnterNoInt(void)
{
    irq_disable_all();
}
void ExitNoInt(void)
{
    irq_enable_all();
}


//static void SPI_Flash_Write_Enable(void)
//{
//    SPI_Send_1Cmd(SPI_WR_EN);
//}
//static void SPI_Flash_Write_Disable(void)
//{
//    SPI_Send_1Cmd(SPI_WR_DIS);
//}
//static void SPI_Flash_VSR_Write_Enable(void)
//{
//    SPI_Send_1Cmd(SPI_VSR_WR_EN);
//}



//static void SPI_Flash_Chip_Erase(void)
//{
//    //还需要做的内容有
//    //清除保护位,CMP,TB,BP3,BP2,BP1,BP0
//    //允许写 enable write
//    //解锁 individual block/sector
//    /*the chip erase instruction will not be executed if
//    any memory region is protected by the block protect
//     (CMP,TB,BP3,BP2,BP1,BP0) bits or the Individual Block/Sector Locks
//    */
//    SPI_Send_1Cmd(SPI_CHIP_ERS);
//}
//void SPI_Flash_Erase_Program_Suspend(void)
//{/*
//the erase/program suspend instruction 75h will be accepted
//by the device only if the SUS bit in the Status Reg equals to 0 and the
//BUSY bit equal to 1 while a sector or block erase or
//a page program operation is on-going.

//*/
//    SPI_Send_1Cmd(SPI_ERS_PRO_SUSP);
//}
//void SPI_Flash_Erase_Program_Resume(void)
//{/*
//the erase/program suspend instruction 7Ah will be accepted
//by the device only if the SUS bit in the Status Reg equals to 1 and the
//BUSY bit equal to 0 .

//*/
//    SPI_Send_1Cmd(SPI_ERS_PRO_RES);
//}

//void SPI_Flash_Power_Down(void)
//{
//    SPI_Send_1Cmd(SPI_POWERDOWN);
//}
////可以多次获取
//uint32 SPI_Flash_Release_PowerDown_DevID(void)
//{
//    uint32 temp = 0;
//    Flash_CS_0;            //置cs低选中
//    spi_send(SPI_RELE_POWERDNID);
//    spi_send(0x00);     //这里发送的三个字节是任意的，不一定是0xff
//    spi_send(0x00);
//    spi_send(0x00);
//    temp|= spi_recv()<<24;
//    temp|= spi_recv()<<16;
//    temp|= spi_recv()<<8;
//    temp|= spi_recv();
//    Flash_CS_1;
//    ExitNoInt();
//    return temp;
//}
//uint16 SPI_Flash_MANU_DEVICE_ID(void)
//{
//    uint16 temp = 0;
//    Flash_CS_0;            //置cs低选中
//    spi_send(SPI_MANU_DEV_ID);
//    spi_send(0x00);     //这里发送的三个字节是任意的，不一定是0xff
//    spi_send(0x00);
//    spi_send(0x00);
//    temp|= spi_recv()<<8;
//    temp|= spi_recv();
//    Flash_CS_1;
//    ExitNoInt();
//    return temp;
//}
////这个怎么不对,结果为：
///*
//data[0]:0x18
//data[1]:0xef
//data[2]:0x18
//*/
//uint32 SPI_Flash_JEDEC_ID(void)
//{
//    uint32 temp = 0;
//    uint8 data[3]={0,};
//    int i=0;

//    Flash_CS_0;            //置cs低选中
//    spi_send(SPI_JEDEC_ID);
//    //    temp|= spi_recv()<<24;
//    //    temp|= spi_recv()<<16;
//    //    temp|= spi_recv()<<8;
//    data[i] = spi_recv();i++;
//    data[i] = spi_recv();i++;
//    data[i] = spi_recv();i++;

//    temp =(data[0]<<16)|(data[1]<<8)|(data[2]);

//    Flash_CS_1;
//    ExitNoInt();
//    //    for(i=0;i<sizeof(data);i++)
//    //    {
//    //        Debug("data[%d]:0x%x\n",i,data[i]);
//    //    }
//    return temp;
//}












/////********************************************
////**函 数 名：SPI_Flash_ReadUniqueID
////**功能描述：读取芯片唯一ID号，总共8字节
////*********************************************/
////uint8 SPI_Flash_ReadUniqueID(void)
////{
////    uint8 Temp = 0;
////    Flash_CS_0;
////    spi_send(CMD1_READ_UNIQUE_ID);//发送读取ID命令
////    spi_send(0x00);
////    spi_send(0x00);
////    spi_send(0x00);
////    spi_send(0x00);
////    Temp=spi_recv();
////    Flash_CS_1;
////    return Temp;

////}

/////********************************************
////**函 数 名：SPI_Flash_Read_JEDECID
////**功能描述：读取芯片JEDECID，总共24字节
////*********************************************/
////uint32 SPI_Flash_Read_JEDECID(void)
////{
////    uint32 Temp = 0;
////    Flash_CS_0;
////    spi_send(CMD1_JEDEC_ID);//发送读取ID命令
////    Temp|=spi_recv()<<16;
////    Temp|=spi_recv()<<8;
////    Temp|=spi_recv();
////    Flash_CS_1;
////    return Temp;

////}

/////********************************************
////**函 数 名：SPI_Flash_Read_ReleasePowerDownID
////**功能描述：读取芯片PowerDownID，总共8字节
////*********************************************/
////uint8 SPI_Flash_Read_ReleasePowerDownID(void)
////{
////    uint8 Temp = 0;
////    Flash_CS_0;
////    spi_send(CMD1_RELEASE_POWERDOWN_ID);//发送读取ID命令
////    spi_send(0x00);
////    spi_send(0x00);
////    spi_send(0x00);
////    Temp=spi_recv();
////    Flash_CS_1;
////    return Temp;
////}

/////********************************************
////**函 数 名：SPI_Flash_Read_Data
////**功能描述：读取flash内容，总共8字节
////*********************************************/
////uint8 SPI_Flash_Read_Data(uint32 address)
////{
////    uint8 Temp = 0;
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    Flash_CS_0;
////    spi_send(CMD1_READ_DATA);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    Temp=spi_recv();
////    Flash_CS_1;
////    return Temp;
////}
/////********************************************
////**函 数 名：SPI_Flash_Fast_Read
////**功能描述：读取flash内容，总共8字节
////*********************************************/
////uint8 SPI_Flash_Fast_Read(uint32 address)
////{
////    uint8 Temp = 0;
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_send(CMD1_FAST_READ);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    spi_send(0x00);
////    Temp=spi_recv();
////    Flash_CS_1;
////    return Temp;
////}


/////********************************************
////**函 数 名：SPI_Flash_Write_Enable
////**功能描述：允许读
////*********************************************/
////static void SPI_Flash_Write_Enable(void)
////{
////    SPI_Send_1Cmd(CMD1_WRITEENABLE);
////}
/////********************************************
////**函 数 名：SPI_Flash_Write_Disable
////**功能描述：禁止读
////*********************************************/
////static void SPI_Flash_Write_Disable(void)
////{
////    SPI_Send_1Cmd(CMD1_WRITEDISABLE);
////}
/////********************************************
////**函 数 名：SPI_Flash_Vo_SR_WriteEnable
////**功能描述：volatile SR wirte enable
////*********************************************/
////static void SPI_Flash_Vo_SR_WriteEnable(void)
////{
////    SPI_Send_1Cmd(CMD1_VSR_WRITEENABLE);
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Read_Status_Reg1                                     */
/////* 功能描述：读取 SR1的各种状态                                                  */
/////******************************************************************************/
////uint8 SPI_Flash_Read_Status_Reg1(void)
////{
////    uint8 busy_flag = 0;
////    SPI_CS_LOW();                     //置cs低选中
////    spi_send(CMD1_READ_SR_1);
////    busy_flag=spi_recv();
////    Flash_CS_1;                     //置cs高不选中

////    return (busy_flag);
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Read_Status_Reg2                                     */
/////* 功能描述：读取 SR1的各种状态                                                  */
/////******************************************************************************/
////uint8 SPI_Flash_Read_Status_Reg2(void)
////{
////    uint8 busy_flag = 0;
////    SPI_CS_LOW();                     //置cs低选中
////    spi_send(CMD1_READ_SR_2);
////    busy_flag=spi_recv();
////    SPI_CS_HIGH();                     //置cs高不选中

////    return (busy_flag);
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Write_Status_Reg                                     */
/////* 功能描述：写状态寄存器                                                  */
/////******************************************************************************/
////void SPI_Flash_Write_Status_Reg(uint16 cmd)
////{
////    SPI_CS_LOW();                     //置cs低选中
////    spi_send(CMD1_WRITE_SR);
////    spi_send((uint8)cmd);
////    spi_send((uint8)cmd>>8);
////    SPI_CS_HIGH();                     //置cs高不选中
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Page_Program                                     */
/////* 功能描述：写数据                                                  */
/////******************************************************************************/
////void SPI_Flash_Page_Program(uint32 address,
////                            uint8* data,
////                            uint16 len)
////{
////    uint8 addr[3]={0,};
////    uint16 num=0;
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    if(len > 256)
////    {
////        Debug("Warning!!page program maybe overwrite!!\n");
////    }

////    SPI_CS_LOW();
////    spi_send(CMD1_PAGE_PROGRAM);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    for(num=0;num<len;num++)
////    {
////        spi_send(*data++);
////    }
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Sector_Erase                                     */
/////* 功能描述：擦除一个扇区4K                                                  */
/////******************************************************************************/
////void SPI_Flash_Sector_Erase(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_send(CMD1_SECTOR_ERASE_4K);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Block_Erase32K                                     */
/////* 功能描述：擦除一个块32K                                                  */
/////******************************************************************************/
////void SPI_Flash_Block_Erase32K(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_send(CMD1_BLOCK_ERASE_32K);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* 函数名称：SPI_Flash_Block_Erase64K                                     */
/////* 功能描述：擦除一个块64K                                                  */
/////******************************************************************************/
////void SPI_Flash_Block_Erase64K(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_send(CMD1_BLOCK_ERASE_64K);
////    spi_send(addr[0]);
////    spi_send(addr[1]);
////    spi_send(addr[2]);
////    SPI_CS_HIGH();
////}

/////******************************************************************************/
/////* 函数名称：SPI_Flash_Chip_Erase                                     */
/////* 功能描述：擦除整个芯片                                                  */
/////******************************************************************************/
////void SPI_Flash_Chip_Erase(void)
////{
////    SPI_CS_LOW();
////    spi_send(CMD1_CHIP_ERASE);
////    SPI_CS_HIGH();
////}



/////*******************************************************************************
////* Function Name  : u8 W25Q64_CMD1B_READ1B(u8 cmd)
////* Description    : 发送一个字节指令并接受返回值
////*                  This function must be used only if the Start_Read_Sequence
////*                  function has been previously called.
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////RDSR Read Status Register
////*******************************************************************************/
////uint8 W25Q64_CMD1B_READ1B(uint8 cmd)
////{
////    uint8 tmp;
////    SPI_CS_LOW();
////    spi_send(cmd);
////    tmp = spi_recv();
////    SPI_CS_HIGH();
////    Debug("cmd:0x%x,val:%d\n",cmd,tmp);

////    return tmp;
////}

/////*******************************************************************************
////* Function Name  : void W25Q64_CMD1B_S1B(u16 cmd , u16 data)
////* Description    : 写一指令与一数据
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B_S1B(uint16 cmd , uint16 data)
////{
////    SPI_CS_LOW();
////    spi_send(cmd);
////    spi_send((uint8)data);
////    spi_send((uint8)(data>>8));
////    SPI_CS_HIGH();
////}
/////*******************************************************************************
////* Function Name  : void W25Q64_CMD1B_S1B(u16 cmd , u16 data)
////* Description    : 写一指令与两数据
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B_S2B(uint16 cmd , uint16 data,uint16 data2)
////{
////    SPI_CS_LOW();
////    spi_send(cmd);
////    spi_send((uint8)data);
////    spi_send((uint8)(data2));
////    SPI_CS_HIGH();
////}

/////*************************************]\******************************************
////* Function Name  : void W25Q64_CMD1B(u8 cmd)
////* Description    : 发送一个字节指令
////*                  This function must be used only if the Start_Read_Sequence
////*                  function has been previously called.
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B(uint8 cmd)
////{
////    SPI_CS_LOW();
////    spi_send(cmd);
////    SPI_CS_HIGH();
////}

/////*******************************************************************************
////* Function Name  : void W25Q64_WP_En(void)
////* Description    : 写使能
////*                  使能寄存器中的WEL位
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_Write_En(void)
////{
////    uint16 tmp;
////    //清除 Block Protect位：PB0，PB1，PB2，即取消所有区域保护
////    //1000 0011  reserve:0111 1100 7c res:~7c
////    tmp = W25Q64_CMD1B_READ1B(RDSR) & (~0x7c);  /*& (~0x7c);*/ // ~00011100  1110 0011
////    //    Debug("W25Q64_CMD1B_READ1B:%d\n",tmp);
////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp);
////    // Protected Area Upper sixteenth (Sector 15)以及WEL位
////    W25Q64_CMD1B(WRITENABLE);//write enable
////    SPI_WR_HIGH();  //write Protection diable .  added by xie
////}
/////*******************************************************************************
////* Function Name  : void W25Q64_Write_CHIPEARSE_En(void)
////* Description    : 写使能
////*                  使能寄存器中的WEL位
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_Write_CHIPEARSE_En(void)
////{
////    uint8 tmp,tmp2;

////    W25Q64_CMD1B(WRITENABLE);
////    //清除 SEC,TB位，清除 Block Protect位：PB0，PB1，PB2，即取消所有区域保护
////    //1000 0011  reserve:0111 1100 7c res:~7c
////    tmp = W25Q64_CMD1B_READ1B(RDSR) & (0x83);  /*& (~0x7c);*/ // ~00011100  1110 0011
////    //1011 1111  reverse:0100 0000 40 rev:~40
////    tmp2= W25Q64_CMD1B_READ1B(RDSR2) & (0xbf);
////    //    Debug("W25Q64_CMD1B_READ1B:%d\n",tmp);
//////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/

////    Debug("tmp :%d\n",tmp);
////    Debug("tmp2:%d\n",tmp2);
////    Debug("tmp|tmp2<<8:%d\n",tmp|(tmp2<<8));
////    W25Q64_CMD1B_S2B(WRSR, tmp,tmp2);
////    // Protected Area Upper sixteenth (Sector 15)以及WEL位


//////    Debug("==>>read conf result!\n");
//////    W25Q64_CMD1B_READ1B(RDSR);
////    SPI_WR_HIGH();  //write Protection diable .  added by xie
////}


/////*******************************************************************************
////* Function Name  : void Flash_WP_En(void)
////* Description    : 写禁止
////*                  使能寄存器中的WEL位
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_WP_En(void)
////{
////    uint16 tmp;
////    //添加PB0，PB1，PB2的保护位
////    tmp = W25Q64_CMD1B_READ1B(RDSR) | 0x1c;
////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp);

////    W25Q64_CMD1B(WRDI);
////    SPI_WR_LOW();          //write Protection enable .  added by xie
////}
/////*******************************************************************************
////* Function Name  : void Flash_WP_En(void)
////* Description    : 写禁止
////*                  使能寄存器中的WEL位
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_WP_CHIPEARSE_En(void)
////{
////    uint8 tmp,tmp2;
////    //添加PB0，PB1，PB2的保护位
////    tmp = W25Q64_CMD1B_READ1B(RDSR) | 0x1c;
////    tmp2 = W25Q64_CMD1B_READ1B(RDSR2) | 0x40;

////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp&(tmp2<<8));

////    W25Q64_CMD1B(WRDI);
////    SPI_WR_LOW();          //write Protection enable .  added by xie
////}


/////********************************************************
////** 函 数 名：SectorErase 4k
////** 功能描述：扇区擦除
////** 参    数：扇区地址
////** 返    回：无
////*********************************************************/
////void Sector_4K_Erase(uint32 address)
////{
////    uint8 ad[3]/* , data*/;

////    ad[0] = (address & 0x00ff0000) >> 16;
////    ad[1] = (address & 0x0000ff00) >> 8;
////    ad[2] = (address & 0x000000ff);

////    // EnterNoInt();
////    W25Q64_Write_En();
////    SPI_CS_LOW();                      //置cs低选中
////    spi_send(0x20);
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    SPI_CS_HIGH();                      //置cs高不选中
////    while(Check_Flash_Busy());       //等待器件写完结束
////    W25Q64_WP_En();
////    // ExitNoInt();
////}
/////********************************************************
////** 函 数 名：SectorErase 32k
////** 功能描述：扇区擦除
////** 参    数：扇区地址
////** 返    回：无
////*********************************************************/
////void Sector_32K_Erase(uint32 address)
////{
////    // EnterNoInt();
////    W25Q64_Write_En();					//写使能
////    SPI_CS_LOW();                      //置cs低选中
////    spi_send(0x52);
////    spi_send((uint8)(address>>16));
////    spi_send((uint8)(address>>8));
////    spi_send((uint8)address);
////    SPI_CS_HIGH();                      //置cs高不选中
////    while(Check_Flash_Busy());       //等待器件写完结束
////    W25Q64_WP_En();
////    // ExitNoInt();
////}
/////********************************************************
////** 函 数 名：SectorErase 64k
////** 功能描述：扇区擦除
////** 参    数：扇区地址
////** 返    回：无
////*********************************************************/
////void Sector_64K_Erase(uint32 address)
////{
////#if 1
////    uint8 ad[3]/* , data*/;

////    ad[0] = (address & 0x00ff0000) >> 16;
////    ad[1] = (address & 0x0000ff00) >> 8;
////    ad[2] = (address & 0x000000ff);

////    // EnterNoInt();
////    //    W25Q64_Write_En();
////    W25Q64_Write_CHIPEARSE_En();
////    SPI_CS_LOW();                      //置cs低选中
////    spi_send(0xD8);//64k erase
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    SPI_CS_HIGH();                      //置cs高不选中
////    while(Check_Flash_Busy());       //等待器件写完结束
////    //    W25Q64_WP_En();
////    W25Q64_WP_CHIPEARSE_En();
////    // ExitNoInt();
////#else
////    // EnterNoInt();
////    W25Q64_Write_En();					//写使能
////    SPI_CS_LOW();                      //置cs低选中
////    spi_send(0xD8);
////    spi_send((uint8)(address>>16));
////    spi_send((uint8)(address>>8));
////    spi_send((uint8)address);
////    SPI_CS_HIGH();                      //置cs高不选中
////    while(Check_Flash_Busy());       //等待器件写完结束
////    W25Q64_WP_En();
////    // ExitNoInt();

////#endif
////}

/////******************************************************************************
////** 函 数 名：FlashProgram
////** 功能描述: 向指定的地址写入指定数量的字节
////** 参    数：数据缓冲区，字节数，起始地址
////** 返    回：无
////******************************************************************************/
////void FlashProgram(uint8 *databuffer,uint16 datasize,uint32 address)
////{
////    uint8 ad[3]/* , data*/;


////    ad[0] = (address & 0x00ff0000) >> 16;
////    ad[1] = (address & 0x0000ff00) >> 8;
////    ad[2] = (address & 0x000000ff);

////    //  EnterNoInt();
////    W25Q64_Write_En();					//写使能
////    SPI_CS_LOW();                      //置cs低选中
////    spi_send(0x02);
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    while(datasize>0)
////    {
////        spi_send(*databuffer++);
////        datasize--;
////    }
////    SPI_CS_HIGH();                      //置cs高不选中


////    while(Check_Flash_Busy());       //等待器件写完结束
////    W25Q64_WP_En();

////    //   ExitNoInt();
////}
/////*******************************************************************************
////* Function Name  : void M25P80_Write_1Byte(u32 addr , u8 data)
////* Description    : 写数据
////*                  写固定地址的一位数据
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void M25P80_Write_1Byte(uint32 addr , uint8 data)
////{
////    uint8 ad[3] ;
////    ad[0] = (addr & 0x00ff0000) >> 16;
////    ad[1] = (addr & 0x0000ff00) >> 8;
////    ad[2] = (addr & 0x000000ff);


////    W25Q64_Write_En();

////    SPI_CS_LOW();
////    spi_send(PAGE_PROG);
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    spi_send(data);//根据指令集，发送两次
////    spi_send(data);
////    SPI_CS_HIGH();


////    while(Check_Flash_Busy());
////    W25Q64_WP_En();
////}
/////*******************************************************************************
////* Function Name  : void ChipErase(void)
////* Description    : 写数据
////*                  写固定地址的一位数据
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void ChipErase(void)
////{
////    //    while(Check_Flash_Busy());

////    W25Q64_Write_CHIPEARSE_En();
////    SPI_CS_LOW();
////    spi_send(0x60);
////    SPI_CS_HIGH();

////    while(Check_Flash_Busy());
////    W25Q64_WP_CHIPEARSE_En();

////}

/////*****************************************************************
////** 函 数 名：ReadRFlash
////** 功能描述：从指定的地址读取指定数量的字节
////** 参    数：数据缓冲区，字节数，起始地址
////** 返    回：无
////******************************************************************/
////void ReadFlash(uint8 *databuffer,uint16 datasize,uint32 address)
////{
////    uint16 i;
////    // EnterNoInt();
////    SPI_CS_LOW();
////    spi_send(0x03);
////    spi_send((uint8)(address>>16));
////    spi_send((uint8)(address>>8));
////    spi_send((uint8)address);
////    for(i=0;i<datasize;i++)
////    {
////        databuffer[i]=spi_recv();
////    }
////    SPI_CS_HIGH();                 //置cs高不选中
////    //ExitNoInt();
////}
/////*******************************************************************************
////* Function Name  : u8 W25Q64_Read_1Byte(u32 addr )
////* Description    : 读数据
////*                  读固定地址的一位数据
////* Input          : None
////* Output         : None
////* Return         : return data;
////*******************************************************************************/
////uint8 ReadFlash_1Byte(uint32 addr)
////{
////#if 0
////    uint8 ad[3] , data;

////    ad[0] = (addr & 0x00ff0000) >> 16;
////    ad[1] = (addr & 0x0000ff00) >> 8;
////    ad[2] = (addr & 0x000000ff);

////    SPI_CS_LOW();//选中
////    spi_send(READ);
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    data = spi_recv();
////    SPI_CS_HIGH();//高阻
////    return data;
////#else
////    uint8 ad[3] , data;



////    ad[0] = (addr & 0x00ff0000) >> 16;
////    ad[1] = (addr & 0x0000ff00) >> 8;
////    ad[2] = (addr & 0x000000ff);

////    SPI_CS_LOW();//选中
////    spi_send(READ);
////    spi_send(ad[0]);
////    spi_send(ad[1]);
////    spi_send(ad[2]);
////    data = spi_recv();
////    SPI_CS_HIGH();//高阻
////    //    Debug("reveive data:%d\n",data);
////    return data;
////#endif
////}

/////**************************************************************************
////** 函数名：CheckData
////** 功能描述：检验指定的Flash起始地址的数据和指定的缓冲区的数据是否全部相等
////** 参数：数据缓冲区DataBuffer, Flash起始地址DataAddr，数据字节数
////** 返回：全部相同返回1，否则返回0
////***************************************************************************/
////uint8 CheckData(uint8 *DataBuffer, uint32 DataAddr, uint16 DataSize)
////{
////    uint8 data;
////    uint16 i;
////    for(i=0;i<DataSize;i++)
////    {
////        ReadFlash(&data,1,DataAddr+i);
////        if(data!=DataBuffer[i])return 0;
////    }
////    return 1;
////}



///////////////////////////////////////////////////////


//////static uint8  Check_Flash_Busy_W25Q(void)
//////{

//////    uint8 busy_flag;

//////    //    DrvSYS_Delay(2);
//////    Delay1Ms(100);

//////    SPI_CS_LOW();

//////    spi_send(0x05);

//////    busy_flag=spi_recv();

//////    SPI_CS_HIGH();

//////    Debug("busy_flag=0x%x\n",busy_flag);

//////    if(busy_flag & 0x01)
//////    {
//////        //if WEL or BUSY=1 ,busy_flag=1
//////        busy_flag = 1;
//////    }
//////    else
//////    {
//////        //if WEL and BUSY=0 ,busy_flag=0
//////        busy_flag = 0;
//////    }

//////    return busy_flag;

//////}




//////void ReadManufactDeviceID_90h_W25Q(void)
//////{//

//////    uint32 val ;

//////    uint32 i ;

//////    Check_Flash_Busy_W25Q();

//////    SPI_CS_LOW();

//////    spi_send(0x90);

//////    spi_send(0x00);

//////    spi_send(0x00);

//////    spi_send(0x00);



//////    Debug("90h\n");

//////    for(i=0;i<2;i++)
//////    {
//////        val=spi_recv();

//////        Debug("val=0x%x---%d\n",val,i);

//////    }

//////    SPI_CS_HIGH();
//////}


