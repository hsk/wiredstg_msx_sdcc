// EnemyDucker.c : 敵／ダッカー
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ground.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 敵を生成する
void EnemyDuckerGenerate(void) {
    char* iy = enemyGenerator;// ジェネレータの取得
    #if 1
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
    return;
    #endif
    do {
        // 初期化の開始
        if (iy[ENEMY_GENERATOR_STATE]==0) {
            if (ground[0x003f]==0) break;// 地面のチェック
            iy[ENEMY_GENERATOR_LENGTH] = 0x01;// 生成数の設定
            iy[ENEMY_GENERATOR_TIMER] = 0x01;// タイマの設定
            iy[ENEMY_GENERATOR_STATE]++;// 初期化の完了
        }
        if (gameScroll!=0) return;// スクロールの監視
        if (--iy[ENEMY_GENERATOR_TIMER]) return;// タイマの更新
        iy[ENEMY_GENERATOR_TIMER] = 0x20;
        // 生成の開始
        char* ix = EnemyGetEmpty();
        if (ix==0) return;
        // 敵の生成
        ix[ENEMY_TYPE] = iy[ENEMY_GENERATOR_TYPE];
        ix[ENEMY_STATE] = 0;
        char a = SystemGetRandom();
        if (a < 0x40) a = 0; else a = 255;
        ix[ENEMY_POSITION_X] = a;
        ix[ENEMY_HP] = 1;
        if(--iy[ENEMY_GENERATOR_LENGTH])return;// 生成数の更新
        // 生成の完了
    } while(0);
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
}
// 敵を更新する
void EnemyDuckerUpdate(char* ix) __naked {
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
        ld      a, #0x04
        ld      ENEMY_PARAM_1(ix), a
        ld      a, ENEMY_TYPE(ix)
        sub     #ENEMY_TYPE_DUCKER_UPPER
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, #0xc0
        ld      ENEMY_PARAM_2(ix), a
        // ショットの設定
        ld      a, #0x10
        ld      ENEMY_SHOT(ix), a
        // アニメーションの設定
        //xor     a
        //ld      ENEMY_ANIMATION(ix), a
        // タイマの設定
        call    _SystemGetRandom
        and     #0x3f
        add     a, #0x20
        ld      ENEMY_TIMER(ix), a
        // 初期化の完了
        ld      a, ENEMY_POSITION_X(ix)
        cp      #0x80
        ld      a, #0x02
        sbc     #0x00
        ld      ENEMY_STATE(ix), a
    09$:
    // do {
        // 移動（→）
        ld      a, ENEMY_STATE(ix)
        dec     a
        jr      nz, 19$
            ld      a, ENEMY_POSITION_X(ix)
            add     a, #0x02
            jp      c, 98$ // break;
                ld      ENEMY_POSITION_X(ix), a
                ld      a, ENEMY_TIMER(ix)
                and     #0x04
                srl     a
                add     a, ENEMY_PARAM_2(ix)
                ld      ENEMY_ANIMATION(ix), a
                dec     ENEMY_TIMER(ix)
                jp      nz, 80$
                    call    _SystemGetRandom
                    and     #0x1f
                    add     a, #0x10
                    ld      ENEMY_TIMER(ix), a
                    ld      a, #0x03
                    ld      ENEMY_STATE(ix), a
                    jp      80$
        19$:
            // 移動（←）
            //ld      a, ENEMY_STATE(ix)
            dec     a
            jr      nz, 29$
            ld      a, ENEMY_POSITION_X(ix)
            sub     #0x02
            jp      c, 98$ // break;
            ld      ENEMY_POSITION_X(ix), a
            ld      a, ENEMY_TIMER(ix)
            and     #0x04
            srl     a
            add     a, #0x06
            add     a, ENEMY_PARAM_2(ix)
            ld      ENEMY_ANIMATION(ix), a
            dec     ENEMY_TIMER(ix)
            jr      nz, 80$
                call    _SystemGetRandom
                and     #0x1f
                add     a, #0x10
                ld      ENEMY_TIMER(ix), a
                ld      a, #0x03
                ld      ENEMY_STATE(ix), a
                jr      80$
        29$:
            // 待機
            //ld      a, ENEMY_STATE(ix)
            //dec     a
            //jr      nz, 39$
            ld      a, (_gameScroll)
            or      a
            jr      nz, 30$
                ld      a, ENEMY_POSITION_X(ix)
                add     a, #0x08
                jp      c, 98$ // break;
                ld      ENEMY_POSITION_X(ix), a
        30$:
            dec     ENEMY_SHOT(ix)
            jr      nz, 31$
                ld      h, ENEMY_POSITION_X(ix)
                ld      l, ENEMY_POSITION_Y(ix)
                call    _BulletGenerate
                call    _SystemGetRandom
                and     #0x0f
                add     #0x10
                ld      ENEMY_SHOT(ix), a
            31$:
                ld      a, (_ship + SHIP_POSITION_X)
                cp      ENEMY_POSITION_X(ix)
                ld      a, #0x04
                jr      nc, 32$
                    ld      a, #0x0a
                32$:
                add     a, ENEMY_PARAM_2(ix)
                ld      ENEMY_ANIMATION(ix), a
                dec     ENEMY_TIMER(ix)
                jr      nz, 80$
                    xor     a
                    dec     ENEMY_PARAM_1(ix)
                    jr      z, 33$
                        call    _SystemGetRandom
                        and     #0x3f
                        add     a, #0x10
                    33$:
                    ld      ENEMY_TIMER(ix), a
                    ld      a, (_ship + SHIP_POSITION_X)
                    cp      ENEMY_POSITION_X(ix)
                    ld      a, #0x01
                    adc     a, #0x00
                    ld      ENEMY_STATE(ix), a
            39$:
            // 高さの取得
        80$:
        ld      a, ENEMY_POSITION_X(ix)
        srl     a
        srl     a
        srl     a
        ld      e, a
        ld      d, #0x00
        ld      a, ENEMY_TYPE(ix)
        cp      #ENEMY_TYPE_DUCKER_UPPER
        jr      nz, 82$
            ld      hl, #(_ground + 0x0020)
            add     hl, de
            ld      e, #0x20
            ld      bc, #0x0810
            81$:
                ld      a, (hl)
                or      a
                jr      z, 89$
                add     hl, de
                ld      a, c
                add     a, b
                ld      c, a
            jr      81$
        82$:
        ld      hl, #(_ground + 0x02e0)
        add     hl, de
        ld      e, #0x20
        ld      bc, #0x08b8
        83$:
            ld      a, (hl)
            or      a
            jr      z, 89$
                sbc     hl, de
                ld      a, c
                sub     b
                ld      c, a
        jr      83$
        89$:
        ld      ENEMY_POSITION_Y(ix), c
        jr 99$
    // 敵の削除
    98$:// }
    xor     a
    ld      ENEMY_TYPE(ix), a
    99$:
    pop ix
    ret
    __endasm;
}
