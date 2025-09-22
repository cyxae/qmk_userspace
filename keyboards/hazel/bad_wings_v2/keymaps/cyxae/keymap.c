// Copyright 2024-2025 cyxae (@cyxae)
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include QMK_KEYBOARD_H

#define HRM_A     LGUI_T(KC_A)
#define HRM_S     LALT_T(KC_S)
#define HRM_D     LCTL_T(KC_D)
#define HRM_F     LSFT_T(KC_F)
#define HRM_J     RSFT_T(KC_J)
#define HRM_K     RCTL_T(KC_K)
#define HRM_L     LALT_T(KC_L)
#define HRM_SCLN  RGUI_T(KC_SCLN)
#define CKC_BSPC  LT(L_NAV, KC_BSPC)
#define CKC_SPC   LT(L_NUM, KC_SPC)
#define CKC_MIR   OSL(L_MIR)
#define ALT_F4    LALT(KC_F4)
#define KC_NBSPC  S(ALGR(KC_SPC))

#define CIRQUE_CPI 2000

enum layers {
  L_ALPHA,
  L_NUM,
  L_NAV,
  L_MIR,
  LAYER_LENGTH
};

enum custom_keycodes {
  ALT_TAB = SAFE_RANGE,
};

void pointing_device_init_user(void) {
  pointing_device_set_cpi(CIRQUE_CPI);
};

bool is_flow_tap_key(uint16_t keycode) {
  if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
    return false; // Disable Flow Tap on hotkeys.
  }
  switch (get_tap_keycode(keycode)) {
    case KC_A ... KC_Z:
    case KC_DOT:
    case KC_COMM:
    case KC_SCLN:
    case KC_SLSH:
        return true;
  }
  return false;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
  // If you quickly hold a tap-hold key after tapping it, the tap action is
  // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
  // lead to missed triggers in fast typing. Here, returning 0 means we
  // instead want to "force hold" and disable key repeating.
  switch (keycode) {
    case HRM_J:
      return QUICK_TAP_TERM;  // Enable key repeating.
    default:
      return 0;  // Otherwise, force hold and disable key repeating.
  }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CKC_BSPC:
      // Immediately select the hold action when another key is pressed.
      return true;
    default:
      // Do not select the hold action when another key is pressed.
      return false;
  }
}

// NumRow 1 + Clic gauche = Download mode to allow upgrade firmware with qmk flash
const uint16_t PROGMEM dfu_cmb[] = {KC_1, MS_BTN1, COMBO_END};

combo_t key_combos[] = {
  COMBO(dfu_cmb, QK_BOOTLOADER),
};

// Ctrl + PgDn = Ctrl + PgUp
const key_override_t ctrl_pgdn_override = ko_make_basic(MOD_MASK_CTRL, KC_PGDN, C(KC_PGUP));

// Ctrl + PgUp = Ctrl + PgDn
const key_override_t ctrl_pgup_override = ko_make_basic(MOD_MASK_CTRL, KC_PGUP, C(KC_PGDN));

// Left Ctrl + G = Ctrl + A
const key_override_t ctrl_g_override = ko_make_basic(MOD_BIT(KC_LCTL), KC_G, C(KC_SCLN));

const key_override_t *key_overrides[] = {
  &ctrl_pgdn_override,
  &ctrl_pgup_override,
  &ctrl_g_override,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_ALPHA] = LAYOUT_split_3x5_3(
    KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,             KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,
    HRM_A,      HRM_S,      HRM_D,      HRM_F,      KC_G,             KC_H,       HRM_J,      HRM_K,      HRM_L,      HRM_SCLN,
    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,             KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,
                            KC_LSFT,    CKC_BSPC,   MS_BTN1,          MS_BTN2,    CKC_SPC,    KC_RALT
  ),
  [L_NUM] = LAYOUT_split_3x5_3(
    KC_NO,      KC_7,       KC_8,       KC_9,       KC_NO,            KC_NO,      KC_F7,      KC_F8,      KC_F9,      KC_F10,
    KC_0,       KC_4,       KC_5,       KC_6,       KC_NO,            KC_NO,      KC_F4,      KC_F5,      KC_F6,      KC_F11,
    KC_KP_DOT,  KC_1,       KC_2,       KC_3,       KC_NO,            KC_NO,      KC_F1,      KC_F2,      KC_F3,      KC_F12,
                            KC_TRNS,    KC_TRNS,    KC_TRNS,          KC_TRNS,    KC_TRNS,    KC_NO
  ),
  [L_NAV] = LAYOUT_split_3x5_3(
    KC_WBAK,    ALT_TAB,    C(KC_N),    KC_WFWD,    ALT_F4,           S(KC_INS),  KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,
    KC_ESC,     KC_ENT,     KC_TAB,     S(KC_TAB),  KC_DEL,           KC_CAPS,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,
    KC_DOWN,    CKC_MIR,    KC_UP,      KC_SPC,     KC_BSPC,          KC_MPLY,    KC_MPRV,    KC_VOLD,    KC_VOLU,    KC_MNXT,
                            KC_NO,      KC_TRNS,    KC_TRNS,          KC_TRNS,    KC_NBSPC,   KC_NO
  ),
  [L_MIR] = LAYOUT_split_3x5_3(
    KC_P,       KC_O,       KC_I,       KC_U,       KC_Y,             KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
    HRM_SCLN,   HRM_L,      HRM_K,      HRM_J,      KC_H,             KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
    KC_SLSH,    KC_DOT,     KC_COMM,    KC_M,       KC_N,             KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,
                            KC_TRNS,    KC_TRNS,    KC_TRNS,          KC_NO,      KC_NO,      KC_NO
  )
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x5_3(
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R',
                  '*', '*', '*',  '*', '*', '*'
    );

bool is_alt_tab_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case ALT_TAB:
      if (record->event.pressed) {
        if (!(get_mods() & MOD_MASK_CTRL)) {
          if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
          }
        }
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      return false;

    case CKC_BSPC:
      if (!record->event.pressed) {
        if (is_alt_tab_active) {
          unregister_code(KC_LALT);
          is_alt_tab_active = false;
        }
      }
      return true;
  }
  return true;
}
