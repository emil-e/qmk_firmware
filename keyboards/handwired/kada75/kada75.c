#include "kada75.h"

#include "is31fl3731-simple.h"
#include "i2c_master.h"
#include "backlight.h"
#include "quantum.h"

const is31_led g_is31_leds[LED_DRIVER_LED_COUNT] = {
  {0, C1_1},
  {0, C1_2},
  {0, C1_3},
  {0, C1_4},
  {0, C1_5},
  {0, C1_6},
  {0, C1_7},
  {0, C1_8},
  {0, C1_9},
  {0, C1_10},
  {0, C1_11},
  {0, C1_12},
  {0, C1_13},
  {0, C1_14},
  {0, C1_15},
  {0, C1_16},

  {0, C2_1},
  {0, C2_2},
  {0, C2_3},
  {0, C2_4},
  {0, C2_5},
  {0, C2_6},
  {0, C2_7},
  {0, C2_8},
  {0, C2_9},
  {0, C2_10},
  {0, C2_11},
  {0, C2_12},
  {0, C2_13},
  {0, C2_15},
  {0, C2_16},

  {0, C3_1},
  {0, C3_2},
  {0, C3_3},
  {0, C3_4},
  {0, C3_5},
  {0, C3_6},
  {0, C3_7},
  {0, C3_8},
  {0, C3_9},
  {0, C3_10},
  {0, C3_11},
  {0, C3_12},
  {0, C3_13},
  {0, C3_16},

  {0, C4_1},
  {0, C4_2},
  {0, C4_3},
  {0, C4_4},
  {0, C4_5},
  {0, C4_6},
  {0, C4_7},
  {0, C4_8},
  {0, C4_9},
  {0, C4_10},
  {0, C4_11},
  {0, C4_12},
  {0, C4_13},
  {0, C4_15},
  {0, C4_16},

  {0, C5_1},
  {0, C5_2},
  {0, C5_3},
  {0, C5_4},
  {0, C5_5},
  {0, C5_6},
  {0, C5_7},
  {0, C5_8},
  {0, C5_9},
  {0, C5_10},
  {0, C5_11},
  {0, C5_12},
  {0, C5_13},
  {0, C5_15},
  {0, C5_16},

  {0, C6_1},
  {0, C6_2},
  {0, C6_3},
  {0, C6_7},
  {0, C6_11},
  {0, C6_12},
  {0, C6_13},
  {0, C6_14},
  {0, C6_15},
  {0, C6_16},
};

  /* k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \ */
  /* k100, k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112,       k114, k115, \ */
  /* k200, k201, k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212,             k215, \ */
  /* k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311, k312,       k314, k315, \ */
  /* k400, k401, k402, k403, k404, k405, k406, k407, k408, k409, k410, k411, k412,       k414, k415, \ */
  /* k500, k501, k502,                   k506,                   k510, k511, k512, k513, k514, k515  \ */


const led_matrix g_leds[LED_DRIVER_LED_COUNT] = {

    /*{row | col << 4}
      |            LED_ROW_COL(row, col)
      |             |            modifier
      |             |            | */
    {{0|(0<<4)},   {0, 0}, 0},           // k000
    {{0|(1<<4)},   {0, 0}, 0},           // k001
    {{0|(2<<4)},   {0, 0}, 0},           // k002
    {{0|(3<<4)},   {0, 0}, 0},           // k003
    {{0|(4<<4)},   {0, 0}, 0},           // k004
    {{0|(5<<4)},   {0, 0}, 0},           // k005
    {{0|(6<<4)},   {0, 0}, 0},           // k006
    {{0|(7<<4)},   {0, 0}, 0},           // k007
    {{0|(8<<4)},   {0, 0}, 0},           // k008
    {{0|(9<<4)},   {0, 0}, 0},           // k009
    {{0|(10<<4)},   {0, 0}, 0},          // k010
    {{0|(11<<4)},   {0, 0}, 0},          // k011
    {{0|(12<<4)},   {0, 0}, 0},          // k012
    {{0|(13<<4)},   {0, 0}, 0},          // k013
    {{0|(14<<4)},   {0, 0}, 0},          // k014
    {{0|(15<<4)},   {0, 0}, 0},          // k015

    {{1|(0<<4)},   {0, 0}, 0},           // k100
    {{1|(1<<4)},   {0, 0}, 0},           // k101
    {{1|(2<<4)},   {0, 0}, 0},           // k102
    {{1|(3<<4)},   {0, 0}, 0},           // k103
    {{1|(4<<4)},   {0, 0}, 0},           // k104
    {{1|(5<<4)},   {0, 0}, 0},           // k105
    {{1|(6<<4)},   {0, 0}, 0},           // k106
    {{1|(7<<4)},   {0, 0}, 0},           // k107
    {{1|(8<<4)},   {0, 0}, 0},           // k108
    {{1|(9<<4)},   {0, 0}, 0},           // k109
    {{1|(10<<4)},   {0, 0}, 0},          // k110
    {{1|(11<<4)},   {0, 0}, 0},          // k111
    {{1|(12<<4)},   {0, 0}, 0},          // k112
    {{1|(14<<4)},   {0, 0}, 0},          // k114
    {{1|(15<<4)},   {0, 0}, 0},          // k115

    {{2|(0<<4)},   {0, 0}, 0},           // k200
    {{2|(1<<4)},   {0, 0}, 0},           // k201
    {{2|(2<<4)},   {0, 0}, 0},           // k202
    {{2|(3<<4)},   {0, 0}, 0},           // k203
    {{2|(4<<4)},   {0, 0}, 0},           // k204
    {{2|(5<<4)},   {0, 0}, 0},           // k205
    {{2|(6<<4)},   {0, 0}, 0},           // k206
    {{2|(7<<4)},   {0, 0}, 0},           // k207
    {{2|(8<<4)},   {0, 0}, 0},           // k208
    {{2|(9<<4)},   {0, 0}, 0},           // k209
    {{2|(10<<4)},   {0, 0}, 0},          // k210
    {{2|(11<<4)},   {0, 0}, 0},          // k211
    {{2|(12<<4)},   {0, 0}, 0},          // k212
    {{2|(15<<4)},   {0, 0}, 0},          // k215

    {{3|(0<<4)},   {0, 0}, 0},           // k300
    {{3|(1<<4)},   {0, 0}, 0},           // k301
    {{3|(2<<4)},   {0, 0}, 0},           // k302
    {{3|(3<<4)},   {0, 0}, 0},           // k303
    {{3|(4<<4)},   {0, 0}, 0},           // k304
    {{3|(5<<4)},   {0, 0}, 0},           // k305
    {{3|(6<<4)},   {0, 0}, 0},           // k306
    {{3|(7<<4)},   {0, 0}, 0},           // k307
    {{3|(8<<4)},   {0, 0}, 0},           // k308
    {{3|(9<<4)},   {0, 0}, 0},           // k309
    {{3|(10<<4)},   {0, 0}, 0},          // k310
    {{3|(11<<4)},   {0, 0}, 0},          // k311
    {{3|(12<<4)},   {0, 0}, 0},          // k312
    {{3|(14<<4)},   {0, 0}, 0},          // k314
    {{3|(15<<4)},   {0, 0}, 0},          // k315

    {{4|(0<<4)},   {0, 0}, 0},           // k400
    {{4|(1<<4)},   {0, 0}, 0},           // k401
    {{4|(2<<4)},   {0, 0}, 0},           // k402
    {{4|(3<<4)},   {0, 0}, 0},           // k403
    {{4|(4<<4)},   {0, 0}, 0},           // k404
    {{4|(5<<4)},   {0, 0}, 0},           // k405
    {{4|(6<<4)},   {0, 0}, 0},           // k406
    {{4|(7<<4)},   {0, 0}, 0},           // k407
    {{4|(8<<4)},   {0, 0}, 0},           // k408
    {{4|(9<<4)},   {0, 0}, 0},           // k409
    {{4|(10<<4)},   {0, 0}, 0},          // k410
    {{4|(11<<4)},   {0, 0}, 0},          // k411
    {{4|(12<<4)},   {0, 0}, 0},          // k412
    {{4|(14<<4)},   {0, 0}, 0},          // k414
    {{4|(15<<4)},   {0, 0}, 0},          // k415

    {{5|(0<<4)},   {0, 0}, 0},           // k500
    {{5|(1<<4)},   {0, 0}, 0},           // k501
    {{5|(2<<4)},   {0, 0}, 0},           // k502
    {{5|(6<<4)},   {0, 0}, 0},           // k506
    {{5|(10<<4)},   {0, 0}, 0},          // k510
    {{5|(11<<4)},   {0, 0}, 0},          // k511
    {{5|(12<<4)},   {0, 0}, 0},          // k512
    {{5|(13<<4)},   {0, 0}, 0},          // k513
    {{5|(14<<4)},   {0, 0}, 0},          // k514
    {{5|(15<<4)},   {0, 0}, 0},          // k515
};

void matrix_init_kb(void) {
  // put your keyboard start-up code here
  // runs once when the firmware starts up
  matrix_init_user();
  led_init_ports();
}

void matrix_scan_kb(void) {
  matrix_scan_user();
}

void keyboard_post_init_user(void) {
  backlight_set(BACKLIGHT_LEVELS);
}
