QT       += core gui
TARGET = targettest
TEMPLATE = app
HEADERS += \
./include/common/lcd_printf.h\
./include/drivers/uart0.h\
./include/drivers/uart1.h\
./include/drivers/uart_all.h\
./include/system/LPC17xx.h\
./include/system/lpc17xx_uart.h\
./include/system/system_LPC17xx.h\
./include/includeall.h\
./include/type.h
SOURCES += \
./common/printf/lcd_printf.c\
./common/system/system_LPC17xx.c\
./dirvers/serial/uart0.c\
./dirvers/serial/uart1.c\
./dirvers/serial/uart_all.c\
./src/main.c
OTHER_FILES += \
./Project/BlackBox.axf\
./Project/BlackBox.htm\
./Project/BlackBox.lnp\
./Project/BlackBox.map\
./Project/BlackBox.plg\
./Project/BlackBox.sct\
./Project/BlackBox.tra\
./Project/BlackBox.uvproj\
./Project/BlackBox_Target 1.dep\
./Project/JLinkLog.txt\
./Project/JLinkSettings.ini\
./Project/lcd_printf.__i\
./Project/lcd_printf.crf\
./Project/lcd_printf.d\
./Project/lcd_printf.o\
./Project/main.__i\
./Project/main.crf\
./Project/main.d\
./Project/main.o\
./Project/startup_LPC17xx.d\
./Project/startup_LPC17xx.lst\
./Project/startup_LPC17xx.o\
./Project/startup_LPC17xx.s\
./Project/system_lpc17xx.__i\
./Project/system_lpc17xx.crf\
./Project/system_lpc17xx.d\
./Project/system_lpc17xx.o\
./Project/uart0.__i\
./Project/uart0.crf\
./Project/uart0.d\
./Project/uart0.o\
./Project/uart1.__i\
./Project/uart1.crf\
./Project/uart1.d\
./Project/uart1.o\
./Project/uart_all.__i\
./Project/uart_all.crf\
./Project/uart_all.d\
./Project/uart_all.o\
./LPC1752_V1_0.pro\
./LPC1752_V1_0.pro.user\
./findcandcpptolist.R.exe


INCLUDEPATH +=\
./Project\
./common\
./common/printf\
./common/system\
./dirvers\
./dirvers/serial\
./include\
./include/common\
./include/drivers\
./include/system\
./src
