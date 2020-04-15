## chip/board settings
MCU_FAMILY = STM32
MCU_SERIES = STM32F3xx
MCU_LDSCRIPT = STM32F302x8
MCU_STARTUP = stm32f3xx
BOARD = GENERIC_STM32_F302X8
MCU  = cortex-m4
ARMV = 7
USE_FPU = yes
OPT_DEFS =

# Options to pass to dfu-util when flashing
DFU_ARGS = -d 0483:df11 -a 0 -s 0x08000000:leave
DFU_SUFFIX_ARGS = -p DF11 -v 0483

BACKLIGHT_ENABLE = yes
BOOTMAGIC_ENABLE = full # Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE = yes   # Mouse keys(+4700)
EXTRAKEY_ENABLE = yes   # Audio control and System control(+450)
CONSOLE_ENABLE = yes    # Console for debug(+400)
COMMAND_ENABLE = no     # Commands for debug and configuration
NKRO_ENABLE = yes       # USB Nkey Rollover
AUDIO_ENABLE = no
RGBLIGHT_ENABLE = no    # Enable keyboard underlight functionality
MIDI_ENABLE = no        # MIDI controls
UNICODE_ENABLE = no     # Unicode
BLUETOOTH_ENABLE = no   # Enable Bluetooth with the Adafruit EZ-Key HID
LED_MATRIX_ENABLE = IS31FL3731
