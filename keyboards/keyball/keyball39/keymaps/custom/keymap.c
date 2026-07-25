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

enum keyball_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAV,
    LAYER_POINTER,
    LAYER_MEDIA,
    LAYER_NUMBER,
    LAYER_SYMBOL,
    LAYER_FUNCTION,
    LAYER_ADJUST,
};

// Home row mods — Graphite NRTS / HAEI
// pinky GUI, ring Alt, middle Ctrl, index Shift
#define HM_N  MT(MOD_LGUI, KC_N)
#define HM_R  MT(MOD_LALT, KC_R)
#define HM_T  MT(MOD_LCTL, KC_T)
#define HM_S  MT(MOD_LSFT, KC_S)
#define HM_H  MT(MOD_RSFT, KC_H)
#define HM_A  MT(MOD_RCTL, KC_A)
#define HM_E  MT(MOD_RALT, KC_E)
#define HM_I  MT(MOD_RGUI, KC_I)

// Dedicated mods (same fingers as HRMs)
#define MOD_N  KC_LGUI
#define MOD_R  KC_LALT
#define MOD_T  KC_LCTL
#define MOD_S  KC_LSFT
#define MOD_H  KC_RSFT
#define MOD_A  KC_RCTL
#define MOD_E  KC_RALT
#define MOD_I  KC_RGUI

#define PT_M   LT(LAYER_POINTER, KC_M)
#define PT_DOT LT(LAYER_POINTER, KC_DOT)
#define TH_L1  LT(LAYER_MEDIA, KC_ESC)
#define TH_L2  LT(LAYER_NAV, KC_SPC)
#define TH_L3  LT(LAYER_FUNCTION, KC_TAB)
#define TH_R1  LT(LAYER_SYMBOL, KC_ENT)
#define TH_R2  LT(LAYER_NUMBER, KC_BSPC)

// copy, cut, paste undo, redo
#define M_REDO  LCTL(KC_Y)
#define M_PASTE LCTL(KC_V)
#define M_COPY  LCTL(KC_C)
#define M_CUT   LCTL(KC_X)
#define M_UNDO  LCTL(KC_Z)

// Graphite nonstandard shift pairs
const key_override_t quote_underscore = ko_make_basic(MOD_MASK_SHIFT, KC_QUOT, KC_UNDS);
const key_override_t comma_question   = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_QUES);
const key_override_t minus_dquote     = ko_make_basic(MOD_MASK_SHIFT, KC_MINS, KC_DQUO);
const key_override_t slash_lt         = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_LT);

const key_override_t *key_overrides[] = {
    &quote_underscore,
    &comma_question,
    &minus_dquote,
    &slash_lt,
};

// Set tap hold delay for each modifier individually
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record, uint16_t prev_keycode) {
    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        switch (keycode) {
            case HM_S:
            case HM_H:
                return 0;
            default:
                return FLOW_TAP_TERM;  // use the global macro directly
        }
    }
    return 0;
}

// Set tapping term for each modifier individually
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HM_N:
        case HM_I:   // pinky: slow, give more time
            return TAPPING_TERM + 45;
        case HM_R:
        case HM_E:    // ring
            return TAPPING_TERM + 20;
        case HM_S:
        case HM_H:    // index shifts: fast, can be shorter
            return TAPPING_TERM - 30;
        default:
            return TAPPING_TERM;
    }
}

// HOLD_ON_OTHER_KEY_PRESS for thumbs (layers) and index shifts only.
// Chordal Hold still forces same-hand shift rolls to tap.
// Other HRMs and pointer LTs stay off HoOKP so rolls remain taps.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TH_L1:
        case TH_L2:
        case TH_L3:
        case TH_R1:
        case TH_R2:
        case HM_S: // left shift
        case HM_H: // right shift
            return true;
        default:
            return false;
    }
}

// Home-row mods use chordal hold (opposite hands); everything else bypasses it.
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case HM_N: case HM_R: case HM_T: case HM_S:
        case HM_H: case HM_A: case HM_E: case HM_I:
            return get_chordal_hold_default(tap_hold_record, other_record);
        default:
            return true;  // bypass chordal-hold for everything else
    }
}

// L/R for alpha keys; * exempts thumbs so same-hand layer chords still hold
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_universal(
    'L', 'L', 'L', 'L', 'L',              'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',              'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',              'R', 'R', 'R', 'R', 'R',
    '*', '*', '*', '*', '*', '*',      '*', '*', '*', '*', '*', '*'
);

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
  [LAYER_BASE] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
        KC_B,    KC_L,    KC_D,    KC_W,    KC_Z,      KC_QUOT,    KC_F,    KC_O,    KC_U,    KC_J,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
        HM_N,    HM_R,    HM_T,    HM_S,    KC_G,        KC_Y,    HM_H,    HM_A,    HM_E,    HM_I,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
        KC_Q,    KC_X,    PT_M,    KC_C,    KC_V,        KC_K,    KC_P,  PT_DOT, KC_MINS, KC_SLSH,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
      KC_NO,  KC_NO,  KC_NO,  TH_L1,  TH_L2,  TH_L3,      TH_R1,  TH_R2,  KC_NO,  KC_NO,  KC_NO,  KC_NO
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,       M_REDO, M_PASTE,  M_COPY,   M_CUT,  M_UNDO,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
       MOD_N,   MOD_R,   MOD_T,   MOD_S,   KC_NO,      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT,  KC_DEL,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
       KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_HOME, KC_PGDN, KC_PGUP,  KC_END,  KC_INS,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
      KC_NO,  KC_NO,  KC_NO,_______,KC_TRNS,_______,      KC_ENT, KC_BSPC,  KC_NO,  KC_NO,  KC_NO,  KC_NO
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
     QK_BOOT,  EE_CLR,   KC_NO,   KC_NO,   KC_NO,       M_REDO, M_PASTE,  M_COPY,   M_CUT,  M_UNDO,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
       MOD_N,   MOD_R,   KC_NO,   KC_NO,   KC_NO,        KC_NO,SCRL_DVD,SCRL_DVI,   KC_NO,   KC_NO,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
       KC_NO,   KC_NO, KC_TRNS, SCRL_MO,   KC_NO,        KC_NO, SCRL_MO, KC_TRNS,   KC_NO,   KC_NO,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  KC_NO,KC_NO,KC_NO,KC_MS_BTN2,KC_MS_BTN1,KC_MS_BTN3,  KC_MS_BTN3,KC_MS_BTN1,KC_NO,KC_NO,KC_NO,KC_NO
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_MEDIA] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
    _______,_______,_______,_______,_______,      RGB_RMOD, RGB_VAD, RGB_VAI, RGB_MOD,_______,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
     KC_MUTE, KC_MPRV, KC_VOLU, KC_VOLD, KC_MNXT,      KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MUTE,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
    _______,_______,_______,_______,_______,      _______,_______,_______,_______,_______,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  _______,_______,_______,KC_TRNS,KC_MPLY,KC_MSTP,      KC_MSTP,KC_MPLY,_______,_______,_______,QK_BOOT
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_NUMBER] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
     KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC,     _______,_______,_______,_______,_______,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
     KC_COMM,    KC_4,    KC_5,    KC_6, KC_PEQL,     _______,   MOD_H,   MOD_A,   MOD_E,   MOD_I,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
      KC_SCLN,    KC_1,    KC_2,    KC_3, KC_BSLS,     _______,_______,_______,_______,_______,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  _______,_______,_______,KC_GRV,  KC_P0,KC_NO,    _______,KC_TRNS,_______,_______,_______,_______
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_SYMBOL] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
     KC_LCBR, KC_LPRN, KC_ASTR, KC_RPRN, KC_RCBR,     _______,_______,_______,_______,_______,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
     KC_QUES,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,     _______,   MOD_H,   MOD_A,   MOD_E,   MOD_I,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
     KC_COLN, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE,     _______,_______,_______,_______,_______,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  _______,_______,_______,KC_TILD,KC_AMPR,KC_NO,    KC_TRNS,_______,_______,_______,_______,_______
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_FUNCTION] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
    _______,_______,_______,_______,_______,      KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
       MOD_N,   MOD_R,   MOD_T,   MOD_S,_______,      KC_SCRL,   KC_F4,   KC_F5,   KC_F6,  KC_F11,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
    _______,_______,_______,_______,_______,      KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  _______,_______,_______,_______,_______,KC_TRNS,    _______, KC_APP,_______,_______,_______,_______
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),

  [LAYER_ADJUST] = LAYOUT_universal(
  // ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
     RGB_TOG,  AML_TO, AML_I50, AML_D50,_______,     _______,_______,SSNP_HOR,SSNP_VRT,SSNP_FRE,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
     RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,SCRL_DVI,     _______,_______,_______,_______,_______,
  // ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
    RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD,SCRL_DVD,     CPI_D1K,CPI_D100,CPI_I100, CPI_I1K,KBC_SAVE,
  // ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
  QK_BOOT,KBC_RST,_______,_______,_______,_______,    _______,_______,_______,_______,KBC_RST,QK_BOOT
  //                   ╰───────────────────────╯ ╰──────────────────────────────────────────╯
  ),
};
// clang-format on

// layer_state_t layer_state_set_user(layer_state_t state) {
//     // Auto enable scroll mode when the highest layer is LAYER_POINTER
//     keyball_set_scroll_mode(get_highest_layer(state) == LAYER_POINTER);
//     return state;
// }

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
