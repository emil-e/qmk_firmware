#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xC1ED
#define PRODUCT_ID      0x2390
#define DEVICE_VER      0x0001
#define MANUFACTURER    Emil Sahlén
#define PRODUCT         Launchpad
#define DESCRIPTION     QMK keyboard firmware for Launchpad

/* Address for jumping to bootloader on STM32 chips. */
/* It is chip dependent, the correct number can be looked up here:
 * http://www.st.com/web/en/resource/technical/document/application_note/CD00167594.pdf
 */
#define STM32_BOOTLOADER_ADDRESS 0x1FFFEC00
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 2

/* ROWS: Top to bottom, COLS: Left to right
*/
#define DIRECT_PINS { \
    { A0, NO_PIN },   \
    { A5, A3     },   \
    { A4, NO_PIN }    \
  }

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#define ENCODERS_PAD_A { A7 }
#define ENCODERS_PAD_B { A1 }
//#define ENCODER_DIRECTION_FLIP
#define ENCODER_RESOLUTION 6
