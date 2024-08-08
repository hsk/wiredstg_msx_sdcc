// Ship.c : 自機
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "string.h"
// 定数の定義
// 自機の初期値
typedef union SHORT {
    short v;
    struct {
        char l;
        char h;
    };
} SHORT;
typedef struct SHIP {
    char kind;//0
    char state;//1
    char x;//2
    char y;//3
    char hp;//4
    char speed;//5
    char animation;//6
    char timer;//7
    SHORT shot;//8
    char dummy0;//a
    char dummy1;//b
    char* sprite0;//c
    char* sprite1;//e
} SHIP;
static SHIP const shipDefault = {
    SHIP_TYPE_VICVIPER,
    SHIP_STATE_NULL,
    0xc0, 0x60,
    0x01,
    0x02,
    0x00,
    0x00,
    {0x0000},
    0x00, 0x00,
    &sprite[GAME_SPRITE_SHIP_0],
    &sprite[GAME_SPRITE_SHIP_1]
};
// スプライト
static char const shipSprite[] = {
    0xf7, 0xf4, 0x20, 0x00, 0xf7, 0x04, 0x24, 0x00,
    0xf7, 0xf4, 0x28, 0x00, 0xf7, 0x04, 0x2c, 0x00,
    0xf7, 0xf4, 0x30, 0x00, 0xf7, 0x04, 0x34, 0x00,
    0xf7, 0xf8, 0x10, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x14, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x18, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x1c, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
};
// ＳＥ
static char const shipSe[] =
    //"T1V15L0O4GO2D-O4EO2D-O4CO2D-O3GO2D-O3EO2D-O3CO2D-O2GD-ED-"
    "T1V15L0O3GO1D-O3EO1D-O3CO1D-O2GO1D-O2EO1D-O2CO1D-O1GD-ED-";
// 変数の定義
char ship[SHIP_SIZE];// 自機
// 自機を初期化する
void ShipInitialize(void) {
    memcpy(ship,(void*)shipDefault,SHIP_SIZE);// 自機の初期化
}
static void ShipPlay(void);
static void ShipBomb(void);
// 自機を更新する
void ShipUpdate(void) {
    __asm;
    // 自機の取得
    ld      ix, #_ship
    // 種類別の処理
    ld      a, SHIP_TYPE(ix)
    cp      #SHIP_TYPE_VICVIPER
    jr      nz, 10$
        call    _ShipPlay
        jr      19$
    10$:
        cp      #SHIP_TYPE_BOMB
        jr      nz, 19$
        call    _ShipBomb
        jr      19$
    19$:    
    // スプライトの更新
    ld      hl, (_ship + SHIP_SPRITE_0)
    ld      de, (_ship + SHIP_SPRITE_1)
    ld      (_ship + SHIP_SPRITE_0), de
    ld      (_ship + SHIP_SPRITE_1), hl
    ret
    __endasm;
} 
// 自機を操作する
static void ShipPlay(void) {
    __asm;
    // スピードアップ
    ld      a, (_input + INPUT_BUTTON_SHIFT)
    dec     a
    jr      nz, 19$
        ld      a, SHIP_SPEED(ix)
        inc     a
        cp      #0x05
        jr      c, 10$
            ld      a, #0x01
        10$:
        ld      SHIP_SPEED(ix), a
    19$:
    // ↑↓の移動
    ld      a, (_input + INPUT_KEY_UP)
    or      a
    jr      z, 21$
        ld      a, SHIP_POSITION_Y(ix)
        sub     SHIP_SPEED(ix)
        cp      #0x10
        jr      nc, 20$
            ld      a, #0x10
        20$:
        ld      SHIP_POSITION_Y(ix), a
        ld      a, #0x02
        jr      23$
    21$:
        ld      a, (_input + INPUT_KEY_DOWN)
        or      a
        jr      z, 23$
            ld      a, SHIP_POSITION_Y(ix)
            add     SHIP_SPEED(ix)
            cp      #0xb7
            jr      c, 22$
                ld      a, #0xb7
    22$:
        ld      SHIP_POSITION_Y(ix), a
        ld      a, #0x01
    23$:
    ld      SHIP_ANIMATION(ix), a
    // ←→の移動
    ld      a, (_input + INPUT_KEY_LEFT)
    or      a
    jr      z, 25$
    ld      a, SHIP_POSITION_X(ix)
    sub     SHIP_SPEED(ix)
    cp      #0x0c
    jr      nc, 24$
        ld      a, #0x0c
    24$:
        ld      SHIP_POSITION_X(ix), a
        jr      27$
    25$:
        ld      a, (_input + INPUT_KEY_RIGHT)
        or      a
        jr      z, 27$
            ld      a, SHIP_POSITION_X(ix)
            add     SHIP_SPEED(ix)
            cp      #0xf6
            jr      c, 26$
                ld      a, #0xf6
    26$:
        ld      SHIP_POSITION_X(ix), a
    27$:
    // ショット
    ld      a, (_input + INPUT_BUTTON_SPACE)
    dec     a
    jr      nz, 39$
        call    _ShotGenerate
        ld      l, SHIP_SHOT_L(ix)
        ld      h, SHIP_SHOT_H(ix)
        inc     hl
        ld      SHIP_SHOT_L(ix), l
        ld      SHIP_SHOT_H(ix), h
    39$:
    ret
    __endasm;
}
// 自機が爆発する
static void ShipBomb(void) {
    __asm;
    // 初期化の開始
    ld      a, SHIP_STATE(ix)
    or      a
    jr      nz, 09$
        // アニメーションの設定
        ld      a, #0x03
        ld      SHIP_ANIMATION(ix), a
        // タイマの設定
        ld      a, #0x04
        ld      SHIP_TIMER(ix), a
        // ＳＥの再生
        ld      hl, #_shipSe
        ld      (_soundRequest + 0x0006), hl
        // 初期化の完了
        inc     SHIP_STATE(ix)
    09$:
    // アニメーションの更新
    dec     SHIP_TIMER(ix)
    jr      nz, 19$
    ld      a, #0x04
    ld      SHIP_TIMER(ix), a
    inc     SHIP_ANIMATION(ix)
    ld      a, SHIP_ANIMATION(ix)
    cp      #0x0b
    jr      nz, 19$
        // 自機の削除
        xor     a
        ld      SHIP_TYPE(ix), a
        ld      SHIP_STATE(ix), a
    19$:
    ret
    __endasm;
}
// 自機を描画する
void ShipRender(void) {
    __asm;
    // スプライトの描画
    ld      ix, #_ship
    ld      a, SHIP_TYPE(ix)
    or      a
    jr      z, 9$
        ld      a, SHIP_ANIMATION(ix)
        add     a, a
        add     a, a
        add     a, a
        ld      e, a
        ld      d, #0x00
        ld      hl, #_shipSprite
        add     hl, de
        ex      de, hl
        ld      hl, (_ship + SHIP_SPRITE_0)
        ld      a, (_appColor)
        ld      c, a
        ld      a, (de)
        add     a, SHIP_POSITION_Y(ix)
        ld      (hl), a
        inc     hl
        inc     de
        ld      a, (de)
        add     a, SHIP_POSITION_X(ix)
        ld      (hl), a
        inc     hl
        inc     de
        ld      a, (de)
        ld      (hl), a
        inc     hl
        inc     de
        ld      (hl), c
        inc     hl
        inc     de
        //   ld      hl, (_ship + SHIP_SPRITE_1)
        ld      a, (de)
        add     a, SHIP_POSITION_Y(ix)
        ld      (hl), a
        inc     hl
        inc     de
        ld      a, (de)
        add     a, SHIP_POSITION_X(ix)
        ld      (hl), a
        inc     hl
        inc     de
        ld      a, (de)
        ld      (hl), a
        inc     hl
        inc     de
        ld      (hl), c
        //inc     hl
        //inc     de
    9$:
    ret
    __endasm;
}
