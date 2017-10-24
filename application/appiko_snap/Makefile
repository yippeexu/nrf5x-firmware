
BOARD           := BOARD_SNAP_REV1
SLNO            := 682073614
BLE_REQD        := BLE_STACK_SUPPORT_REQD
DEBUGGER        := JLINK
#pnarasim check log.h, use SEGGER_RTT for the jlink debugger (the jlink dbgr and the nrf togther on that dev board interfacing to our snap)
#LOGGER			:= LOG_UART_PRINTF
LOGGER			:= LOG_SEGGER_RTT
SD_USED         := blank

SDK_DIR         = ../../SDK_components
DOC_DIR         = ../../doc
PLATFORM_DIR    = ../../platform
CODEBASE_DIR    = ../../codebase

INCLUDEDIRS	= .
INCLUDEDIRS	+= $(PLATFORM_DIR)
INCLUDEDIRS += $(CODEBASE_DIR)/hal
INCLUDEDIRS += $(CODEBASE_DIR)/segger_rtt
INCLUDEDIRS += $(CODEBASE_DIR)/peripheral_modules
INCLUDEDIRS += $(CODEBASE_DIR)/util

C_SRC_DIRS = .

C_SRC_DIRS += $(CODEBASE_DIR)/hal
C_SRC_DIRS += $(CODEBASE_DIR)/peripheral_modules
C_SRC_DIRS += $(CODEBASE_DIR)/segger_rtt
C_SRC = main.c
C_SRC += hal_clocks.c ms_timer.c
C_SRC += SEGGER_RTT_printf.c SEGGER_RTT.c uart_printf.c tinyprintf.c
C_SRC += ble_adv.c profiler_timer.c us_timer.c

#Gets the name of the application folder
APPLN = $(shell basename $PWD)

CFLAGS_APP = -DDEBUG
CFLAGS_APP += -D$(BLE_REQD)
CFLAGS_APP += -D$(LOGGER)

#Lower case of BOARD
BOARD_HEADER  = $(shell echo $(BOARD) | tr A-Z a-z)
include $(PLATFORM_DIR)/Makefile.$(BOARD_HEADER)

include ../Makefile.common
