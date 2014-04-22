QT       += core gui
TARGET = targettest
TEMPLATE = app
HEADERS += \
./ZY_CODE/Startup/lpc1700.h\
./ZY_CODE/Startup/target.h\
./ZY_CODE/zy_if/zy_if.h\
./ZY_CODE/firmware.h\
./cfg_file/Startup/LPC1700PinCfg.h\
./cfg_file/Startup/vector_cfg.h\
./cfg_file/zy_if/zy_if_cfg.h\
./cfg_file/target_cfg.h\
./usbDevLPC1700/usbDevCI.h\
./usbDevLPC1700/usbDevConfig.h\
./usbDevLPC1700/usbDevDMA.h\
./usbDevLPC1700/usbDevDescriptor.h\
./usbDevLPC1700/usbDevDriver.h\
./usbDevLPC1700/usbDevHAL.h\
./usbDevLPC1700/usbDevProtocol.h\
./usbDevLPC1700/usbReg.h\
./config.h
SOURCES += \
./USER_CODE/main.c\
./ZY_CODE/Startup/LPC1700PinCfg.c\
./ZY_CODE/Startup/target.c\
./ZY_CODE/Startup/vector_table.c\
./ZY_CODE/zy_if/zy_if.c\
./cfg_file/Startup/rt_sys_c.c\
./usbDevLPC1700/usbDevCI.c\
./usbDevLPC1700/usbDevDMA.c\
./usbDevLPC1700/usbDevDescriptor.c\
./usbDevLPC1700/usbDevDriver.c\
./usbDevLPC1700/usbDevHAL.c\
./usbDevLPC1700/usbDevProtocol.c
OTHER_FILES += \
./cfg_file/Startup/InChip.sct\
./cfg_file/Startup/rt_sys_s.s\
./out/Cortex-M3 Execute for lpc1700.axf\
./out/Cortex-M3 Execute for lpc1700.htm\
./out/Cortex-M3 Execute for lpc1700.lnp\
./out/Cortex-M3 Execute for lpc1700.plg\
./out/Cortex-M3 Execute for lpc1700.tra\
./out/isp1301.crf\
./out/isp1301.d\
./out/isp1301.o\
./out/lpc1700pincfg.crf\
./out/lpc1700pincfg.d\
./out/lpc1700pincfg.o\
./out/main.crf\
./out/main.d\
./out/main.o\
./out/rt_sys_c.crf\
./out/rt_sys_c.d\
./out/rt_sys_c.o\
./out/rt_sys_s.d\
./out/rt_sys_s.o\
./out/target.crf\
./out/target.d\
./out/target.o\
./out/usbdevci.crf\
./out/usbdevci.d\
./out/usbdevci.o\
./out/usbdevdescriptor.crf\
./out/usbdevdescriptor.d\
./out/usbdevdescriptor.o\
./out/usbdevdma.crf\
./out/usbdevdma.d\
./out/usbdevdma.o\
./out/usbdevdriver.crf\
./out/usbdevdriver.d\
./out/usbdevdriver.o\
./out/usbdevhal.crf\
./out/usbdevhal.d\
./out/usbdevhal.o\
./out/usbdevprotocol.crf\
./out/usbdevprotocol.d\
./out/usbdevprotocol.o\
./out/vector_table.crf\
./out/vector_table.d\
./out/vector_table.o\
./out/zy_if.crf\
./out/zy_if.d\
./out/zy_if.o\
./Cortex-M3 Execute for lpc1700.Opt\
./Cortex-M3 Execute for lpc1700.Uv2\
./Cortex-M3 Execute for lpc1700.map\
./Cortex-M3 Execute for lpc1700_DebugInFlash.dep\
./Cortex-M3 Execute for lpc1700_Uv2.Bak\
./LPC17xx”√ªß ÷≤·.pdf\
./findcandcpptolist.R.exe\
./usbDevSimpleDemo.pro


INCLUDEPATH +=\
./USER_CODE\
./ZY_CODE\
./ZY_CODE/Startup\
./ZY_CODE/zy_if\
./cfg_file\
./cfg_file/Startup\
./cfg_file/zy_if\
./out\
./usbDevLPC1700
