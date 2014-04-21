#include "includeall.h"
#include "usb_driver.h"
#include "usbcfg.h"
#include "usbreg.h"


void USB_Init (void)
{
    PCONP |= (1UL<<31);

    //usb_D+ P0.29
    PINSEL1 &= ~(0x3<<26);//clear P0.29
    PINSEL1 |= (0x1<<26);//P0.29,Func[1] /* P0.29 D+, P0.30 D- */
    //usb_d- P0.30
    PINSEL1 &= ~(0x3<<28);//clear P0.30
    PINSEL1 |= (0x1<<28);//P0.30,Func[1] /* PINSEL1 26.27, 28.29  = 01 */


    //  LPC_PINCON->PINSEL1 &= ~((3<<26)|(3<<28));   /* P0.29 D+, P0.30 D- */
    //  LPC_PINCON->PINSEL1 |=  ((1<<26)|(1<<28));   /* PINSEL1 26.27, 28.29  = 01 */

    //p1.18 usb_up_led,
    //P1.30 vbus
    //  LPC_PINCON->PINSEL3 &= ~((3<< 4)|(3<<28));   /* P1.18 GoodLink, P1.30 VBUS */
    //  LPC_PINCON->PINSEL3 |=  ((1<< 4)|(2<<28));   /* PINSEL3 4.5 = 01, 28.29 = 10 */

    //P2.9 softconnect
    //  LPC_PINCON->PINSEL4 &= ~((3<<18)        );   /* P2.9 SoftConnect */
    //  LPC_PINCON->PINSEL4 |=  ((1<<18)        );   /* PINSEL4 18.19 = 01 */

    //    LPC_SC->PCONP |= (1UL<<31);                /* USB PCLK -> enable USB Per.       */


    USBClkCtrl = 0x1A;
//    while((USBClkSt & 0x1A) != 0x1A);

    //    LPC_USB->USBClkCtrl = 0x1A;                /* Dev, PortSel, AHB clock enable */
    //    while ((LPC_USB->USBClkSt & 0x1A) != 0x1A);

    //    NVIC_EnableIRQ(USB_IRQn);               /* enable USB interrupt */

    //    USB_Reset();
    //    USB_SetAddress(0);

    Debug("usb init done\n");
}



/*
 *  USB Reset Function
 *   Called automatically on USB Reset
 *    Return Value:    None
 */

void USB_Reset (void) {
#if USB_DMA
    uint32_t n;
#endif

    LPC_USB->USBEpInd = 0;
    LPC_USB->USBMaxPSize = USB_MAX_PACKET0;
    LPC_USB->USBEpInd = 1;
    LPC_USB->USBMaxPSize = USB_MAX_PACKET0;
    while ((LPC_USB->USBDevIntSt & EP_RLZED_INT) == 0);

    LPC_USB->USBEpIntClr  = 0xFFFFFFFF;
    LPC_USB->USBEpIntEn   = 0xFFFFFFFF ^ USB_DMA_EP;
    LPC_USB->USBDevIntClr = 0xFFFFFFFF;
    LPC_USB->USBDevIntEn  = DEV_STAT_INT    | EP_SLOW_INT    |
            (USB_SOF_EVENT   ? FRAME_INT : 0) |
            (USB_ERROR_EVENT ? ERR_INT   : 0);

#if USB_DMA
    LPC_USB->USBUDCAH   = USB_RAM_ADR;
    LPC_USB->USBDMARClr = 0xFFFFFFFF;
    LPC_USB->USBEpDMADis  = 0xFFFFFFFF;
    LPC_USB->USBEpDMAEn   = USB_DMA_EP;
    LPC_USB->USBEoTIntClr = 0xFFFFFFFF;
    LPC_USB->USBNDDRIntClr = 0xFFFFFFFF;
    LPC_USB->USBSysErrIntClr = 0xFFFFFFFF;
    LPC_USB->USBDMAIntEn  = 0x00000007;
    DDMemMap[0] = 0x00000000;
    DDMemMap[1] = 0x00000000;
    for (n = 0; n < USB_EP_NUM; n++) {
        udca[n] = 0;
        UDCA[n] = 0;
    }
#endif
}

/*
 *  USB Set Address Function
 *    Parameters:      adr:   USB Address
 *    Return Value:    None
 */

void USB_SetAddress (uint32_t adr) {
    WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr)); /* Don't wait for next */
    WrCmdDat(CMD_SET_ADDR, DAT_WR_BYTE(DEV_EN | adr)); /*  Setup Status Phase */
}


/*
 *  Write Command Data
 *    Parameters:      cmd:   Command
 *                     val:   Data
 *    Return Value:    None
 */

void WrCmdDat (uint32_t cmd, uint32_t val) {

    LPC_USB->USBDevIntClr = CCEMTY_INT;
    LPC_USB->USBCmdCode = cmd;
    while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
    LPC_USB->USBDevIntClr = CCEMTY_INT;
    LPC_USB->USBCmdCode = val;
    while ((LPC_USB->USBDevIntSt & CCEMTY_INT) == 0);
}
/*
 *  USB Connect Function
 *   Called by the User to Connect/Disconnect USB
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USB_Connect (uint32_t con) {
    WrCmdDat(CMD_SET_DEV_STAT, DAT_WR_BYTE(con ? DEV_CON : 0));
}
