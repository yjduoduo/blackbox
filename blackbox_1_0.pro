QT       += core gui
TARGET = targettest
TEMPLATE = app
HEADERS += \
./include/board/lpc1752/hardware_sets.h\
./include/common/lcd_printf.h\
./include/drivers/delay.h\
./include/drivers/led.h\
./include/drivers/uart0.h\
./include/drivers/uart1.h\
./include/drivers/uart_all.h\
./include/drivers/w25q256.h\
./include/system/LPC17xx.h\
./include/system/lpc17xx_uart.h\
./include/system/system_LPC17xx.h\
./include/includeall.h\
./include/type.h
SOURCES += \
./board/lpc1752/hardware_sets.c\
./common/printf/lcd_printf.c\
./common/system/system_LPC17xx.c\
./dirvers/delay/delay.c\
./dirvers/led/led.c\
./dirvers/serial/uart0.c\
./dirvers/serial/uart1.c\
./dirvers/serial/uart_all.c\
./dirvers/spi/w25q256/w25q256.c\
./src/main.c
OTHER_FILES += \
./output/blackbox.lnp\
./output/blackbox.plg\
./output/blackbox.sct\
./output/blackbox.tra\
./output/startup_LPC17xx.d\
./output/startup_LPC17xx.o\
./prj/blackbox.map\
./prj/blackbox.plg\
./prj/blackbox.uvproj\
./prj/blackbox_Target 1.dep\
./prj/startup_LPC17xx.lst\
./prj/startup_LPC17xx.s\
./blackbox_1_0.pro\
./findcandcpptolist.R.exe


INCLUDEPATH +=\
./board\
./board/lpc1752\
./common\
./common/printf\
./common/system\
./dirvers\
./dirvers/delay\
./dirvers/led\
./dirvers/serial\
./dirvers/spi\
./dirvers/spi/w25q256\
./include\
./include/board\
./include/board/lpc1752\
./include/common\
./include/drivers\
./include/system\
./output\
./prj\
./src
