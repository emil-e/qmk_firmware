## chip/board settings
MCU_FAMILY = STM32
MCU_SERIES = STM32F0xx
MCU_LDSCRIPT = STM32F070x6
MCU_STARTUP = stm32f0xx
BOARD = GENERIC_STM32_F070X6
MCU  = cortex-m0
ARMV = 6

USE_FPU = no
OPT_DEFS =

# Options to pass to dfu-util when flashing
DFU_ARGS = -d 0483:df11 -a 0 -s 0x08000000:leave
DFU_SUFFIX_ARGS = -p DF11 -v 0483

EXTRAKEY_ENABLE = yes   # Audio control and System control(+450)
NKRO_ENABLE = yes       # USB Nkey Rollover
EEPROM_DRIVER = transient
ENCODER_ENABLE = yes
