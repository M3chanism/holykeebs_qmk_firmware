/*
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

#include QMK_KEYBOARD_H

#include "quantum.h"

void keyball_on_apply_motion_to_mouse_scroll(report_mouse_t *r, report_mouse_t *o, bool is_left) {
    static int16_t acc_x = 0;
    static int16_t acc_y = 0;
    acc_x += r->x;
    acc_y += r->y;
    r->x = 0;
    r->y = 0;

    const int16_t scroll_speed_div = 1 << (keyball_get_scroll_div() - 1);

    o->h = -(acc_x / scroll_speed_div);
    acc_x += o->h * scroll_speed_div;

    o->v = acc_y / scroll_speed_div;
    acc_y -= o->v * scroll_speed_div;

    if (is_left) {
        o->h = -o->h;
        o->v = -o->v;
    }
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q, KC_W, KC_E, KC_R, KC_T,         KC_Y, KC_U, KC_I, KC_O, KC_P,
    MT(MOD_LALT,KC_A), MT(MOD_LCTL,KC_S), MT(MOD_LGUI,KC_D), MT(MOD_LSFT,KC_F), KC_G,    KC_H, MT(MOD_RSFT,KC_J), MT(MOD_RGUI,KC_K), MT(MOD_RCTL,KC_L), MT(MOD_RALT,KC_QUOT),
    KC_Z, KC_X, LT(2,KC_C), KC_V, KC_B,            KC_N, KC_M, LT(2,KC_COMM), KC_DOT, KC_SLSH,
    KC_NO,KC_NO,KC_NO , LT(3,KC_ESC),LT(1,KC_SPC),LT(6,KC_TAB),      LT(5,KC_ENT),LT(4,KC_BSPC),  KC_NO,KC_NO,KC_NO,     KC_NO
  ),

  [1] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO  ,                           KC_NO, KC_CAPS, KC_INS, KC_NO, KC_NO,
    MOD_LALT  , MOD_LCTL , MOD_LGUI , MOD_LSFT , KC_NO ,                           KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_DEL,
    KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,                                                 KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_NO,
    KC_NO,KC_NO,KC_NO         , _______  , KC_TRNS , _______  ,       KC_ENT, KC_BSPC,        KC_NO,KC_NO,KC_NO,     KC_NO
  ),

  [2] = LAYOUT_universal(
    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                             KC_NO, SCRL_DVI, SCRL_DVD, KC_NO, KC_NO,
    MOD_LALT, MOD_LCTL, MOD_LGUI , MOD_LSFT , KC_NO ,                              KC_NO, KC_RSFT, KC_RGUI, KC_RCTL, KC_RALT,
    KC_NO, KC_NO, KC_TRNS, SCRL_MO, KC_NO,                                         KC_NO, SCRL_MO, KC_TRNS, KC_NO, KC_NO,
    KC_NO,KC_NO,KC_NO,           KC_MS_BTN2, KC_MS_BTN1, KC_MS_BTN3,       KC_MS_BTN3, KC_MS_BTN1,    KC_NO,KC_NO,KC_NO,     KC_NO
  ),

  [3] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            RGB_RMOD , RGB_VAD , RGB_VAI , RGB_MOD , _______ ,
    _______ , KC_MPRV , KC_VOLU , KC_VOLD , KC_MNXT ,                            KC_MPRV , KC_VOLD , KC_VOLU , KC_MNXT , _______ ,
    _______ , _______ , _______ , _______ , _______ ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ ,      KC_TRNS , KC_MPLY , KC_MSTP ,      KC_MSTP , KC_MPLY ,       _______ , _______ , _______ , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    KC_LBRC , KC_7 , KC_8 , KC_9 , KC_RBRC ,                            _______ , _______ , _______ , _______ , _______ ,
    KC_SCLN , KC_4 , KC_5 , KC_6 , KC_PEQL ,                            _______ , MOD_RSFT , MOD_RGUI , MOD_RCTL , MOD_RALT ,
    KC_GRV , KC_1 , KC_2 , KC_3 , KC_BSLS ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , KC_PDOT , KC_P0 , KC_PMNS ,      _______ ,   KC_TRNS , _______ , _______ , _______ , _______
  ),

  [5] = LAYOUT_universal(
    S(KC_LBRC) , S(KC_7) , S(KC_8) , S(KC_9) , S(KC_RBRC) ,                      _______ , _______ , _______ , _______ , _______ ,
    S(KC_SCLN) , S(KC_4) , S(KC_5) , S(KC_6) , KC_PPLS ,                            _______ , MOD_RSFT , MOD_RGUI , MOD_RCTL , MOD_RALT ,
    S(KC_GRV) , S(KC_1) , S(KC_2) , S(KC_3) , S(KC_BSLS) ,                            _______ , _______ , _______ , _______ , _______ ,
    _______ , _______ , _______ , S(KC_9) , S(KC_0) , S(KC_MINS) ,      KC_TRNS ,   _______ , _______ , _______ , _______ , _______
  ),

  [6] = LAYOUT_universal(
    _______ , _______ , _______ , _______ , _______ ,                            KC_PSCR , KC_F7 , KC_F8 , KC_F9 , KC_F12 ,
    _______ , _______ , _______ , _______ , _______ ,                            KC_SCRL , KC_F4 , KC_F5 , KC_F6 , KC_F11 ,
    MOD_LALT , MOD_LCTL , MOD_LGUI , MOD_LSFT , _______ ,                        KC_PAUS , KC_F1 , KC_F2 , KC_F3 , KC_F10 ,
    _______ , _______ , _______ , _______ , _______ , KC_TRNS ,      _______ ,   _______ , _______ , _______ , _______ , _______
  ),

  [7] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
