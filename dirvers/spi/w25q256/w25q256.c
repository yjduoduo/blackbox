#include "includeall.h"
#include "w25q256.h"


#define Flash_CS_1    SPI_CS_HIGH();
#define Flash_CS_0    SPI_CS_LOW();
#define Flash_Hold_1  SPI_HOLD_HIGH();
#define Flash_Hold_0  SPI_HOLD_LOW();


void SPI0_init(void) 
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

    //cs---GPIO
    PINSEL1 &= ~(0x3<<0);//clear P0.16
    PINSEL1 |= (0x0<<0);//P0.16,Func[0]
    FIO0DIR |= (0x1<<16);//dir P0.16,output
    FIO0SET |= (0x1<<16);//level P0.16,high

    //sck0
    PINSEL0 &= ~(0x1<<30);//clear P0.15
    PINSEL0 |= (unsigned int)(0x2<<30);//P0.15,Func[2]
    //miso0
    PINSEL1 &= ~(0x3<<2);//clear P0.17
    PINSEL1 |= (0x2<<2);//P0.17,Func[2]
    //mosi0
    PINSEL1 &= ~(0x3<<4);//clear P0.18
    PINSEL1 |= (0x2<<4);//P0.18,Func[2]

    SPI_WR_LOW();//WR����
    SPI_HOLD_HIGH();//HLOD����
    SPI_CS_HIGH();//CS���� ��ѡ

    SSP0CR0 = (0x00 << 8)               	/* SCR  ����SPIʱ�ӷ�Ƶ	        */
            | (0x00 << 7)                	/* CPHA ʱ�������λ	        */
            | (0x00 << 6)                   /* CPOL ʱ���������            */
            | (0x00 << 4)                   /* FRF  ֡��ʽ 00=SPI           */
            | (0x07 << 0);                  /* DSS  ���ݳ���,0111=8λ       */


    SSP0CR1 = (0x00 << 3)                    /* SOD  �ӻ��������,0=����	    */
            | (0x00 << 2)                    /* MS   ����ѡ��,0=����,1=�ӻ�  */
            | (0x01 << 1)                    /* SSE  SSPʹ��                 */
            | (0x00 << 0);                   /* LBM  ��дģʽ	            */
  

  /* ����SSP��PCLK��õķ�Ƶֵ,   */
    SSP0CPSR = 0x52;                         /* ��ģʽ����Ч, ��СֵΪ0x02   */

    SPI_CS_HIGH();//CS����

}

/*********************************************************************************************************
** Function name:		SSP_SendData
** Descriptions:		SSP�ӿ���SSP���߷�������
** input parameters:	data        �����͵�����
** output parameters:	����ֵΪ��ȡ������
*********************************************************************************************************/
uint8 spi_flash_send_byte(uint8 tdata)
{
#if 1
    /* Wait until the transmit buffer is empty,
 1:empty,go*/
//     while (!(SSP0SR & SSP_SR_TFE));
    /* Send the byte */
    SSP0DR = tdata;
    /* Wait until a data is received */
//     while (!(SSP0SR & SSP_SR_TFE));
    while (SSP0SR & BSY);
    /* Get the received data */
    return (SSP0DR);
    #else
    
    FIO0CLR |= SLAVE_CS; 
    S0SPDR  = data;
    while (0 == (S0SPSR & 0x80));			// �̨���ySPIF????��??��̨���y��y?Y����?��������?
    S0SPSR  = 0;							// ??3ySPIF����??
    FIO0SET |= SLAVE_CS; 
    
    
    
    #endif
}

// /*****************************************************************
// ** function name  :	 spi_flash_receive_byte
// ** description    :  SPI1 receive byte
// ******************************************************************/
// uint8 spi_flash_receive_byte(void)
// {
//     //    uint8 rdata;
//     //    uint16 timeout=WaitTimeOut;

//     //    while((SSP0DR & SSP_SR_TNF));
//     /* Wait until the transmit buffer is empty,
//  1:empty,go*/
//     //��ѯ״̬�Ĵ����ķ���FIFOΪ�գ�Ϊ1���������
// //     while (!(SSP0SR & SSP_SR_TFE));
//     /* Send the byte */
//     SSP0DR = 0xff;
//     //    timeout=WaitTimeOut;
//     /* Wait until a data is received */
// //    while (!(SSP0SR & SSP_SR_TFE));
// //     while (!(SSP0SR & SSP_SR_RNE));
//     while (SSP0SR & BSY);

//     /* Get the received data */
//     return (SSP0DR);
// }

uint8 w25qxx_read_status_reg1(void)
{
    uint8 rdata;

    Flash_CS_0;
    spi_flash_send_byte(SPI_RD_SR_1);
    rdata = spi_flash_send_byte(0xff);
    Flash_CS_1;
    return(rdata);
}
uint8 w25qxx_read_status_reg2(void)
{
    uint8 rdata;

    Flash_CS_0;
    spi_flash_send_byte(SPI_RD_SR_2);
    rdata = spi_flash_send_byte(0xff);
    Flash_CS_1;
    return(rdata);
}
uint8 w25qxx_read_status_reg3(void)
{
    uint8 rdata;

    Flash_CS_0;
    spi_flash_send_byte(SPI_RD_SR_3);
    rdata = spi_flash_send_byte(0xff);
    Flash_CS_1;
    return(rdata);
}


void w25qxx_read_id(uint8 *buffer)
{
    Flash_CS_0;
    spi_flash_send_byte(SPI_JEDEC_ID);
    *buffer = spi_flash_send_byte(0xff);
    buffer++;
    *buffer = spi_flash_send_byte(0xff);
    buffer++;
    *buffer = spi_flash_send_byte(0xff);
    Flash_CS_1;
}

void w25qxx_wait_busy(void)
{
    while( w25qxx_read_status_reg1() & (0x01));
}

void w25qxx_write_enable(void)
{
    Flash_CS_0;
    spi_flash_send_byte(SPI_WR_EN);
    Flash_CS_1;
}

void w25qxx_write_disable(void)
{
    Flash_CS_0;
    spi_flash_send_byte(SPI_WR_DIS);
    Flash_CS_1;
}

/*******************************************
*function : write a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_write(uint32 sector, uint8 * buffer, uint8 sector_number)
{
    uint8 send_buffer[4];
    uint32 index,page_number,byte_addr,byte_number,buffer_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    for(index=0 ; index<page_number ; index++)
    {
        send_buffer[0] = SPI_3B_PAGE_PROG;
        send_buffer[1] = (uint8)(byte_addr>>16);
        send_buffer[2] = (uint8)(byte_addr>>8);
        send_buffer[3] = (uint8)(byte_addr);
        w25qxx_write_enable();
        Flash_CS_0;
        spi_flash_send_byte(send_buffer[0]);
        spi_flash_send_byte(send_buffer[1]);
        spi_flash_send_byte(send_buffer[2]);
        spi_flash_send_byte(send_buffer[3]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            spi_flash_send_byte(buffer[buffer_index++]);
        }
        Flash_CS_1;

        byte_addr += 256;
        w25qxx_wait_busy();
    }

}

/*******************************************
*function : read a sector (512byte)
*input      sector
*           buffer
*           sector_number
* output  : none
*******************************************/
void w25qxx_sector_read(uint32 sector, uint8 * buffer, uint8 sector_number)
{
    uint8 send_buffer[4];
    uint32 index,page_number,byte_addr,byte_number,buffer_index=0;

    page_number = sector_number*2;
    byte_addr = sector*512;

    for(index=0 ; index<page_number ; index++)
    {

        send_buffer[0] = SPI_3B_RD_DAT;
        send_buffer[1] = (uint8)(byte_addr>>16);
        send_buffer[2] = (uint8)(byte_addr>>8);
        send_buffer[3] = (uint8)(byte_addr);

        Flash_CS_0;
        spi_flash_send_byte(send_buffer[0]);
        spi_flash_send_byte(send_buffer[1]);
        spi_flash_send_byte(send_buffer[2]);
        spi_flash_send_byte(send_buffer[3]);

        for(byte_number=0;byte_number<256;byte_number++)
        {
            buffer[buffer_index++] = spi_flash_send_byte(0xff);
        }
        Flash_CS_1;

        byte_addr += 256;
        w25qxx_wait_busy();
    }
}


void w25qxx_sector_erase(uint32 sector)
{
    uint8 send_buffer[4];
    uint32 byte_addr;

    byte_addr = sector*4096;

    w25qxx_write_enable();
    Flash_CS_0;
    send_buffer[0] = SPI_3B_SECTOR_ERS_4K;
    send_buffer[1] = (uint8)(byte_addr >> 16);
    send_buffer[2] = (uint8)(byte_addr >> 8);
    send_buffer[3] = (uint8)(byte_addr);
    spi_flash_send_byte(send_buffer[0]);
    spi_flash_send_byte(send_buffer[1]);
    spi_flash_send_byte(send_buffer[2]);
    spi_flash_send_byte(send_buffer[3]);
    Flash_CS_1;

    w25qxx_wait_busy();

}

uint16 w25qxx_get_sector_num(void)
{

    uint8 id_recv[3];
    uint16 memory_type_capacity ,sector_count;


    /* read flash id */
    w25qxx_read_id(id_recv);

    if(id_recv[0] != MF_ID)
    {
        lcd_printf("!!warning!manufacturer err!!!\n");
        /* Manufacturer��s ID error */
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
    lcd_printf("memory_type_capacity:0x%x\n",memory_type_capacity);
    sector_count *=8;
    return  sector_count;

}





void wx_read_jedec(void)
{
    unsigned char count = 0;
    unsigned char ids[3] = {0};

    Flash_CS_0;
    spi_flash_send_byte(SPI_JEDEC_ID);
    //    while(spi_flash_send_byte(0xff)&0x02);

    while(count < 3)
    {
        //        spi_flash_send_byte(0x00);
        //        while(spi_flash_send_byte(0xff)&0x02);
        ids[count] =spi_flash_send_byte(0xff);
        ++count;
    }
    Flash_CS_1;

    lcd_printf("jedec :%x %x %x \n",ids[0],ids[1],ids[2]);

}


//Flash_Hold_1;
//Flash_CS_0;
//spi_flash_send_byte(SPI_JEDEC_ID);
//*buffer = spi_flash_send_byte(0xff);
//buffer++;
//*buffer = spi_flash_send_byte(0xff);
//buffer++;
//*buffer = spi_flash_send_byte(0xff);
//Flash_CS_1;
//Flash_Hold_0;














#if 1
/******************************************************************************/
/* �������ƣ�uint8 check_flash_busy(void)                                     */
/* �������������flash�Ƿ�æ                                                  */
/* ����˵������                                                               */
/* ��������: ���æ����1����æ����0                                           */
/******************************************************************************/
static uint8 Check_Flash_Busy(void)
{
    uint8 busy_flag = 0;
    Flash_CS_0;                     //��cs��ѡ��
    spi_flash_send_byte(QPI_RD_SR_1);
    busy_flag=spi_flash_send_byte(0xff);
    Flash_CS_1;                     //��cs�߲�ѡ��

    return (busy_flag & 0x01);
}
#else
/*******************************************************************************
* Function Name  : u8 W25Q64_Busy(void)
* Description    : ״̬���
*                  ���λWIPΪ1�����ڹ���
* Input          : None
* Output         : None
* Return         : return (sta & 0x01);
*******************************************************************************/
static uint8 Check_Flash_Busy(void)
{
    uint8 tmp,tmp1;
    tmp1=5;
    Flash_CS_0;
    spi_flash_send_byte(RDSR);
    tmp = spi_flash_send_byte(0XFF);
    Flash_CS_1;
    for(;tmp1>0;tmp1--);
    //    lcd_printf("busy:%d\n",(tmp & 0x01));
    return (tmp & 0x01);
}
#endif




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
    //    Delay1Ms(10000);
    FIO0MASK = 0x00000000;
    FIO0SET |= (0x1<<16);
}
void SPI_CS_LOW(void)
{
    //    uint16 i;
    //    for(i=0;i<2000;i++)
    FIO0MASK = 0x00000000;
    FIO0CLR |= (0x1<<16);
    //    Delay1Ms(10000);
}
//FIOPINΪ32λ��
uint32 pin016_status(void)
{
    lcd_printf("0x1<<16 is:0x%x\n",0x1<<16);
    return (FIO0PIN & (uint32)0x1<<16);
}

void EnterNoInt(void)
{

}
void ExitNoInt(void)
{

}












///********************************************
//**�� �� ����SPI_Flash_Send_1Cmd
//**��������������һ������
//*********************************************/
//static void SPI_Flash_Send_1Cmd(uint8 cmd)
//{
//    Flash_CS_0;
//    spi_flash_send_byte(cmd);
//    Flash_CS_1;
//}
//static void ENTER_QPI_MOD(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_ENTER_QPI_MOD);
//}

uint16 SPI_Flash_Read_MANU_DEVICE_ID(void)
{
    uint16 temp = 0;
    Flash_CS_0;            //��cs��ѡ��
    spi_flash_send_byte(SPI_MANU_DEV_ID);
    spi_flash_send_byte(0x00);     //���﷢�͵������ֽ�������ģ���һ����0xff
    spi_flash_send_byte(0x00);
    spi_flash_send_byte(0x00);
    temp|= spi_flash_send_byte(0xff)<<8;
    temp|= spi_flash_send_byte(0xff);
    Flash_CS_1;
    ExitNoInt();
    return temp;
}



////��ȡоƬID
////����ֵ����:
////0XEF13,��ʾоƬ�ͺ�ΪW25Q80
////0XEF14,��ʾоƬ�ͺ�ΪW25Q16
////0XEF15,��ʾоƬ�ͺ�ΪW25Q32
////0XEF16,��ʾоƬ�ͺ�ΪW25Q64
////0XEF18, W25Q64FVSFIF
////uint16 SPI_Flash_Read_MANU_DEVICE_ID(void)
////{
////    uint16 Temp = 0;

////    Flash_CS_0;
////    spi_flash_send_byte(0x90);//���Ͷ�ȡID����
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    Temp|=spi_flash_send_byte(0xff)<<8;
////    Temp|=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;
////}
//static void SPI_Flash_Enter_4byte_mode(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_ENT_ADDRMOD_4B);
//}
//static void SPI_Flash_Exit_4byte_mode(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_EXIT_ADDRMOD_4B);
//}
//static void SPI_Flash_Write_Enable(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_WR_EN);
//}
//static void SPI_Flash_Write_Disable(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_WR_DIS);
//}
//static void SPI_Flash_VSR_Write_Enable(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_VSR_WR_EN);
//}
//static void SPI_Flash_Chip_Erase(void)
//{
//    //����Ҫ����������
//    //�������λ,CMP,TB,BP3,BP2,BP1,BP0
//    //����д enable write
//    //���� individual block/sector
//    /*the chip erase instruction will not be executed if
//    any memory region is protected by the block protect
//     (CMP,TB,BP3,BP2,BP1,BP0) bits or the Individual Block/Sector Locks
//    */
//    SPI_Flash_Send_1Cmd(SPI_CHIP_ERS);
//}
//void SPI_Flash_Erase_Program_Suspend(void)
//{/*
//the erase/program suspend instruction 75h will be accepted
//by the device only if the SUS bit in the Status Reg equals to 0 and the
//BUSY bit equal to 1 while a sector or block erase or
//a page program operation is on-going.

//*/
//    SPI_Flash_Send_1Cmd(SPI_ERS_PRO_SUSP);
//}
//void SPI_Flash_Erase_Program_Resume(void)
//{/*
//the erase/program suspend instruction 7Ah will be accepted
//by the device only if the SUS bit in the Status Reg equals to 1 and the
//BUSY bit equal to 0 .

//*/
//    SPI_Flash_Send_1Cmd(SPI_ERS_PRO_RES);
//}

//void SPI_Flash_Power_Down(void)
//{
//    SPI_Flash_Send_1Cmd(SPI_POWERDOWN);
//}
////���Զ�λ�ȡ
//uint32 SPI_Flash_Release_PowerDown_DevID(void)
//{
//    uint32 temp = 0;
//    Flash_CS_0;            //��cs��ѡ��
//    spi_flash_send_byte(SPI_RELE_POWERDNID);
//    spi_flash_send_byte(0x00);     //���﷢�͵������ֽ�������ģ���һ����0xff
//    spi_flash_send_byte(0x00);
//    spi_flash_send_byte(0x00);
//    temp|= spi_flash_send_byte(0xff)<<24;
//    temp|= spi_flash_send_byte(0xff)<<16;
//    temp|= spi_flash_send_byte(0xff)<<8;
//    temp|= spi_flash_send_byte(0xff);
//    Flash_CS_1;
//    ExitNoInt();
//    return temp;
//}
//uint16 SPI_Flash_MANU_DEVICE_ID(void)
//{
//    uint16 temp = 0;
//    Flash_CS_0;            //��cs��ѡ��
//    spi_flash_send_byte(SPI_MANU_DEV_ID);
//    spi_flash_send_byte(0x00);     //���﷢�͵������ֽ�������ģ���һ����0xff
//    spi_flash_send_byte(0x00);
//    spi_flash_send_byte(0x00);
//    temp|= spi_flash_send_byte(0xff)<<8;
//    temp|= spi_flash_send_byte(0xff);
//    Flash_CS_1;
//    ExitNoInt();
//    return temp;
//}
////�����ô����,���Ϊ��
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

//    Flash_CS_0;            //��cs��ѡ��
//    spi_flash_send_byte(SPI_JEDEC_ID);
//    //    temp|= spi_flash_send_byte(0xff)<<24;
//    //    temp|= spi_flash_send_byte(0xff)<<16;
//    //    temp|= spi_flash_send_byte(0xff)<<8;
//    data[i] = spi_flash_send_byte(0xff);i++;
//    data[i] = spi_flash_send_byte(0xff);i++;
//    data[i] = spi_flash_send_byte(0xff);i++;

//    temp =(data[0]<<16)|(data[1]<<8)|(data[2]);

//    Flash_CS_1;
//    ExitNoInt();
//    //    for(i=0;i<sizeof(data);i++)
//    //    {
//    //        lcd_printf("data[%d]:0x%x\n",i,data[i]);
//    //    }
//    return temp;
//}












/////********************************************
////**�� �� ����SPI_Flash_ReadUniqueID
////**������������ȡоƬΨһID�ţ��ܹ�8�ֽ�
////*********************************************/
////uint8 SPI_Flash_ReadUniqueID(void)
////{
////    uint8 Temp = 0;
////    Flash_CS_0;
////    spi_flash_send_byte(CMD1_READ_UNIQUE_ID);//���Ͷ�ȡID����
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    Temp=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;

////}

/////********************************************
////**�� �� ����SPI_Flash_Read_JEDECID
////**������������ȡоƬJEDECID���ܹ�24�ֽ�
////*********************************************/
////uint32 SPI_Flash_Read_JEDECID(void)
////{
////    uint32 Temp = 0;
////    Flash_CS_0;
////    spi_flash_send_byte(CMD1_JEDEC_ID);//���Ͷ�ȡID����
////    Temp|=spi_flash_send_byte(0xff)<<16;
////    Temp|=spi_flash_send_byte(0xff)<<8;
////    Temp|=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;

////}

/////********************************************
////**�� �� ����SPI_Flash_Read_ReleasePowerDownID
////**������������ȡоƬPowerDownID���ܹ�8�ֽ�
////*********************************************/
////uint8 SPI_Flash_Read_ReleasePowerDownID(void)
////{
////    uint8 Temp = 0;
////    Flash_CS_0;
////    spi_flash_send_byte(CMD1_RELEASE_POWERDOWN_ID);//���Ͷ�ȡID����
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    spi_flash_send_byte(0x00);
////    Temp=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;
////}

/////********************************************
////**�� �� ����SPI_Flash_Read_Data
////**������������ȡflash���ݣ��ܹ�8�ֽ�
////*********************************************/
////uint8 SPI_Flash_Read_Data(uint32 address)
////{
////    uint8 Temp = 0;
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    Flash_CS_0;
////    spi_flash_send_byte(CMD1_READ_DATA);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    Temp=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;
////}
/////********************************************
////**�� �� ����SPI_Flash_Fast_Read
////**������������ȡflash���ݣ��ܹ�8�ֽ�
////*********************************************/
////uint8 SPI_Flash_Fast_Read(uint32 address)
////{
////    uint8 Temp = 0;
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_FAST_READ);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    spi_flash_send_byte(0x00);
////    Temp=spi_flash_send_byte(0xff);
////    Flash_CS_1;
////    return Temp;
////}


/////********************************************
////**�� �� ����SPI_Flash_Write_Enable
////**���������������
////*********************************************/
////static void SPI_Flash_Write_Enable(void)
////{
////    SPI_Flash_Send_1Cmd(CMD1_WRITEENABLE);
////}
/////********************************************
////**�� �� ����SPI_Flash_Write_Disable
////**������������ֹ��
////*********************************************/
////static void SPI_Flash_Write_Disable(void)
////{
////    SPI_Flash_Send_1Cmd(CMD1_WRITEDISABLE);
////}
/////********************************************
////**�� �� ����SPI_Flash_Vo_SR_WriteEnable
////**����������volatile SR wirte enable
////*********************************************/
////static void SPI_Flash_Vo_SR_WriteEnable(void)
////{
////    SPI_Flash_Send_1Cmd(CMD1_VSR_WRITEENABLE);
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Read_Status_Reg1                                     */
/////* ������������ȡ SR1�ĸ���״̬                                                  */
/////******************************************************************************/
////uint8 SPI_Flash_Read_Status_Reg1(void)
////{
////    uint8 busy_flag = 0;
////    SPI_CS_LOW();                     //��cs��ѡ��
////    spi_flash_send_byte(CMD1_READ_SR_1);
////    busy_flag=spi_flash_send_byte(0xff);
////    Flash_CS_1;                     //��cs�߲�ѡ��

////    return (busy_flag);
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Read_Status_Reg2                                     */
/////* ������������ȡ SR1�ĸ���״̬                                                  */
/////******************************************************************************/
////uint8 SPI_Flash_Read_Status_Reg2(void)
////{
////    uint8 busy_flag = 0;
////    SPI_CS_LOW();                     //��cs��ѡ��
////    spi_flash_send_byte(CMD1_READ_SR_2);
////    busy_flag=spi_flash_send_byte(0xff);
////    SPI_CS_HIGH();                     //��cs�߲�ѡ��

////    return (busy_flag);
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Write_Status_Reg                                     */
/////* ����������д״̬�Ĵ���                                                  */
/////******************************************************************************/
////void SPI_Flash_Write_Status_Reg(uint16 cmd)
////{
////    SPI_CS_LOW();                     //��cs��ѡ��
////    spi_flash_send_byte(CMD1_WRITE_SR);
////    spi_flash_send_byte((uint8)cmd);
////    spi_flash_send_byte((uint8)cmd>>8);
////    SPI_CS_HIGH();                     //��cs�߲�ѡ��
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Page_Program                                     */
/////* ����������д����                                                  */
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
////        lcd_printf("Warning!!page program maybe overwrite!!\n");
////    }

////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_PAGE_PROGRAM);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    for(num=0;num<len;num++)
////    {
////        spi_flash_send_byte(*data++);
////    }
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Sector_Erase                                     */
/////* ��������������һ������4K                                                  */
/////******************************************************************************/
////void SPI_Flash_Sector_Erase(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_SECTOR_ERASE_4K);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Block_Erase32K                                     */
/////* ��������������һ����32K                                                  */
/////******************************************************************************/
////void SPI_Flash_Block_Erase32K(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_BLOCK_ERASE_32K);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    SPI_CS_HIGH();
////}
/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Block_Erase64K                                     */
/////* ��������������һ����64K                                                  */
/////******************************************************************************/
////void SPI_Flash_Block_Erase64K(uint32 address)
////{
////    uint8 addr[3]={0,};
////    addr[0] = (address & 0x00ff0000) >> 16;//bit 16~23
////    addr[1] = (address & 0x0000ff00) >> 8; //bit 8~15
////    addr[2] = (address & 0x000000ff);      //bit 0~7

////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_BLOCK_ERASE_64K);
////    spi_flash_send_byte(addr[0]);
////    spi_flash_send_byte(addr[1]);
////    spi_flash_send_byte(addr[2]);
////    SPI_CS_HIGH();
////}

/////******************************************************************************/
/////* �������ƣ�SPI_Flash_Chip_Erase                                     */
/////* ������������������оƬ                                                  */
/////******************************************************************************/
////void SPI_Flash_Chip_Erase(void)
////{
////    SPI_CS_LOW();
////    spi_flash_send_byte(CMD1_CHIP_ERASE);
////    SPI_CS_HIGH();
////}



/////*******************************************************************************
////* Function Name  : u8 W25Q64_CMD1B_READ1B(u8 cmd)
////* Description    : ����һ���ֽ�ָ����ܷ���ֵ
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
////    spi_flash_send_byte(cmd);
////    tmp = spi_flash_send_byte(0xff);
////    SPI_CS_HIGH();
////    lcd_printf("cmd:0x%x,val:%d\n",cmd,tmp);

////    return tmp;
////}

/////*******************************************************************************
////* Function Name  : void W25Q64_CMD1B_S1B(u16 cmd , u16 data)
////* Description    : дһָ����һ����
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B_S1B(uint16 cmd , uint16 data)
////{
////    SPI_CS_LOW();
////    spi_flash_send_byte(cmd);
////    spi_flash_send_byte((uint8)data);
////    spi_flash_send_byte((uint8)(data>>8));
////    SPI_CS_HIGH();
////}
/////*******************************************************************************
////* Function Name  : void W25Q64_CMD1B_S1B(u16 cmd , u16 data)
////* Description    : дһָ����������
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B_S2B(uint16 cmd , uint16 data,uint16 data2)
////{
////    SPI_CS_LOW();
////    spi_flash_send_byte(cmd);
////    spi_flash_send_byte((uint8)data);
////    spi_flash_send_byte((uint8)(data2));
////    SPI_CS_HIGH();
////}

/////*************************************]\******************************************
////* Function Name  : void W25Q64_CMD1B(u8 cmd)
////* Description    : ����һ���ֽ�ָ��
////*                  This function must be used only if the Start_Read_Sequence
////*                  function has been previously called.
////* Input          : None
////* Output         : None
////* Return         : Byte Read from the SPI Flash.
////*******************************************************************************/
////void W25Q64_CMD1B(uint8 cmd)
////{
////    SPI_CS_LOW();
////    spi_flash_send_byte(cmd);
////    SPI_CS_HIGH();
////}

/////*******************************************************************************
////* Function Name  : void W25Q64_WP_En(void)
////* Description    : дʹ��
////*                  ʹ�ܼĴ����е�WELλ
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_Write_En(void)
////{
////    uint16 tmp;
////    //��� Block Protectλ��PB0��PB1��PB2����ȡ���������򱣻�
////    //1000 0011  reserve:0111 1100 7c res:~7c
////    tmp = W25Q64_CMD1B_READ1B(RDSR) & (~0x7c);  /*& (~0x7c);*/ // ~00011100  1110 0011
////    //    lcd_printf("W25Q64_CMD1B_READ1B:%d\n",tmp);
////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp);
////    // Protected Area Upper sixteenth (Sector 15)�Լ�WELλ
////    W25Q64_CMD1B(WRITENABLE);//write enable
////    SPI_WR_HIGH();  //write Protection diable .  added by xie
////}
/////*******************************************************************************
////* Function Name  : void W25Q64_Write_CHIPEARSE_En(void)
////* Description    : дʹ��
////*                  ʹ�ܼĴ����е�WELλ
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_Write_CHIPEARSE_En(void)
////{
////    uint8 tmp,tmp2;

////    W25Q64_CMD1B(WRITENABLE);
////    //��� SEC,TBλ����� Block Protectλ��PB0��PB1��PB2����ȡ���������򱣻�
////    //1000 0011  reserve:0111 1100 7c res:~7c
////    tmp = W25Q64_CMD1B_READ1B(RDSR) & (0x83);  /*& (~0x7c);*/ // ~00011100  1110 0011
////    //1011 1111  reverse:0100 0000 40 rev:~40
////    tmp2= W25Q64_CMD1B_READ1B(RDSR2) & (0xbf);
////    //    lcd_printf("W25Q64_CMD1B_READ1B:%d\n",tmp);
//////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/

////    lcd_printf("tmp :%d\n",tmp);
////    lcd_printf("tmp2:%d\n",tmp2);
////    lcd_printf("tmp|tmp2<<8:%d\n",tmp|(tmp2<<8));
////    W25Q64_CMD1B_S2B(WRSR, tmp,tmp2);
////    // Protected Area Upper sixteenth (Sector 15)�Լ�WELλ


//////    lcd_printf("==>>read conf result!\n");
//////    W25Q64_CMD1B_READ1B(RDSR);
////    SPI_WR_HIGH();  //write Protection diable .  added by xie
////}


/////*******************************************************************************
////* Function Name  : void Flash_WP_En(void)
////* Description    : д��ֹ
////*                  ʹ�ܼĴ����е�WELλ
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_WP_En(void)
////{
////    uint16 tmp;
////    //���PB0��PB1��PB2�ı���λ
////    tmp = W25Q64_CMD1B_READ1B(RDSR) | 0x1c;
////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp);

////    W25Q64_CMD1B(WRDI);
////    SPI_WR_LOW();          //write Protection enable .  added by xie
////}
/////*******************************************************************************
////* Function Name  : void Flash_WP_En(void)
////* Description    : д��ֹ
////*                  ʹ�ܼĴ����е�WELλ
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void W25Q64_WP_CHIPEARSE_En(void)
////{
////    uint8 tmp,tmp2;
////    //���PB0��PB1��PB2�ı���λ
////    tmp = W25Q64_CMD1B_READ1B(RDSR) | 0x1c;
////    tmp2 = W25Q64_CMD1B_READ1B(RDSR2) | 0x40;

////    W25Q64_CMD1B(VSRWE);/*volatile SR wirte enable should before WRSR*/
////    W25Q64_CMD1B_S1B(WRSR, tmp&(tmp2<<8));

////    W25Q64_CMD1B(WRDI);
////    SPI_WR_LOW();          //write Protection enable .  added by xie
////}


/////********************************************************
////** �� �� ����SectorErase 4k
////** ������������������
////** ��    ����������ַ
////** ��    �أ���
////*********************************************************/
////void Sector_4K_Erase(uint32 address)
////{
////    uint8 ad[3]/* , data*/;

////    ad[0] = (address & 0x00ff0000) >> 16;
////    ad[1] = (address & 0x0000ff00) >> 8;
////    ad[2] = (address & 0x000000ff);

////    // EnterNoInt();
////    W25Q64_Write_En();
////    SPI_CS_LOW();                      //��cs��ѡ��
////    spi_flash_send_byte(0x20);
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    SPI_CS_HIGH();                      //��cs�߲�ѡ��
////    while(Check_Flash_Busy());       //�ȴ�����д�����
////    W25Q64_WP_En();
////    // ExitNoInt();
////}
/////********************************************************
////** �� �� ����SectorErase 32k
////** ������������������
////** ��    ����������ַ
////** ��    �أ���
////*********************************************************/
////void Sector_32K_Erase(uint32 address)
////{
////    // EnterNoInt();
////    W25Q64_Write_En();					//дʹ��
////    SPI_CS_LOW();                      //��cs��ѡ��
////    spi_flash_send_byte(0x52);
////    spi_flash_send_byte((uint8)(address>>16));
////    spi_flash_send_byte((uint8)(address>>8));
////    spi_flash_send_byte((uint8)address);
////    SPI_CS_HIGH();                      //��cs�߲�ѡ��
////    while(Check_Flash_Busy());       //�ȴ�����д�����
////    W25Q64_WP_En();
////    // ExitNoInt();
////}
/////********************************************************
////** �� �� ����SectorErase 64k
////** ������������������
////** ��    ����������ַ
////** ��    �أ���
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
////    SPI_CS_LOW();                      //��cs��ѡ��
////    spi_flash_send_byte(0xD8);//64k erase
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    SPI_CS_HIGH();                      //��cs�߲�ѡ��
////    while(Check_Flash_Busy());       //�ȴ�����д�����
////    //    W25Q64_WP_En();
////    W25Q64_WP_CHIPEARSE_En();
////    // ExitNoInt();
////#else
////    // EnterNoInt();
////    W25Q64_Write_En();					//дʹ��
////    SPI_CS_LOW();                      //��cs��ѡ��
////    spi_flash_send_byte(0xD8);
////    spi_flash_send_byte((uint8)(address>>16));
////    spi_flash_send_byte((uint8)(address>>8));
////    spi_flash_send_byte((uint8)address);
////    SPI_CS_HIGH();                      //��cs�߲�ѡ��
////    while(Check_Flash_Busy());       //�ȴ�����д�����
////    W25Q64_WP_En();
////    // ExitNoInt();

////#endif
////}

/////******************************************************************************
////** �� �� ����FlashProgram
////** ��������: ��ָ���ĵ�ַд��ָ���������ֽ�
////** ��    �������ݻ��������ֽ�������ʼ��ַ
////** ��    �أ���
////******************************************************************************/
////void FlashProgram(uint8 *databuffer,uint16 datasize,uint32 address)
////{
////    uint8 ad[3]/* , data*/;


////    ad[0] = (address & 0x00ff0000) >> 16;
////    ad[1] = (address & 0x0000ff00) >> 8;
////    ad[2] = (address & 0x000000ff);

////    //  EnterNoInt();
////    W25Q64_Write_En();					//дʹ��
////    SPI_CS_LOW();                      //��cs��ѡ��
////    spi_flash_send_byte(0x02);
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    while(datasize>0)
////    {
////        spi_flash_send_byte(*databuffer++);
////        datasize--;
////    }
////    SPI_CS_HIGH();                      //��cs�߲�ѡ��


////    while(Check_Flash_Busy());       //�ȴ�����д�����
////    W25Q64_WP_En();

////    //   ExitNoInt();
////}
/////*******************************************************************************
////* Function Name  : void M25P80_Write_1Byte(u32 addr , u8 data)
////* Description    : д����
////*                  д�̶���ַ��һλ����
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
////    spi_flash_send_byte(PAGE_PROG);
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    spi_flash_send_byte(data);//����ָ�����������
////    spi_flash_send_byte(data);
////    SPI_CS_HIGH();


////    while(Check_Flash_Busy());
////    W25Q64_WP_En();
////}
/////*******************************************************************************
////* Function Name  : void ChipErase(void)
////* Description    : д����
////*                  д�̶���ַ��һλ����
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void ChipErase(void)
////{
////    //    while(Check_Flash_Busy());

////    W25Q64_Write_CHIPEARSE_En();
////    SPI_CS_LOW();
////    spi_flash_send_byte(0x60);
////    SPI_CS_HIGH();

////    while(Check_Flash_Busy());
////    W25Q64_WP_CHIPEARSE_En();

////}

/////*****************************************************************
////** �� �� ����ReadRFlash
////** ������������ָ���ĵ�ַ��ȡָ���������ֽ�
////** ��    �������ݻ��������ֽ�������ʼ��ַ
////** ��    �أ���
////******************************************************************/
////void ReadFlash(uint8 *databuffer,uint16 datasize,uint32 address)
////{
////    uint16 i;
////    // EnterNoInt();
////    SPI_CS_LOW();
////    spi_flash_send_byte(0x03);
////    spi_flash_send_byte((uint8)(address>>16));
////    spi_flash_send_byte((uint8)(address>>8));
////    spi_flash_send_byte((uint8)address);
////    for(i=0;i<datasize;i++)
////    {
////        databuffer[i]=spi_flash_send_byte(0xff);
////    }
////    SPI_CS_HIGH();                 //��cs�߲�ѡ��
////    //ExitNoInt();
////}
/////*******************************************************************************
////* Function Name  : u8 W25Q64_Read_1Byte(u32 addr )
////* Description    : ������
////*                  ���̶���ַ��һλ����
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

////    SPI_CS_LOW();//ѡ��
////    spi_flash_send_byte(READ);
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    data = spi_flash_send_byte(0xff);
////    SPI_CS_HIGH();//����
////    return data;
////#else
////    uint8 ad[3] , data;



////    ad[0] = (addr & 0x00ff0000) >> 16;
////    ad[1] = (addr & 0x0000ff00) >> 8;
////    ad[2] = (addr & 0x000000ff);

////    SPI_CS_LOW();//ѡ��
////    spi_flash_send_byte(READ);
////    spi_flash_send_byte(ad[0]);
////    spi_flash_send_byte(ad[1]);
////    spi_flash_send_byte(ad[2]);
////    data = spi_flash_send_byte(0xff);
////    SPI_CS_HIGH();//����
////    //    lcd_printf("reveive data:%d\n",data);
////    return data;
////#endif
////}

/////**************************************************************************
////** ��������CheckData
////** ��������������ָ����Flash��ʼ��ַ�����ݺ�ָ���Ļ������������Ƿ�ȫ�����
////** ���������ݻ�����DataBuffer, Flash��ʼ��ַDataAddr�������ֽ���
////** ���أ�ȫ����ͬ����1�����򷵻�0
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

//////    spi_flash_send_byte(0x05);

//////    busy_flag=spi_flash_send_byte(0xff);

//////    SPI_CS_HIGH();

//////    lcd_printf("busy_flag=0x%x\n",busy_flag);

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

//////    spi_flash_send_byte(0x90);

//////    spi_flash_send_byte(0x00);

//////    spi_flash_send_byte(0x00);

//////    spi_flash_send_byte(0x00);



//////    lcd_printf("90h\n");

//////    for(i=0;i<2;i++)
//////    {
//////        val=spi_flash_send_byte(0xff);

//////        lcd_printf("val=0x%x---%d\n",val,i);

//////    }

//////    SPI_CS_HIGH();
//////}


