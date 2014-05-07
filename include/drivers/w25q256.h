#ifndef W25Q256_H
#define W25Q256_H



/* JEDEC Manufacturer¡¯s ID */
#define MF_ID           (0xEF)
/* JEDEC Device ID: Memory type and Capacity */
#define MTC_W25Q80_BV         (0x4014) /* W25Q80DW */
#define MTC_W25Q16_BV_CL_CV   (0x4015) /* W25Q16BV W25Q16CL W25Q16CV  */
#define MTC_W25Q16_DW         (0x6015) /* W25Q16DW  */
#define MTC_W25Q32_BV         (0x4016) /* W25Q32BV */
#define MTC_W25Q32_DW         (0x6016) /* W25Q32DW */
#define MTC_W25Q64_BV_CV      (0x4017) /* W25Q64BV W25Q64CV */
#define MTC_W25Q64_DW         (0x4017) /* W25Q64DW */
#define MTC_W25Q128_BV        (0x4018) /* W25Q128BV */
#define MTC_W25Q256_FV        (0x4019) /* W25Q256FV*/


/* SSPxSR - bit definitions. */
#define TFE     0x01
#define TNF     0x02
#define RNE     0x04
#define RFF     0x08
#define BSY     0x10





//===========================================================================
/*
WR:Write
RD:read
ERS:erase
SR:status register
REG:register
VSR:volatile status register
EXT:extended
PRO:program
SUSP:suspend
RES:resume
PAR:parameter
MANU:manufactor
DEV:device
RELE:release
POWERDN:power down
GLO:global
BLK:block
ENT:enter
ADDR:address
MOD:mode
INDV:individual
OCT:octal
*/
/*   instruction set table 4(QPI instructions,3-byte&4-byte address mode)  */
#define QPI_WR_EN           (0x06)  /*  write enable */
#define QPI_VSR_WR_EN       (0x50)  /*  volatile SR write enable */
#define QPI_W_DIS           (0x04)  /*  write disable */
#define QPI_RD_SR_1         (0x05)  /*  read status register-1 */
#define QPI_WR_SR_1         (0x01)  /*  write status register-1 */
#define QPI_RD_SR_2         (0x35)  /*  read status register-2 */
#define QPI_WR_SR_2         (0x31)  /*  write status register-2 */
#define QPI_RD_SR_3         (0x15)  /*  read status register-3 */
#define QPI_WR_SR_3         (0x11)  /*  write status register-3 */
#define QPI_RD_EXT_ADR_REG  (0xC8)  /*  read extended addr.register */
#define QPI_WR_EXT_ADR_REG  (0xC5)  /*  write extended add.register */
#define QPI_CHIP_ERS        (0xC7)  /*  chip erase */
#define QPI_ERS_PRO_SUSP    (0x75)  /*  erase/program suspend */
#define QPI_ERS_PRO_RES     (0x7A)  /*  erase/program resume */
#define QPI_POWERDOWN       (0xB9)  /*  power-down */
#define QPI_RELE_POWERDNID  (0xAB)  /*  release powerdown ID */
#define QPI_MANU_DEV_ID     (0x90)  /*  manufacturer/device id */
#define QPI_JEDEC_ID        (0x9F)  /*  JEDEC ID */
#define QPI_GLO_BLK_LOCK    (0x7E)  /*  global block lock */
#define QPI_GLO_BLK_UNLOCK  (0x98)  /*  global block unlock */
#define QPI_EXIT_QPI_MODE   (0xFF)  /*  exit QPI mode */
#define QPI_ENT_ADDRMOD_4B  (0xB7)  /*  enter 4-byte address mode */
#define QPI_EXIT_ADDRMOD_4B (0xE9)  /*  exit 4-byte address mode */
#define QPI_ENABLE_RESET    (0x66)  /*  enable reset */
#define QPI_RESET_DEV       (0x99)  /*  reset device */
/*   instruction set table 6(QPI instructions,4-byte address mode)  */
#define QPI_PAGE_PROGRAM    (0x02)  /* page program  */
#define QPI_SECTOR_ERS      (0x20)  /* sector erase(4KB)  */
#define QPI_BLK_ERS_32K     (0x52)  /* block erase(32KB)  */
#define QPI_BLK_ERS_64K     (0xD8)  /* block erase(64KB)  */
#define QPI_FAST_RD         (0x0B)  /* fast read  */
#define QPI_BURST_RD_WRAP   (0x0C)  /* burst read with wrap  */
#define QPI_FAST_RD_QUAD_IO (0xEB)  /* fast read quad I/O  */
#define QPI_INDV_BLK_LOCK   (0x36)  /* individual block lock  */
#define QPI_INDV_BLK_UNLOCK (0x39)  /* individual block unlock  */
#define QPI_RD_BLK_LOCK     (0x3D)  /* read block lock  */


//SPI mode
/*   instruction set table 1(SPI instructions,3-byte&4-byte address mode)  */
#define SPI_WR_EN                   (0x06)  /*  write enable */
#define SPI_VSR_WR_EN               (0x50)  /*  volatile SR write enable */
#define SPI_WR_DIS                  (0x04)  /*  write disable */
#define SPI_RD_SR_1                 (0x05)  /*  read status register-1 */
#define SPI_WR_SR_1                 (0x01)  /*  write status register-1 */
#define SPI_RD_SR_2                 (0x35)  /*  read status register-2 */
#define SPI_WR_SR_2                 (0x31)  /*  write status register-2 */
#define SPI_RD_SR_3                 (0x15)  /*  read status register-3 */
#define SPI_WR_SR_3                 (0x11)  /*  write status register-3 */
#define SPI_RD_EXT_ADR_REG          (0xC8)  /*  read extended addr.register */
#define SPI_WR_EXT_ADR_REG          (0xC5)  /*  write extended add.register */
#define SPI_CHIP_ERS                (0xC7)  /*  chip erase */
#define SPI_ERS_PRO_SUSP            (0x75)  /*  erase/program suspend */
#define SPI_ERS_PRO_RES             (0x7A)  /*  erase/program resume */
#define SPI_POWERDOWN               (0xB9)  /*  power-down */
#define SPI_SET_RD_PAR              (0xC0)  /*  set read parameters */
#define SPI_RELE_POWERDNID          (0xAB)  /*  release powerdown ID */
#define SPI_MANU_DEV_ID             (0x90)  /*  manufacturer/device id */
#define SPI_JEDEC_ID                (0x9F)  /*  JEDEC ID */
#define SPI_GLO_BLK_LOCK            (0x7E)  /*  global block lock */
#define SPI_GLO_BLK_UNLOCK          (0x98)  /*  global block unlock */
#define SPI_ENTER_QPI_MOD           (0x38)  /* enter QPI mode  */
#define SPI_ENT_ADDRMOD_4B          (0xB7)  /*  enter 4-byte address mode */
#define SPI_EXIT_ADDRMOD_4B         (0xE9)  /*  exit 4-byte address mode */
#define SPI_ENABLE_RESET            (0x66)  /*  enable reset */
#define SPI_RESET_DEV               (0x99)  /*  reset device */
#define SPI_RD_DAT_4BYTE            (0x13)  /*  read data with 4-byte address */
#define SPI_FAST_RD_4BYTE           (0x0C)  /*  fast read with 4-byte address */
#define SPI_FAST_RD_DUALOUT_4BYTE   (0x3C)  /*  fast read dual output */
#define SPI_FAST_RD_QUADOUT_4BYTE   (0x6C)  /*  fast read quad output */
#define SPI_FAST_RD_DUALIO_4BYTE    (0xBC)  /*  fast read dual I/O */
#define SPI_FAST_RD_QUADIO_4BYTE    (0xEC)  /*  fast read quad I/O */
/*   instruction set table 2(SPI instructions,3-byte address mode)  */
#define SPI_3B_RD_UNIQUE_ID         (0x4B)
#define SPI_3B_PAGE_PROG            (0x02)
#define SPI_3B_QUAD_PAGE_PROG       (0x32)
#define SPI_3B_SECTOR_ERS_4K        (0x20)
#define SPI_3B_BLK_ERS_32K          (0x52)
#define SPI_3B_BLK_ERS_64K          (0xD8)
#define SPI_3B_RD_DAT               (0x03)
#define SPI_3B_FAST_RD              (0x0B)
#define SPI_3B_FAST_RD_DUALOUT      (0x3B)
#define SPI_3B_FAST_RD_QUADOUT      (0x6B)
#define SPI_3B_RD_SFDP_REG          (0x5A)
#define SPI_3B_ERS_SECURITY_REG     (0x44)
#define SPI_3B_PROG_SECURITY_REG    (0x42)
#define SPI_3B_RD_SECURITY_REG      (0x48)
#define SPI_3B_INDV_BLK_LOCK        (0x36)
#define SPI_3B_INDV_BLK_UNLOCK      (0x39)
#define SPI_3B_RD_BLK_LOCK          (0X3D)
#define SPI_3B_FAST_RD_DUAL_IO      (0XBB)
#define SPI_3B_MFTR_DEV_ID_DUALIO   (0X92)
#define SPI_3B_SET_BURST_WRAP       (0X77)
#define SPI_3B_FAST_RD_QUAL_IO      (0XEB)
#define SPI_3B_WORD_RD_QUAD_IO      (0XE7)
#define SPI_3B_OCT_WORD_RD_QUAD_IO  (0XE3)
#define SPI_3B_MFTR_DEV_ID_QUAD_IO  (0X94)
/*   instruction set table 3(SPI instructions,4-byte address mode)  */
#define SPI_4B_RD_UNIQUE_ID         (0x4B)
#define SPI_4B_PAGE_PROG            (0x02)
#define SPI_4B_QUAD_PAGE_PROG       (0x32)
#define SPI_4B_SECTOR_ERS_4K        (0x20)
#define SPI_4B_BLK_ERS_32K          (0x52)
#define SPI_4B_BLK_ERS_64K          (0xD8)
#define SPI_4B_RD_DAT               (0x03)
#define SPI_4B_FAST_RD              (0x0B)
#define SPI_4B_FAST_RD_DUALOUT      (0x3B)
#define SPI_4B_FAST_RD_QUADOUT      (0x6B)
#define SPI_4B_RD_SFDP_REG          (0x5A)
#define SPI_4B_ERS_SECURITY_REG     (0x44)
#define SPI_4B_PROG_SECURITY_REG    (0x42)
#define SPI_4B_RD_SECURITY_REG      (0x48)
#define SPI_4B_INDV_BLK_LOCK        (0x36)
#define SPI_4B_INDV_BLK_UNLOCK      (0x39)
#define SPI_4B_RD_BLK_LOCK          (0X3D)
#define SPI_4B_FAST_RD_DUAL_IO      (0XBB)
#define SPI_4B_MFTR_DEV_ID_DUALIO   (0X92)
#define SPI_4B_SET_BURST_WRAP       (0X77)
#define SPI_4B_FAST_RD_QUAL_IO      (0XEB)
#define SPI_4B_WORD_RD_QUAD_IO      (0XE7)
#define SPI_4B_OCT_WORD_RD_QUAD_IO  (0XE3)
#define SPI_4B_MFTR_DEV_ID_QUAD_IO  (0X94)

//===========================================================================
//===========================================================================


/* local public functions*/
//===========================================================================
static void SPI_WR_HIGH(void);
static void SPI_WR_LOW(void);
static void SPI_HOLD_HIGH(void);
static void SPI_HOLD_LOW(void);
void SPI_CS_HIGH(void);
void SPI_CS_LOW(void);
void EnterNoInt(void);
void ExitNoInt(void);
void SPI0_init(void);
uint8 spi_flash_send_byte(uint8 tdata);
// uint8 spi_flash_receive_byte(void);
uint8_t w25qxx_read_status_reg1(void);
uint8_t w25qxx_read_status_reg2(void);
uint8_t w25qxx_read_status_reg3(void);
void w25qxx_read_id(uint8_t *buffer);
void w25qxx_wait_busy(void);
void w25qxx_write_enable(void);
void w25qxx_write_disable(void);
void w25qxx_sector_write(uint32_t sector, uint8_t * buffer, uint8_t sector_number);
void w25qxx_sector_read(uint32_t sector, uint8_t * buffer, uint8_t sector_number);
void w25qxx_sector_erase(uint32_t sector);
uint16_t w25qxx_get_sector_num(void);


static uint8 Check_Flash_Busy(void);
static void SPI_Flash_Send_1Cmd(uint8 cmd);
static void ENTER_QPI_MOD(void);

//public func lists
//===========================================================================

uint16 SPI_Flash_Read_MANU_DEVICE_ID(void);

static void SPI_Flash_Enter_4byte_mode(void);
static void SPI_Flash_Exit_4byte_mode(void);
static void SPI_Flash_Write_Enable(void);
static void SPI_Flash_Write_Disable(void);
static void SPI_Flash_VSR_Write_Enable(void);
static void SPI_Flash_Chip_Erase(void);
void SPI_Flash_Power_Down(void);
uint32 SPI_Flash_Release_PowerDown_DevID(void);
uint16 SPI_Flash_MANU_DEVICE_ID(void);
uint32 SPI_Flash_JEDEC_ID(void);

void w25qxx_read_id(uint8_t *buffer);

void wx_read_jedec(void);


//===========================================================================




//static void SPI_WR_HIGH(void);
//static void SPI_WR_LOW(void);
//static void SPI_HOLD_HIGH(void);
//static void SPI_HOLD_LOW(void);
//static void SPI_CS_HIGH(void);
//static void SPI_CS_LOW(void);
//void ExitNoInt(void);
//void EnterNoInt(void);
//static void SPI_Flash_Send_1Cmd(uint8 cmd);
//static void SPI_Flash_Write_Enable(void);
//static void SPI_Flash_Write_Disable(void);
//static void SPI_Flash_Vo_SR_WriteEnable(void);
//uint8 SPI_Flash_Read_Status_Reg1(void);
//uint8 SPI_Flash_Read_Status_Reg2(void);
//void SPI_Flash_Write_Status_Reg(uint16 cmd);
//void SPI_Flash_Page_Program(uint32 address,
//                            uint8* data,
//                            uint16 len);
//void SPI_Flash_Sector_Erase(uint32 address);
//void SPI_Flash_Block_Erase32K(uint32 address);
//void SPI_Flash_Block_Erase64K(uint32 address);
//void SPI_Flash_Chip_Erase(void);

////#define EnterNoInt() _EnterNoInt()

////#define ExitNoInt() _ExitNoInt()


//extern void SPI0_init(void);
//extern uint8 SPISendByte(uint8 cdata);
//extern void WriteEnable(void);
//extern void WriteDisable(void);
//extern void ReadUniqueID(uint8 *databuffer);
////extern void ReadDeviceID(uint8 *databuffer);
//extern void Sector_4K_Erase(uint32);
//extern void Sector_32K_Erase(uint32 address);
//extern void Sector_64K_Erase(uint32 address);
//extern void FlashProgram(uint8 *databuffer,uint16 datasize,uint32 address) ;
//extern void M25P80_Write_1Byte(uint32 addr , uint8 data);
//extern void ReadFlash(uint8 *databuffer,uint16 datasize,uint32 address);

//extern uint8 CheckData(uint8 *DataBuffer, uint32 DataAddr, uint16 DataSize);

//extern uint16 SPI_Flash_Read_MANU_DEVICE_ID(void);
//extern uint8  SPI_Flash_ReadUniqueID(void);
//extern uint32 SPI_Flash_Read_JEDECID(void);
//extern uint8  SPI_Flash_Read_ReleasePowerDownID(void);
//extern uint8  SPI_Flash_Read_Data(uint32 address);
//extern uint8  SPI_Flash_Fast_Read(uint32 address);

////
//extern uint8 ReadFlash_1Byte(uint32 addr);


////////////////////new things//////////////
//extern void ReadManufactDeviceID_90h_W25Q(void);

//extern void ChipErase(void);
//===========================================================================

#endif // W25Q256_H
