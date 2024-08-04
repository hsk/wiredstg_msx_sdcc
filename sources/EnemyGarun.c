// EnemyGarun.c : 敵／ガルン
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 定数の定義
// カーブ
static char const enemyGarunCurve[] = {
    0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02,
    0x02, 0x01, 0xff, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd,
    0xfc, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe,
    0xfe, 0xff, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03,
    //0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01,
    //0x00, 0x00, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfd,
    //0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe, 0xff, 0xff,
    //0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
};
// アニメーション
static char const enemyGarunAnimation[] = {
    0x80, 0x82, 0x84, 0x82,
    0xa0, 0xa2, 0xa4, 0xa2,
};
// 敵を生成する
void EnemyGarunGenerate(void) __naked {
    __asm;
    // ジェネレータの取得
    ld      iy, #_enemyGenerator
    // 初期化の開始
    ld      a, ENEMY_GENERATOR_STATE(iy)
    or      a
    jr      nz, 09$
        // 生成数の設定
        ld      a, #0x04
        ld      ENEMY_GENERATOR_LENGTH(iy), a
        // タイマの設定
        ld      a, #0x01
        ld      ENEMY_GENERATOR_TIMER(iy), a
        // 初期化の完了
        inc     ENEMY_GENERATOR_STATE(iy)
    09$:
    // タイマの更新
    dec     ENEMY_GENERATOR_TIMER(iy)
    ret     nz
    ld      a, #0x10
    ld      ENEMY_GENERATOR_TIMER(iy), a
    // 生成の開始
    call    _EnemyGetEmpty
    ld a,d
    or e
    ret     z
    push de
    pop ix
    // 敵の生成
    ld      a, ENEMY_GENERATOR_TYPE(iy)
    ld      ENEMY_TYPE(ix), a
    xor     a
    ld      ENEMY_STATE(ix), a
    ld      a, ENEMY_TYPE(ix)
    cp      #ENEMY_TYPE_GARUN_BACK
    ld      a, #0xff
    adc     a, #0x00
    ld      ENEMY_POSITION_X(ix), a
    call    _SystemGetRandom
    and     #0x03
    add     a, a
    add     a, a
    add     a, a
    add     a, a
    add     a, a
    add     a, #0x30
    ld      ENEMY_POSITION_Y(ix), a
    ld      a, #0x01
    ld      ENEMY_HP(ix), a
    // 生成数の更新
    dec     ENEMY_GENERATOR_LENGTH(iy)
    ret     nz
    // 生成の完了
    xor     a
    ld      ENEMY_GENERATOR_TYPE(iy), a
    ld      ENEMY_GENERATOR_STATE(iy), a
    ret
    __endasm;
}
// 敵を更新する
void EnemyGarunUpdate(char* ix) __naked {
    ix;
    __asm;
    push ix
    push hl
    pop ix
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // パラメータの設定
        xor     a
        ld      ENEMY_PARAM_0(ix), a
        // ショットの設定
        call    _SystemGetRandom
        and     #0x1f
        add     #0x10
        ld      ENEMY_SHOT(ix), a
        // アニメーションの設定
        //xor     a
        //ld      ENEMY_ANIMATION(ix), a
        // タイマの設定
        xor     a
        ld      ENEMY_TIMER(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // for(;;) {
        // 移動（←→）
        ld      a, ENEMY_TYPE(ix)
        cp      #ENEMY_TYPE_GARUN_FRONT
        ld      a, ENEMY_POSITION_X(ix)
        jr      nz, 10$
            add     a, #0x02
            jr      c, 98$// break;
            jr      11$
        10$:
            sub     #0x02
            jr      c, 98$// break;
        11$:
        ld      ENEMY_POSITION_X(ix), a
        // 移動（↑↓）
        inc     ENEMY_PARAM_0(ix)
        ld      a, ENEMY_PARAM_0(ix)
        and     #0x1f
        ld      e, a
        ld      d, #0x00
        ld      hl, #_enemyGarunCurve
        add     hl, de
        ld      a, (hl)
        add     a, ENEMY_POSITION_Y(ix)
        cp      #0xc0
        jr      nc, 98$ // break
        ld      ENEMY_POSITION_Y(ix), a
        // ショットの更新
        dec     ENEMY_SHOT(ix)
        jr      nz, 91$
            ld      h, ENEMY_POSITION_X(ix)
            ld      l, ENEMY_POSITION_Y(ix)
            call    _BulletGenerate
            call    _SystemGetRandom
            and     #0x1f
            add     #0x18
            ld      ENEMY_SHOT(ix), a
            // アニメーションの更新
        91$:
        inc     ENEMY_TIMER(ix)
        ld      a, ENEMY_TYPE(ix)
        sub     #ENEMY_TYPE_GARUN_FRONT
        add     a, a
        add     a, a
        ld      e, a
        ld      a, ENEMY_TIMER(ix)
        and     #0x0c
        srl     a
        srl     a
        add     a, e
        ld      e, a
        ld      d, #0x00
        ld      hl, #_enemyGarunAnimation
        add     hl, de
        ld      a, (hl)
        ld      ENEMY_ANIMATION(ix), a
        jr 99$
        // 敵の削除
    98$:
    xor     a
    ld      ENEMY_TYPE(ix), a
    // 更新の完了
    99$:
    pop ix
    ret
    __endasm;
}
