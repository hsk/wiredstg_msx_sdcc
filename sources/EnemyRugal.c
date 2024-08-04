// EnemyRugal.c : 敵／ルグル
// よってくるやつ
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 敵を生成する
void EnemyRugalGenerate(void) {
    char* iy = enemyGenerator;
    #if 1
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
    return;
    #endif
    // 初期化の開始
    if (iy[ENEMY_GENERATOR_STATE] == 0) {
        iy[ENEMY_GENERATOR_LENGTH] = 3;// 生成数の設定
        iy[ENEMY_GENERATOR_TIMER] = 1;// タイマの設定
        // パラメータの設定
        iy[ENEMY_GENERATOR_PARAM_0] = (iy[ENEMY_GENERATOR_TYPE] == ENEMY_TYPE_RUGAL_BACK) ? 0xff : 0;
        iy[ENEMY_GENERATOR_STATE]++;// 初期化の完了
    }
    if (--iy[ENEMY_GENERATOR_TIMER]) return;// タイマの更新
    iy[ENEMY_GENERATOR_TIMER] = 0x10;
    char* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix[ENEMY_TYPE] = iy[ENEMY_GENERATOR_TYPE];// 敵の生成
    ix[ENEMY_STATE] = 0;
    ix[ENEMY_POSITION_X] = iy[ENEMY_GENERATOR_PARAM_0];
    char c = ((SystemGetRandom()&7)<<3);
    ix[ENEMY_POSITION_Y] = (c<<1)+c+0x20;
    ix[ENEMY_HP] = 0x01;
    // 生成数の更新
    if(--iy[ENEMY_GENERATOR_LENGTH]) return;
    // 生成の完了
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
}
// 敵を更新する
void EnemyRugalUpdate(char* ix) __naked {
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
    jr     nz,99$
    ld      h, ENEMY_POSITION_X(ix)
    ld      l, ENEMY_POSITION_Y(ix)
    call    _BulletGenerate
    call    _SystemGetRandom
    and     #0x3f
    add     #0x40
    ld      ENEMY_SHOT(ix), a
    jr 99$
    98$://}
    // 敵の削除
    xor     a
    ld      ENEMY_TYPE(ix), a
    99$:
    // 更新の完了
    pop ix
    ret
    __endasm;
}
