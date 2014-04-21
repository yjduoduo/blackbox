#ifndef USB_DRIVER_H
#define USB_DRIVER_H



/* USB Hardware Functions */
extern void  USB_Init       (void);
extern void  USB_Connect    (uint32_t  con);
extern void  USB_Reset      (void);
extern void  USB_SetAddress (uint32_t adr);
extern void WrCmdDat (uint32_t cmd, uint32_t val);
extern void USB_Connect (uint32_t con) ;
#endif // USB_DRIVER_H
