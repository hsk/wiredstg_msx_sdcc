// Ground.c : 地面
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ground.h"
#include "string.h"
// 変数の定義
char ground[0x0300];// 地面
char groundGenerator[GROUND_GENERATOR_SIZE];// ジェネレータ
static char groundGeneratorRow[0x18];
// 地面を初期化する
void GroundInitialize(void) {
    memset(ground,0,0x300);// 地面の初期化
    // ジェネレータの初期化
    groundGenerator[GROUND_GENERATOR_UPPER_STATE] = 0;
    groundGenerator[GROUND_GENERATOR_LOWER_STATE] = 0;
    groundGenerator[GROUND_GENERATOR_UPPER_HEIGHT] = 0;
    groundGenerator[GROUND_GENERATOR_LOWER_HEIGHT] = 0;
    groundGenerator[GROUND_GENERATOR_UPPER_LENGTH] = 1;
    groundGenerator[GROUND_GENERATOR_LOWER_LENGTH] = 1;
}
// 地面を更新する
void GroundUpdate(void) __naked {
    __asm;
    // スクロールの開始
    ld      a, (_gameScroll)
    or      a
    ret     nz
    // １キャラ分のスクロール
    ld      hl, #(_ground + 0x02fe)
    ld      de, #(_ground + 0x02ff)
    ld      bc, #0x02ff
    lddr
    // 生成する列の初期化
    ld      hl, #(_groundGeneratorRow + 0x0000)
    ld      de, #(_groundGeneratorRow + 0x0001)
    ld      bc, #0x0017
    xor     a
    ld      (hl), a
    ldir
    // ジェネレータの取得
    ld      ix, #_groundGenerator
    // ↑の生成
    ld      hl, #(_groundGeneratorRow + 0x0001)
    ld      a, GROUND_GENERATOR_UPPER_STATE(ix)
    // 地面なし／↑
    or      a
    jr      nz, 10$
        dec     GROUND_GENERATOR_UPPER_LENGTH(ix)
        jp      nz, 19$
        ld      a, #0x4a
        ld      (hl), a
        inc     GROUND_GENERATOR_UPPER_HEIGHT(ix)
        call    _SystemGetRandom
        and     #0x1f
        add     a, #0x11
        ld      GROUND_GENERATOR_UPPER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_FLAT
        ld      GROUND_GENERATOR_UPPER_STATE(ix), a
        jr      19$
        // 平地／↑
    10$:
        cp      #GROUND_GENERATOR_STATE_FLAT
        jr      nz, 13$
        ld      b, GROUND_GENERATOR_UPPER_HEIGHT(ix)
        dec     b
        jr      z, 12$
        ld      a, #0x4f
    11$:
        ld      (hl), a
        inc     hl
        djnz    11$
    12$:
        ld      a, #0x48
        ld      (hl), a
        dec     GROUND_GENERATOR_UPPER_LENGTH(ix)
        jr      nz, 18$
        call    _SystemGetRandom
        and     #0x07
        inc     a
        ld      GROUND_GENERATOR_UPPER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_UP
        ld      GROUND_GENERATOR_UPPER_STATE(ix), a
        jr      18$
        // 上る／↑
    13$:
        cp      #GROUND_GENERATOR_STATE_UP
        jr      nz, 15$
        ld      b, GROUND_GENERATOR_UPPER_HEIGHT(ix)
        ld      a, #0x4f
    14$:
        ld      (hl), a
        inc     hl
        djnz    14$
        ld      a, #0x4a
        ld      (hl), a
        inc     GROUND_GENERATOR_UPPER_HEIGHT(ix)
        dec     GROUND_GENERATOR_UPPER_LENGTH(ix)
        jr      nz, 18$
        ld      a, GROUND_GENERATOR_UPPER_HEIGHT(ix)
        dec     a
        ld      GROUND_GENERATOR_UPPER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_DOWN
        ld      GROUND_GENERATOR_UPPER_STATE(ix), a
        jr      18$
        // 下る／↑
    15$:
    //   cp      #GROUND_GENERATOR_STATE_DOWN
    //   jr      nz, 19$
        ld      b, GROUND_GENERATOR_UPPER_HEIGHT(ix)
        dec     b
        ld      a, #0x4f
    16$:
        ld      (hl), a
        inc     hl
        djnz    16$
        ld      a, #0x4b
        ld      (hl), a
        dec     GROUND_GENERATOR_UPPER_HEIGHT(ix)
        dec     GROUND_GENERATOR_UPPER_LENGTH(ix)
        jr      nz, 18$
        call    _SystemGetRandom
        and     #0x1f
        add     a, #0x11
        ld      GROUND_GENERATOR_UPPER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_FLAT
        ld      GROUND_GENERATOR_UPPER_STATE(ix), a
    //   jr      18$
        // ↑の生成の完了
    18$:
    19$:
    // ↓の生成
    ld      hl, #(_groundGeneratorRow + 0x0017)
    ld      a, GROUND_GENERATOR_LOWER_STATE(ix)
    // 地面なし／↓
    or      a
    jr      nz, 20$
        dec     GROUND_GENERATOR_LOWER_LENGTH(ix)
        jp      nz, 29$
        ld      a, #0x4b
        ld      (hl), a
        inc     GROUND_GENERATOR_LOWER_HEIGHT(ix)
        call    _SystemGetRandom
        and     #0x1f
        add     a, #0x11
        ld      GROUND_GENERATOR_LOWER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_FLAT
        ld      GROUND_GENERATOR_LOWER_STATE(ix), a
        jr      29$
        // 平地／↓
    20$:
        cp      #GROUND_GENERATOR_STATE_FLAT
        jr      nz, 23$
        ld      b, GROUND_GENERATOR_LOWER_HEIGHT(ix)
        dec     b
        jr      z, 22$
        ld      a, #0x4f
    21$:
        ld      (hl), a
        dec     hl
        djnz    21$
    22$:
        ld      a, #0x49
        ld      (hl), a
        dec     GROUND_GENERATOR_LOWER_LENGTH(ix)
        jr      nz, 28$
        call    _SystemGetRandom
        and     #0x07
        inc     a
        ld      GROUND_GENERATOR_LOWER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_UP
        ld      GROUND_GENERATOR_LOWER_STATE(ix), a
        jr      28$
        // 上る／↓
    23$:
        cp      #GROUND_GENERATOR_STATE_UP
        jr      nz, 25$
        ld      b, GROUND_GENERATOR_LOWER_HEIGHT(ix)
        ld      a, #0x4f
    24$:
        ld      (hl), a
        dec     hl
        djnz    24$
        ld      a, #0x4b
        ld      (hl), a
        inc     GROUND_GENERATOR_LOWER_HEIGHT(ix)
        dec     GROUND_GENERATOR_LOWER_LENGTH(ix)
        jr      nz, 28$
        ld      a, GROUND_GENERATOR_LOWER_HEIGHT(ix)
        dec     a
        ld      GROUND_GENERATOR_LOWER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_DOWN
        ld      GROUND_GENERATOR_LOWER_STATE(ix), a
        jr      28$
        // 下る／↓
    25$:
    //   cp      #GROUND_GENERATOR_STATE_DOWN
    //   jr      nz, 29$
        ld      b, GROUND_GENERATOR_LOWER_HEIGHT(ix)
        dec     b
        ld      a, #0x4f
    26$:
        ld      (hl), a
        dec     hl
        djnz    26$
        ld      a, #0x4a
        ld      (hl), a
        dec     GROUND_GENERATOR_LOWER_HEIGHT(ix)
        dec     GROUND_GENERATOR_LOWER_LENGTH(ix)
        jr      nz, 28$
        call    _SystemGetRandom
        and     #0x1f
        add     a, #0x08
        ld      GROUND_GENERATOR_LOWER_LENGTH(ix), a
        ld      a, #GROUND_GENERATOR_STATE_FLAT
        ld      GROUND_GENERATOR_LOWER_STATE(ix), a
        //jr      28$
        // ↓の生成の完了
    28$:
    29$:
    // 列の転送
    ld      hl, #(_groundGeneratorRow + 0x0001)
    ld      ix, #(_ground + 0x0020)
    ld      de, #0x0020
    ld      b, #0x17
    90$:
        ld      a, (hl)
        ld      0(ix), a
        inc     hl
        add     ix, de
    djnz    90$
    // スクロールの完了
    ret
    __endasm;
}
// 地面を描画する
void GroundRender(void) {
    memcpy(appPatternName,ground,0x300);// パターンネームの描画
}
