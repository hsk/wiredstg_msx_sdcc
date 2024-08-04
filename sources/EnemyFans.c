// EnemyFans.s : 敵／ファン
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 敵を生成する
void EnemyFansGenerate(void) {
    char *iy = enemyGenerator;
    #if 1
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
    return;
    #endif
    // 初期化の開始
    if (iy[ENEMY_GENERATOR_STATE] == 0) {
        // 生成数の設定
        iy[ENEMY_GENERATOR_LENGTH] = (SystemGetRandom()&1)*2+4;
        // タイマの設定
        iy[ENEMY_GENERATOR_TIMER] = 1;
        // パラメータの設定
        iy[ENEMY_GENERATOR_PARAM_0] = (iy[ENEMY_GENERATOR_TYPE] == ENEMY_TYPE_FANS_BACK) ? 0xff : 0;
        iy[ENEMY_GENERATOR_PARAM_1] = (SystemGetRandom()&0x80)+0x20;
        iy[ENEMY_GENERATOR_STATE]++;// 初期化の完了
    }
    // タイマの更新
    if(--iy[ENEMY_GENERATOR_TIMER])return;
    iy[ENEMY_GENERATOR_TIMER] = 0x04;
    // 生成の開始
    char* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix[ENEMY_TYPE] = iy[ENEMY_GENERATOR_TYPE];// 敵の生成
    ix[ENEMY_STATE] = 0;
    ix[ENEMY_POSITION_X] = iy[ENEMY_GENERATOR_PARAM_0];
    ix[ENEMY_POSITION_Y] = iy[ENEMY_GENERATOR_PARAM_1];
    ix[ENEMY_HP] = 0x01;
    
    if(--iy[ENEMY_GENERATOR_LENGTH])return;// 生成数の更新
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
}
static void EnemyFansFrontMove(void);
static void EnemyFansBackMove(void);
// 敵を更新する
void EnemyFansUpdate(char* ix) __naked {
    ix;
    __asm;
    push ix
    push hl
    pop ix
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // ショットの設定
        call    _SystemGetRandom
        and     #0x1f
        add     #0x08
        ld      ENEMY_SHOT(ix), a
        // アニメーションの設定
        ld      a, #0x60
        ld      ENEMY_ANIMATION(ix), a
        // タイマの設定
        ld      a, #0x04
        ld      ENEMY_TIMER(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // 移動
    ld      a, ENEMY_TYPE(ix)
    cp      #ENEMY_TYPE_FANS_FRONT
    jr      nz, 10$
        call    _EnemyFansFrontMove
        jr      19$
    10$:
        call    _EnemyFansBackMove
    19$:
    // ショットの更新
    dec     ENEMY_SHOT(ix)
    jr      nz, 91$
        ld      h, ENEMY_POSITION_X(ix)
        ld      l, ENEMY_POSITION_Y(ix)
        call    _BulletGenerate
        call    _SystemGetRandom
        and     #0x1f
        add     #0x30
        ld      ENEMY_SHOT(ix), a
        // アニメーションの更新
    91$:
    dec     ENEMY_TIMER(ix)
    jr      nz, 99$
        ld      a, ENEMY_ANIMATION(ix)
        add     a, #0x02
        cp      #0x66
        jr      c, 92$
            ld      a, #0x60
        92$:
        ld      ENEMY_ANIMATION(ix), a
        ld      a, #0x04
        ld      ENEMY_TIMER(ix), a
        jr      99$
        // 更新の完了
    99$:
    pop ix
    ret
    __endasm;
}
// 敵が移動する／→
static void EnemyFansFrontMove(void) __naked {
    __asm;
    // 移動（→）
    ld      a, ENEMY_STATE(ix)
    dec     a
    jr      nz, 19$
        ld      a, ENEMY_POSITION_X(ix)
        add     a, #0x04
        jp      c, 98$
            ld      ENEMY_POSITION_X(ix), a
            cp      #0xd0
            jr      c, 99$
                ld      a, (_ship + SHIP_POSITION_Y)
                cp      ENEMY_POSITION_Y(ix)
                ld      a, #0x02
                adc     a, #0x00
                ld      ENEMY_STATE(ix), a
                jr      99$
    19$:
    // 移動（↓）
    //ld      a, ENEMY_STATE(ix)
    dec     a
    jr      nz, 29$
        ld      a, ENEMY_POSITION_X(ix)
        sub     #0x04
        jr      c, 98$
            ld      ENEMY_POSITION_X(ix), a
            ld      a, ENEMY_POSITION_Y(ix)
            add     a, #0x04
            cp      #0xc0
            jr      nc, 98$
                ld      ENEMY_POSITION_Y(ix), a
                ld      b, a
                ld      a, (_ship + SHIP_POSITION_Y)
                cp      b
                jr      nc, 99$
                    ld      a, #0x04
                    ld      ENEMY_STATE(ix), a
                    jr      99$
    29$:
        // 移動（↑）
        //ld      a, ENEMY_STATE(ix)
        dec     a
        jr      nz, 39$
            ld      a, ENEMY_POSITION_X(ix)
            sub     #0x04
            jr      c, 98$
                ld      ENEMY_POSITION_X(ix), a
                ld      a, ENEMY_POSITION_Y(ix)
                sub     #0x04
                jr      c, 98$
                    ld      ENEMY_POSITION_Y(ix), a
                    ld      b, a
                    ld      a, (_ship + SHIP_POSITION_Y)
                    cp      b
                    jr      c, 99$
                        ld      a, #0x04
                        ld      ENEMY_STATE(ix), a
                        jr      99$
    39$:
        // 移動（←）
        //ld      a, ENEMY_STATE(ix)
        //dec     a
        //jr      nz, 49$
        ld      a, ENEMY_POSITION_X(ix)
        sub     #0x04
        jr      c, 98$
            ld      ENEMY_POSITION_X(ix), a
            jr      99$
    49$:
    // 敵の削除
    98$:
        xor     a
        ld      ENEMY_TYPE(ix), a
        // 移動の完了
    99$:
    ret
    __endasm;
}
// 敵が移動する／←
static void EnemyFansBackMove(void) __naked {
    __asm;
    // 移動（←）
    ld      a, ENEMY_STATE(ix)
    dec     a
    jr      nz, 19$
        ld      a, ENEMY_POSITION_X(ix)
        sub     #0x04
        ld      ENEMY_POSITION_X(ix), a
        cp      #0x30
        jp      nc, 99$
            ld      a, (_ship + SHIP_POSITION_Y)
            cp      ENEMY_POSITION_Y(ix)
            ld      a, #0x02
            adc     a, #0x00
            ld      ENEMY_STATE(ix), a
            jr      99$
    19$:
        // 移動（↓）
        //ld      a, ENEMY_STATE(ix)
        dec     a
        jr      nz, 29$
            ld      a, ENEMY_POSITION_X(ix)
            add     a, #0x04
            jr      c, 98$
                ld      ENEMY_POSITION_X(ix), a
                ld      a, ENEMY_POSITION_Y(ix)
                add     a, #0x04
                cp      #0xc0
                jr      nc, 98$
                    ld      ENEMY_POSITION_Y(ix), a
                    ld      b, a
                    ld      a, (_ship + SHIP_POSITION_Y)
                    cp      b
                    jr      nc, 99$
                        ld      a, #0x04
                        ld      ENEMY_STATE(ix), a
                        jr      99$
    29$:
        // 移動（↑）
        //ld      a, ENEMY_STATE(ix)
        dec     a
        jr      nz, 39$
            ld      a, ENEMY_POSITION_X(ix)
            add     a, #0x04
            jr      c, 98$
                ld      ENEMY_POSITION_X(ix), a
                ld      a, ENEMY_POSITION_Y(ix)
                sub     #0x04
                jr      c, 98$
                    ld      ENEMY_POSITION_Y(ix), a
                    ld      b, a
                    ld      a, (_ship + SHIP_POSITION_Y)
                    cp      b
                    jr      c, 99$
                        ld      a, #0x04
                        ld      ENEMY_STATE(ix), a
                        jr      99$
    39$:
        // 移動（→）
        //ld      a, ENEMY_STATE(ix)
        //dec     a
        //jr      nz, 49$
        ld      a, ENEMY_POSITION_X(ix)
        add     a, #0x04
        jr      c, 98$
            ld      ENEMY_POSITION_X(ix), a
            jr      99$
    49$:
    // 敵の削除
    98$:
        xor     a
        ld      ENEMY_TYPE(ix), a
        // 移動の完了
    99$:
    ret
    __endasm;
}
