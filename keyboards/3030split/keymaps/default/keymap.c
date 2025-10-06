
#include QMK_KEYBOARD_H

enum layer_names {
    _BL,
    _SL,
};

enum unicode_names {
    A_UMLT_KL,
    O_UMLT_KL,
    U_UMLT_KL,
    A_UMLT_GR,
    O_UMLT_GR,
    U_UMLT_GR,
    EURO,
    SZ_KL,
    SZ_GR,
};

const uint32_t PROGMEM unicode_map[] = {
    [A_UMLT_KL] = 0x00E4 , //ä
    [O_UMLT_KL] = 0x00F6 , //ö
    [U_UMLT_KL] = 0x00FC , //ü

    [A_UMLT_GR] = 0x00C4 , //Ä
    [O_UMLT_GR] = 0x00D6 , //Ö
    [U_UMLT_GR] = 0x00DC , //Ü

    [EURO] = 0x20AC , //€

    [SZ_KL] = 0x00DF , //ß
    [SZ_GR] = 0x1E9E 
};


const key_override_t eql_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_EQL, KC_HASH); // Shift = is #
const key_override_t ques_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_EXLM); // Shift ? is !
const key_override_t dot_key_override = 
    ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);
const key_override_t comma_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN);

const key_override_t* key_overrides[] = {
    &eql_key_override,
    &ques_key_override,
    &dot_key_override,
    &comma_key_override,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┐
     * │ X │ Q │ ? │Tab│ ß │ ┌───┐               │ = │   │   │   │   │
     * ├───┼───┼───┼───┼───┤ │ ↑ │               ├───┼───┼───┼───┼───┤
     * │ W │ Z │ M │ G │ J │ │ ↓ │               │ ' │ . │ Ä │ Ö │ Ü │
     * ├───┼───┼───┼───┼───│ └───┘               ├───┼───┼───┼───┼───┤
     * │ C │ S │ N │ T │ K │    ┌───┐            │ , │ A │ E │ I │ H │
     * ├───┼───┼───┼───┼───┤    │   │            ├───┼───┼───┼───┼───┤
     * │ P │ F │ L │ D │ V │    └───┘            │ - │ U │ O │ Y │ B │
     * └───┴───┴───┴───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┴───┴───┴───┘
     *                       │ R │Bck│ │Etr│Spc│
     *                       ├───┼───┤ ├───┼───┤
     *                       │Sft│   │ │Mod│Ctr│
     *                       ├───┼───┤ ├───┼───┤
     *                       │End│Pos│ │Win│Del│
     *                       ├───┼───┤ ├───┼───┤
     *                       │ ← │ ↑ │ │ ↓ │ → │  
     *                       └───┴───┘ └───┴───┘   
     */
    [_BL] = LAYOUT(
        KC_X,    KC_Q,   KC_QUES, KC_TAB, UP(7,8),  KC_DOWN,    KC_LEFT,      KC_SPC, KC_ENT,   KC_EQL,   KC_ESC,   KC_NO,   KC_NO,   KC_NO,     
        KC_W,    KC_Z,   KC_M,    KC_G,   KC_J,     KC_HOME,  KC_END,       KC_LCTL, MO(_SL), KC_QUOT,  KC_DOT,  UP(0,3), UP(1,4), UP(2,5),    
        KC_C,    KC_S,   KC_N,    KC_T,   KC_K,     KC_LALT,  KC_LSFT,      KC_DEL, KC_LGUI,  KC_COMM,  KC_A,    KC_E,    KC_I,    KC_H,        
        KC_P,    KC_F,   KC_L,    KC_D,   KC_V,     KC_BSPC,  KC_R,         KC_RGHT, KC_UP, KC_MINS,  KC_U,    KC_O,    KC_Y,    KC_B   
    ),
        /*
     * ┌───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┐
     * │   │   │   │   │   │ ┌───┐               │ = │   │   │   │   │
     * ├───┼───┼───┼───┼───┤ │ ↑ │               ├───┼───┼───┼───┼───┤
     * │ | │ < │ [ │ ( │ { │ │ ↓ │               │ } │ ) │ ] │ > │ & │
     * ├───┼───┼───┼───┼───│ └───┘               ├───┼───┼───┼───┼───┤
     * │ 5 │ 4 │ 3 │ 2 │ 1 │    ┌───┐            │ 0 │ 6 │ 7 │ 8 │ 9 │
     * ├───┼───┼───┼───┼───┤    │   │            ├───┼───┼───┼───┼───┤
     * │ ^ │ " │ * │ / │ + │    └───┘            │ - │ \ │ % │ ~ │ @ │
     * └───┴───┴───┴───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┴───┴───┴───┘
     *                       │ $ │   │ │   │   │
     *                       ├───┼───┤ ├───┼───┤
     *                       │   │   │ │   │   │
     *                       ├───┼───┤ ├───┼───┤
     *                       │PUp│PDn│ │   │   │
     *                       ├───┼───┤ ├───┼───┤
     *                       │   │ € │ │UCL│UCW│  
     *                       └───┴───┘ └───┴───┘   
     */
     [_SL] = LAYOUT(
        _______, _______, _______, _______, _______,  _______, UM(EURO),      UC_WINC, UC_LINX, _______,  _______, _______, _______, QK_BOOT,        
        KC_PIPE, KC_LABK, KC_LBRC, KC_LPRN, KC_LCBR,  KC_PGUP, KC_PGDN,       _______, _______, KC_RCBR, KC_RPRN, KC_RBRC, KC_RABK,  KC_AMPR,      
        KC_5,    KC_4,    KC_3,    KC_2,    KC_1,     _______, _______,       _______, _______, KC_0,    KC_6,    KC_7,    KC_8,     KC_9,      
        KC_CIRC, KC_DQUO, KC_ASTR, KC_SLSH, KC_PLUS,  _______, KC_DLR,       _______, _______, KC_MINS, KC_BSLS, KC_PERC, KC_TILD,  KC_AT
     )
};