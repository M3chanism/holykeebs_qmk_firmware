/*
This is the c configuration file for the keymap

Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#endif

#define TAP_CODE_DELAY 5

// #define POINTING_DEVICE_AUTO_MOUSE_ENABLE
// #define AUTO_MOUSE_DEFAULT_LAYER 2

#define KEYBALL_SCROLLSNAP_ENABLE     1 // enable snap-to-axis (prevents diagonal jitter)
#define KEYBALL_SCROLLSNAP_MODE       0  // 0 = vertical priority, 1 = horizontal priority
#define KEYBALL_SCROLLSNAP_INTERVAL   100  // ms between snap checks; lower = more responsive but potentially jitterier

// #define HK_MAIN_DEFAULT_POINTER_SCROLL_BUFFER_SIZE 0
// #define HK_PERIPHERAL_DEFAULT_POINTER_SCROLL_BUFFER_SIZE 0
// #define KEYBALL_SCROLL_DIV_DEFAULT 4

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

// for homerow mods
// see: https://docs.qmk.fm/tap_hold
#define CHORDAL_HOLD // only resolve as hold if next key is on opposite hand
#define PERMISSIVE_HOLD // overriden by hold on other key press
#define TAPPING_TERM 200 // register hold after X ms delay
#define TAPPING_TERM_PER_KEY
#define FLOW_TAP_TERM 100 // disables holds when key is pressed X ms after previous key
#define QUICK_TAP_TERM 120  // defaults to TAPPING_TERM, delay for double tap to repeat
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY // only apply to layer tap thumb keys
