#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xC1ED
#define PRODUCT_ID      0x2390
#define DEVICE_VER      0x0001
#define MANUFACTURER    Emil Sahlén
#define PRODUCT         Kada75
#define DESCRIPTION     QMK keyboard firmware for Kada75

/* Address for jumping to bootloader on STM32 chips. */
/* It is chip dependent, the correct number can be looked up here:
 * http://www.st.com/web/en/resource/technical/document/application_note/CD00167594.pdf
 */
#define STM32_BOOTLOADER_ADDRESS 0x1FFFD800
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 16

/* ROWS: Top to bottom, COLS: Left to right
*/
#define MATRIX_ROW_PINS { A0, A1, A2, A3, A4, A5 }
#define MATRIX_COL_PINS { A7, B0, B1, B2, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15 }
#define UNUSED_PINS { A6, A15, C13, C14, C15 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define LED_DRIVER_COUNT 1
#define LED_DRIVER_LED_COUNT 85
#define LED_DRIVER_ADDR_1 0b1110100
#define I2C_DRIVER I2CD2
#define I2C1_SCL_BANK GPIOA
#define I2C1_SDA_BANK GPIOA
#define I2C1_SCL 9
#define I2C1_SDA 10

#define BACKLIGHT_LEVELS 10

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT
