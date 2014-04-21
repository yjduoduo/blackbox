#ifndef W25Q256_H
#define W25Q256_H



#define Flash_CS_1    SPI_CS_HIGH();
#define Flash_CS_0    SPI_CS_LOW();
#define Flash_Hold_1  SPI_HOLD_HIGH();
#define Flash_Hold_0  SPI_HOLD_LOW();
#define Flash_WR_0    SPI_WR_LOW();
#define Flash_WR_1		SPI_WR_HIGH();




/* JEDEC Manufacturer’s ID */
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
//E:/lianhe/blackbox_1.1/dirvers/spi/w25q256_ssp/w25q256.c
//2014/03/26 14:58:14
/*****************************************************************************
*author:
*time  :
*****************************************************************************/
void spi_init(uint32 port);
uint8 spi_send(uint8 tdata);
uint8 spi_recv(void);
void w25qxx_wait_busy(void);
void w25xx_send_1cmd(uint8 cmd);
void w25qxx_write_enable(void);
void w25qxx_write_disable(void);
void w25qxx_SR_write_enable(void);
static void w25xx_in_qpi_mode(void);
static void w25xx_in_4byte(void);
static void w25xx_out_4byte(void);
void get_all_status_reg(void);
//十进制打印成二进制
void show_decTobin(uint8 rdata);
void w25qxx_read_id_3(uint8 *buffer);
void w25qxx_read_unique_id_3(uint8 *buffer);
int32 judge_erase_addr(uint32 byte_addr);
void w25qxx_sector_write_3(uint32 sector, uint8 * dstbuf, uint8 sector_number);
void w25qxx_sector_read_3(uint32 sector, uint8 * buffer, uint8 sector_number);
void w25qxx_sector_erase_3(uint32 sector);
void w25qxx_read_id_4(uint8 *buffer);
void w25qxx_read_unique_id_4(uint8 *buffer);
void w25qxx_sector_write_4(uint32 sector, uint8 * dst, uint8 sector_number);
void w25qxx_sector_read_4(uint32 sector, uint8 * buffer, uint8 sector_number);
void w25qxx_sector_erase_4(uint32 sector);
uint8 w25qxx_read_status_reg1(uint8 yesflag);
uint8 w25qxx_read_status_reg2(uint8 yesflag);
uint8 w25qxx_read_status_reg3(uint8 yesflag);


void w25qxx_write_1byte_4(uint32 addr, uint8 data);
uint8 w25qxx_read_1byte_4(uint32 addr);
void w25qxx_section_erase_4(uint32 addr);


void w25qxx_write_status_reg1(uint8 wdata);
void w25qxx_write_status_reg2(uint8 wdata);
void w25qxx_write_status_reg3(uint8 wdata);
void set_SR_CMP(void);
void clr_SR_CMP(void);
void set_SR_WPS(void);
void clr_SR_WPS(void);
void w25qxx_chip_erase(void);
uint16 w25qxx_get_sector_num(void);
void wx_read_jedec(void);
static void SPI_WR_HIGH(void);
static void SPI_WR_LOW(void);
static void SPI_HOLD_HIGH(void);
static void SPI_HOLD_LOW(void);
void SPI_CS_HIGH(void);
void SPI_CS_LOW(void);
void EnterNoInt(void);
void ExitNoInt(void);



//===========================================================================

#endif // W25Q256_H
