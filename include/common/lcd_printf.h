#ifndef LCD_PRINTF_H
#define LCD_PRINTF_H


#ifdef EXTERN_LCD_PRINTF
#define EXT_LCD_PRINTF
#else
#define EXT_LCD_PRINTF extern
#endif



EXT_LCD_PRINTF void puts__(char *s);
EXT_LCD_PRINTF void lcd_printf(char *str,...);



#endif // LCD_PRINTF_H
