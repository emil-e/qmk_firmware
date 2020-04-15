/* Copyright 2017 Jason Williams
 * Copyright 2017 Jack Humbert
 * Copyright 2018 Yiancar
 * Copyright 2019 Clueboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"
#include "ledmatrix.h"
#include "progmem.h"
#include "config.h"
#include "eeprom.h"
#include <string.h>
#include <math.h>

led_config_t led_matrix_config;

#ifndef MAX
#    define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

#ifndef MIN
#    define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef LED_DISABLE_AFTER_TIMEOUT
#    define LED_DISABLE_AFTER_TIMEOUT 0
#endif

#ifndef LED_DISABLE_WHEN_USB_SUSPENDED
#    define LED_DISABLE_WHEN_USB_SUSPENDED false
#endif

#ifndef EECONFIG_LED_MATRIX
#    define EECONFIG_LED_MATRIX EECONFIG_RGBLIGHT
#endif

#if !defined(LED_MATRIX_MAXIMUM_BRIGHTNESS) || LED_MATRIX_MAXIMUM_BRIGHTNESS > 255
#    define LED_MATRIX_MAXIMUM_BRIGHTNESS 255
#endif

bool g_suspend_state = false;

static uint8_t g_framebuffer[LED_DRIVER_LED_COUNT];
static uint32_t g_last_time;
static uint32_t g_acc;

uint32_t eeconfig_read_led_matrix(void) { return eeprom_read_dword(EECONFIG_LED_MATRIX); }

void eeconfig_update_led_matrix(uint32_t config_value) { eeprom_update_dword(EECONFIG_LED_MATRIX, config_value); }

void eeconfig_update_led_matrix_default(void) {
    dprintf("eeconfig_update_led_matrix_default\n");
    led_matrix_config.enable = 1;
    led_matrix_config.mode   = LED_MATRIX_UNIFORM_BRIGHTNESS;
    led_matrix_config.val    = 128;
    led_matrix_config.speed  = 0;
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void eeconfig_debug_led_matrix(void) {
    dprintf("led_matrix_config eeprom\n");
    dprintf("led_matrix_config.enable = %d\n", led_matrix_config.enable);
    dprintf("led_matrix_config.mode = %d\n", led_matrix_config.mode);
    dprintf("led_matrix_config.val = %d\n", led_matrix_config.val);
    dprintf("led_matrix_config.speed = %d\n", led_matrix_config.speed);
}

void led_matrix_update_pwm_buffers(void) { led_matrix_driver.flush(); }

void led_matrix_set_index_value(int index, uint8_t value) { led_matrix_driver.set_value(index, value); }

void led_matrix_set_index_value_all(uint8_t value) { led_matrix_driver.set_value_all(value); }

bool process_led_matrix(uint16_t keycode, keyrecord_t *record) {
    uint8_t value = record->event.pressed ? 255 : 254;
    for (uint8_t i = 0; i < LED_DRIVER_LED_COUNT; i++) {
        if (g_leds[i].matrix_co.row == record->event.key.row && g_leds[i].matrix_co.col == record->event.key.col) {
            g_framebuffer[i] = value;
        }
    }

    if (record->event.pressed) {
        switch (keycode) {
            case LED_MATRIX_MODE_FORWARD:
                led_matrix_step();
                return false;

            case LED_MATRIX_MODE_REVERSE:
                led_matrix_step_reverse();
                return false;
        }
    }

    return true;
}

void led_matrix_set_suspend_state(bool state) { g_suspend_state = state; }

// All LEDs off
void led_matrix_all_off(void) { led_matrix_set_index_value_all(0); }

// Uniform brightness
void led_matrix_uniform_brightness(void) { led_matrix_set_index_value_all(LED_MATRIX_MAXIMUM_BRIGHTNESS / BACKLIGHT_LEVELS * led_matrix_config.val); }

void led_matrix_trail(void) {
    g_acc += timer_elapsed32(g_last_time);
    g_last_time   = timer_read32();
    uint8_t ticks = g_acc / 5;
    g_acc         = g_acc % 5;

    for (uint8_t i = 0; i < LED_DRIVER_LED_COUNT; i++) {
        led_matrix_set_index_value(i, g_framebuffer[i]);
        if (g_framebuffer[i] != 255) {
            g_framebuffer[i] -= MIN(ticks, g_framebuffer[i]);
        }
    }
}

void led_matrix_task(void) {
    if (!led_matrix_config.enable) {
        led_matrix_all_off();
        led_matrix_indicators();
        return;
    }

    // Ideally we would also stop sending zeros to the LED driver PWM buffers
    // while suspended and just do a software shutdown. This is a cheap hack for now.
    bool    suspend_backlight = g_suspend_state && LED_DISABLE_WHEN_USB_SUSPENDED;
    uint8_t effect            = suspend_backlight ? 0 : led_matrix_config.mode;

    // this gets ticked at 20 Hz.
    // each effect can opt to do calculations
    // and/or request PWM buffer updates.
    switch (effect) {
        case LED_MATRIX_UNIFORM_BRIGHTNESS:
            led_matrix_uniform_brightness();
            break;
        case LEF_MATRIX_TRAIL:
            led_matrix_trail();
            break;
        default:
            led_matrix_all_off();
            break;
    }

    if (!suspend_backlight) {
        led_matrix_indicators();
    }

    // Tell the LED driver to update its state
    led_matrix_driver.flush();
}

void led_matrix_indicators(void) {
    led_matrix_indicators_kb();
    led_matrix_indicators_user();
}

__attribute__((weak)) void led_matrix_indicators_kb(void) {}

__attribute__((weak)) void led_matrix_indicators_user(void) {}

void led_matrix_init(void) {
    led_matrix_driver.init();

    // Wait half a second for the driver to finish initializing
    wait_ms(500);

    g_last_time = timer_read32();

    for (uint8_t i = 0; i < LED_DRIVER_LED_COUNT; i++) {
      g_framebuffer[i] = 0;
    }

    if (!eeconfig_is_enabled()) {
        dprintf("led_matrix_init_drivers eeconfig is not enabled.\n");
        eeconfig_init();
        eeconfig_update_led_matrix_default();
    }

    led_matrix_config.raw = eeconfig_read_led_matrix();

    if (!led_matrix_config.mode) {
        dprintf("led_matrix_init_drivers led_matrix_config.mode = 0. Write default values to EEPROM.\n");
        eeconfig_update_led_matrix_default();
        led_matrix_config.raw = eeconfig_read_led_matrix();
    }

    eeconfig_debug_led_matrix();  // display current eeprom values
}

// Deals with the messy details of incrementing an integer
static uint8_t increment(uint8_t value, uint8_t step, uint8_t min, uint8_t max) {
    int16_t new_value = value;
    new_value += step;
    return MIN(MAX(new_value, min), max);
}

static uint8_t decrement(uint8_t value, uint8_t step, uint8_t min, uint8_t max) {
    int16_t new_value = value;
    new_value -= step;
    return MIN(MAX(new_value, min), max);
}

void led_matrix_toggle(void) {
    led_matrix_config.enable ^= 1;
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_enable(void) {
    led_matrix_config.enable = 1;
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_enable_noeeprom(void) { led_matrix_config.enable = 1; }

void led_matrix_disable(void) {
    led_matrix_config.enable = 0;
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_disable_noeeprom(void) { led_matrix_config.enable = 0; }

void led_matrix_step(void) {
    led_matrix_config.mode++;
    if (led_matrix_config.mode >= LED_MATRIX_EFFECT_MAX) {
        led_matrix_config.mode = 1;
    }
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_step_reverse(void) {
    led_matrix_config.mode--;
    if (led_matrix_config.mode < 1) {
        led_matrix_config.mode = LED_MATRIX_EFFECT_MAX - 1;
    }
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_increase_val(void) {
    led_matrix_config.val = increment(led_matrix_config.val, 8, 0, LED_MATRIX_MAXIMUM_BRIGHTNESS);
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_decrease_val(void) {
    led_matrix_config.val = decrement(led_matrix_config.val, 8, 0, LED_MATRIX_MAXIMUM_BRIGHTNESS);
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void led_matrix_increase_speed(void) {
    led_matrix_config.speed = increment(led_matrix_config.speed, 1, 0, 3);
    eeconfig_update_led_matrix(led_matrix_config.raw);  // EECONFIG needs to be increased to support this
}

void led_matrix_decrease_speed(void) {
    led_matrix_config.speed = decrement(led_matrix_config.speed, 1, 0, 3);
    eeconfig_update_led_matrix(led_matrix_config.raw);  // EECONFIG needs to be increased to support this
}

void led_matrix_mode(uint8_t mode, bool eeprom_write) {
    led_matrix_config.mode = mode;
    if (eeprom_write) {
        eeconfig_update_led_matrix(led_matrix_config.raw);
    }
}

uint8_t led_matrix_get_mode(void) { return led_matrix_config.mode; }

void led_matrix_set_value_noeeprom(uint8_t val) { led_matrix_config.val = val; }

void led_matrix_set_value(uint8_t val) {
    led_matrix_set_value_noeeprom(val);
    eeconfig_update_led_matrix(led_matrix_config.raw);
}

void backlight_set(uint8_t val) { led_matrix_set_value(val); }
