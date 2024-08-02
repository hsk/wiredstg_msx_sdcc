// EnemyRugal.c : 敵／ルグル
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 敵を生成する
void EnemyRugalGenerate(void) __naked {
    __asm;
    // ジェネレータの取得
    ld      iy, #_enemyGenerator
    // 初期化の開始
    ld      a, ENEMY_GENERATOR_STATE(iy)
    or      a
    jr      nz, 09$
        // 生成数の設定
        ld      a, #0x03
        ld      ENEMY_GENERATOR_LENGTH(iy), a
        // タイマの設定
        ld      a, #0x01
        ld      ENEMY_GENERATOR_TIMER(iy), a
        // パラメータの設定
        ld      a, ENEMY_GENERATOR_TYPE(iy)
        cp      #ENEMY_TYPE_RUGAL_BACK
        ld      a, #0xff
        adc     a, #0x00
        ld      ENEMY_GENERATOR_PARAM_0(iy), a
        // 初期化の完了
        inc     ENEMY_GENERATOR_STATE(iy)
    09$:
    // タイマの更新
    dec     ENEMY_GENERATOR_TIMER(iy)
    jr      nz, 19$
        ld      a, #0x10
        ld      ENEMY_GENERATOR_TIMER(iy), a
        // 生成の開始
        call    _EnemyGetEmpty
        jr      c, 19$
            // 敵の生成
            ld      a, ENEMY_GENERATOR_TYPE(iy)
            ld      ENEMY_TYPE(ix), a
            xor     a
            ld      ENEMY_STATE(ix), a
            ld      a, ENEMY_GENERATOR_PARAM_0(iy)
            ld      ENEMY_POSITION_X(ix), a
            call    _SystemGetRandom
            and     #0x07
            add     a, a
            add     a, a
            add     a, a
            ld      c, a
            add     a, a
            add     a, c
            add     a, #0x20
            ld      ENEMY_POSITION_Y(ix), a
            ld      a, #0x01
            ld      ENEMY_HP(ix), a
            // 生成数の更新
            dec     ENEMY_GENERATOR_LENGTH(iy)
            jr      nz, 19$
                // 生成の完了
                xor     a
                ld      ENEMY_GENERATOR_TYPE(iy), a
                ld      ENEMY_GENERATOR_STATE(iy), a
    19$:
    ret
    __endasm;
}
// 敵を更新する
void EnemyRugalUpdate(void) __naked {
    __asm;
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // パラメータの設定
        xor     a
        ld      ENEMY_PARAM_0(ix), a
        ld      a, ENEMY_TYPE(ix)
        sub     #ENEMY_TYPE_RUGAL_FRONT
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, #0x68
        ld      ENEMY_PARAM_1(ix), a
        // ショットの設定
        call    _SystemGetRandom
        and     #0x3f
        add     #0x40
        ld      ENEMY_SHOT(ix), a
        // アニメーションの設定
        ld      a, ENEMY_PARAM_1(ix)
        ld      ENEMY_ANIMATION(ix), a
        // タイマの設定
        ld      a, #0x40
        ld      ENEMY_TIMER(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // 移動
    //for(;;) {
    ld      a, ENEMY_TYPE(ix)
    cp      #ENEMY_TYPE_RUGAL_FRONT
    ld      a, ENEMY_POSITION_X(ix)
    jr      nz, 10$
        inc     a
        jr      z, 98$ // braak;
        jr      11$
    10$:
        or      a
        jr      z, 98$ // braak;
        dec     a
    11$:
    ld      ENEMY_POSITION_X(ix), a
    ld      a, ENEMY_POSITION_Y(ix)
    add     a, ENEMY_PARAM_0(ix)
    cp      #0xc0
    jr      nc, 98$
    ld      ENEMY_POSITION_Y(ix), a
    // 方向転換
    dec     ENEMY_TIMER(ix)
    jr      nz, 29$
        ld      a, (_ship + SHIP_POSITION_Y)
        sub     ENEMY_POSITION_Y(ix)
        jr      nc, 20$
            cp      #0xf8
            ld      a, #0x00
            sbc     #0x00
            jr      21$
        20$:
            cp      #0x08
            ld      a, #0x01
            sbc     #0x00
        21$:
        ld      ENEMY_PARAM_0(ix), a
        add     a, a
        add     a, ENEMY_PARAM_1(ix)
        ld      ENEMY_ANIMATION(ix), a
        call    _SystemGetRandom
        and     #0x3f
        add     a, #0x10
        ld      ENEMY_TIMER(ix), a
    29$:
    // ショットの更新
    dec     ENEMY_SHOT(ix)
    ret     nz
    ld      h, ENEMY_POSITION_X(ix)
    ld      l, ENEMY_POSITION_Y(ix)
    call    _BulletGenerate
    call    _SystemGetRandom
    and     #0x3f
    add     #0x40
    ld      ENEMY_SHOT(ix), a
    ret
    98$://}
    // 敵の削除
    xor     a
    ld      ENEMY_TYPE(ix), a
    // 更新の完了
    ret
    __endasm;
}
