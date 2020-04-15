#include "launchpad.h"

#include "quantum.h"
#include "encoder.h"

void matrix_init_kb(void) {
  // put your keyboard start-up code here
  // runs once when the firmware starts up
  matrix_init_user();
}

void matrix_scan_kb(void) {
  matrix_scan_user();
}

void keyboard_post_init_user(void) {
}

void encoder_update_kb(int8_t index, bool clockwise) {
    encoder_update_user(index, clockwise);

    uint16_t held_keycode_timer = timer_read();
    uint8_t keycode = clockwise ? KC_VOLU : KC_VOLD;
    register_code(keycode);
    while (timer_elapsed(held_keycode_timer) < 10);
    unregister_code(keycode);
}
