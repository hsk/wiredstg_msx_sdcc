// EnemyDee01.c : 敵／ディー０１
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
void EnemyDee01Generate(void) {
    char* iy = enemyGenerator;
    do {
        #if 0
        iy[ENEMY_GENERATOR_TYPE] = 0;
        iy[ENEMY_GENERATOR_STATE] = 0;
        return;
        #endif
        // 初期化の開始
        if (iy[ENEMY_GENERATOR_STATE] == 0) {
            // 地面のチェック
            if (iy[ENEMY_GENERATOR_TYPE]==ENEMY_TYPE_DEE01_UPPER){
                if (groundGenerator[GROUND_GENERATOR_UPPER_STATE]!=GROUND_GENERATOR_STATE_FLAT) break;
                if (groundGenerator[GROUND_GENERATOR_UPPER_LENGTH]<8) break;
            } else {
                if (groundGenerator[GROUND_GENERATOR_LOWER_STATE]!=GROUND_GENERATOR_STATE_FLAT) break;
                if (groundGenerator[GROUND_GENERATOR_LOWER_LENGTH]<8) break;
            }
            // 生成数の設定
            char a = (SystemGetRandom()&3);
            if(a==0)a++;
            iy[ENEMY_GENERATOR_LENGTH] = a;// 生成数の設定
            iy[ENEMY_GENERATOR_TIMER] = 2;// タイマの設定
            iy[ENEMY_GENERATOR_STATE]++;// 初期化の完了
        }
        if (gameScroll!=0) return;// スクロールの監視
        if (--iy[ENEMY_GENERATOR_TIMER]) return;// タイマの更新
        iy[ENEMY_GENERATOR_TIMER] = 0x2;
        // 生成の開始
        char* ix = EnemyGetEmpty();
        if (ix==0) return;
        // 敵の生成
        ix[ENEMY_TYPE] = iy[ENEMY_GENERATOR_TYPE];
        ix[ENEMY_STATE] = 0;
        ix[ENEMY_POSITION_X] = 0;
        ix[ENEMY_POSITION_Y] = (ix[ENEMY_TYPE] == ENEMY_TYPE_DEE01_UPPER) ? 0x18 : 0xb0;
        ix[ENEMY_HP] = 1;
        if(--iy[ENEMY_GENERATOR_LENGTH])return;// 生成数の更新
    } while(0);
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
}
// 敵を更新する
void EnemyDee01Update(char* ix) __naked {
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
        and     #0x3f
        add     #0x40
        ld      ENEMY_SHOT(ix), a
        // アニメーションの設定
        //   xor     a
        //   ld      ENEMY_ANIMATION(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // 移動（→）
    ld      a, (_gameScroll)
    or      a
    jr      nz, 19$
        ld      a, ENEMY_POSITION_X(ix)
        add     a, #0x08
        jp      c, 98$
        ld      ENEMY_POSITION_X(ix), a
    19$:
    // 向きの取得
    ld      a, ENEMY_TYPE(ix)
    cp      #ENEMY_TYPE_DEE01_UPPER
    jr      nz, 22$
        ld      h, ENEMY_POSITION_X(ix)
        ld      l, ENEMY_POSITION_Y(ix)
        ld      a, (_ship + SHIP_POSITION_X)
        srl     a
        srl     h
        sub     h
        ld      h, a
        ld      a, #0x40
        jr      c, 20$
            ld      a, (_ship + SHIP_POSITION_Y)
            srl     a
            srl     l
            sub     l
            ld      l, a
            ld      a, #0x00
            jr      c, 20$
                call    _SystemGetAtan2
        20$:
        ld      c, #0x00
        cp      #0x10
        jr      c, 21$
            inc     c
            cp      #0x30
            jr      c, 21$
                inc     c
        21$:
        ld      a, c
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, #0xac
        ld      ENEMY_ANIMATION(ix), a
        jr      29$
    22$:
        ld      h, ENEMY_POSITION_X(ix)
        ld      l, ENEMY_POSITION_Y(ix)
        ld      a, (_ship + SHIP_POSITION_X)
        srl     a
        srl     h
        sub     h
        ld      h, a
        ld      a, #0xc0
        jr      c, 23$
            ld      a, (_ship + SHIP_POSITION_Y)
            srl     a
            srl     l
            sub     l
            ld      l, a
            ld      a, #0x00
            jr      nc, 23$
                call    _SystemGetAtan2
    23$:
        or      a
        jr      nz, 24$
            dec     a
        24$:
        ld      c, #0x00
        or      a
        jr      z, 25$
        cp      #0xf0
        jr      nc, 25$
        inc     c
        cp      #0xd0
        jr      nc, 25$
            inc     c
        25$:
        ld      a, c
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, a
        add     a, #0xae
        ld      ENEMY_ANIMATION(ix), a
        //jr      29$
    29$:
    // ショットの更新
    ld      a, (_ship + SHIP_POSITION_X)
    cp      ENEMY_POSITION_X(ix)
    jr      c, 99$
        dec     ENEMY_SHOT(ix)
        jr      nz, 99$
            ld      h, ENEMY_POSITION_X(ix)
            ld      l, ENEMY_POSITION_Y(ix)
            call    _BulletGenerate
            call    _SystemGetRandom
            and     #0x3f
            add     #0x30
            ld      ENEMY_SHOT(ix), a
            jr      99$
    98$:
        // 敵の削除
        xor     a
        ld      ENEMY_TYPE(ix), a
    99$:
    pop ix
    ret
    __endasm;
}
