// EnemyBigCore.c : 敵／ビッグコア
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
// 定数の定義
// パターンネーム
static char const enemyBigCoreCorePatternName[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0xb2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0xb2, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0xb2, 0xb2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xb0, 0xb1, 0x00, 0xb2, 0xb2, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char const enemyBigCoreBodyPatternName[] = {
    0x00, 0x00, 0x60, 0x00, 0x00, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x65, 0x66, 0x67, 0x68, 0xa0, 0x69, 0x6a, 0x6b, 0x00, 0x00, 0x00,
    0x6c, 0x6d, 0x6e, 0x6f, 0xa0, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x00,
    0x76, 0x77, 0x78, 0xa0, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7e, 0x7f,
    0xa1, 0xa2, 0xa3, 0xa0, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x86, 0x87, 0x88, 0xa0, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8e, 0x8f,
    0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x00,
    0x94, 0x95, 0x96, 0x97, 0x98, 0xa0, 0x99, 0x9a, 0x9b, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x90, 0x00, 0x00, 0x91, 0x92, 0x93, 0x00, 0x00, 0x00, 0x00,
};
// ＳＥ
static char const enemyBigCoreSeHit[] = "T1V15-L0O7A";
static char const enemyBigCoreSeBomb[] =
    "T1V15L0"
    "O3GO2D-O3EO2D-O3CO2D-O2GD-ED-"
    "O2CO1D-O2D-O1CO2CO1D-O2D-O1CO2CO1D-O2D-O1CO2CO1D-O2D-O1C";
// 敵を生成する
void EnemyBigCoreGenerate(void) {
    // 敵の生成／コア
    char* ix = EnemyGetEmpty();
    ix[ENEMY_TYPE] = ENEMY_TYPE_BIGCORE_CORE;
    ix[ENEMY_STATE] = 0;
    ix[ENEMY_POSITION_X] = 0xa0;
    ix[ENEMY_POSITION_Y] = 0xe0;
    ix[ENEMY_HP] = 0x0e;
    ix[ENEMY_PARAM_0] =0x0e;
    // 敵の生成／ボディ
    char* body = EnemyGetEmpty();
    *(char**)&body[ENEMY_PARAM_0]=ix;
    body[ENEMY_TYPE] = ENEMY_TYPE_BIGCORE_BODY;
    body[ENEMY_STATE] = 0;
    body[ENEMY_HP] = 0xff;
    enemyGenerator[ENEMY_GENERATOR_TYPE] = 0;// 生成の完了
    enemyGenerator[ENEMY_GENERATOR_STATE] = 0;
}
// 敵を更新する／コア
void EnemyBigCoreUpdateCore(char* ix) __naked {
    ix;
    __asm;
    push ix
    push hl
    pop ix
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
        // 登場
        ld      a, ENEMY_STATE(ix)
        dec     a
        jr      nz, 19$
        ld      a, ENEMY_POSITION_Y(ix)
        cp      #0x61
        jr      c, 10$
        sub     #0x04
        ld      ENEMY_POSITION_Y(ix), a
        jp      nc, 90$
    10$:
        ld      a, ENEMY_POSITION_X(ix)
        cp      #0x39
        jr      c, 11$
        sub     #0x04
        ld      ENEMY_POSITION_X(ix), a
        jp      nc, 90$
    11$:
        ld      a, #0x02
        ld      ENEMY_STATE(ix), a
        jp      90$
    19$:
        // ビームを撃つ
        dec     a
        jr      nz, 29$
        ld      h, ENEMY_POSITION_X(ix)
        ld      l, ENEMY_POSITION_Y(ix)
        call    _EnemyBeamGenerate
        ld      a, #0x20
        ld      ENEMY_TIMER(ix), a
        ld      a, #0x03
        ld      ENEMY_STATE(ix), a
        jp      90$
    29$:
        // 待機
        dec     a
        jr      nz, 39$
        dec     ENEMY_TIMER(ix)
        jr      nz, 90$
        call    _SystemGetRandom
        and     #0x18
        add     a, #0x18
        ld      c, a
        ld      a, (_ship + SHIP_POSITION_Y)
        ld      b, a
        ld      a, ENEMY_POSITION_Y(ix)
        cp      #0x88
        jr      nc, 30$
        cp      #0x38
        jr      c, 32$
        cp      b
        jr      c, 32$
        jr      nz, 30$
        cp      #0x64
        jr      c, 32$
    //   jr      30$
    30$:
        sub     c
        cp      #0x28
        jr      nc, 31$
        ld      a, #0x28
    31$:
        ld      ENEMY_TIMER(ix), a
        ld      a, #0x04
        ld      ENEMY_STATE(ix), a
        jr      90$
    32$:
        add     a, c
        cp      #0x98
        jr      c, 33$
        ld      a, #0x98
    33$:
        ld      ENEMY_TIMER(ix), a
        ld      a, #0x05
        ld      ENEMY_STATE(ix), a
        jr      90$
    39$:
        // 移動↑
        dec     a
        jr      nz, 49$
        ld      a, ENEMY_POSITION_Y(ix)
        sub     #0x04
        ld      ENEMY_POSITION_Y(ix), a
        cp      ENEMY_TIMER(ix)
        jr      nc, 90$
        ld      a, ENEMY_TIMER(ix)
        ld      ENEMY_POSITION_Y(ix), a
        ld      a, #0x02
        ld      ENEMY_STATE(ix), a
        jr      90$
    49$:
        // 移動↓
    //   dec     a
    //   jr      nz, 59$
        ld      a, ENEMY_POSITION_Y(ix)
        add     a, #0x04
        ld      ENEMY_POSITION_Y(ix), a
        cp      ENEMY_TIMER(ix)
        jr      c, 90$
        ld      a, ENEMY_TIMER(ix)
        ld      ENEMY_POSITION_Y(ix), a
        ld      a, #0x02
        ld      ENEMY_STATE(ix), a
    //   jr      90$
    59$:
        // ＨＰの監視
    90$:
        ld      a, ENEMY_HP(ix)
        cp      ENEMY_PARAM_0(ix)
        jr      z, 91$
        ld      ENEMY_PARAM_0(ix), a
        // ＳＥの再生
        ld      hl, #_enemyBigCoreSeHit
        ld      (_soundRequest + 0x0006), hl
    91$:
    pop ix
    ret
    __endasm;
}
// 敵を更新する／ボディ
void EnemyBigCoreUpdateBody(char* ix) __naked {
    ix;
    __asm;
    push ix
    push hl
    pop ix
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // アニメーションの設定
        ld      a, #0x0c
        ld      ENEMY_ANIMATION(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
        // ＨＰの再設定
        ld      a, #0xff
        ld      ENEMY_HP(ix), a
        // コアの取得
        ld      l, ENEMY_PARAM_0(ix)
        ld      h, ENEMY_PARAM_1(ix)
        push    hl
        pop     iy
        // コアの監視
        ld      a, ENEMY_TYPE(iy)
        cp      #ENEMY_TYPE_BIGCORE_CORE
        jr      nz, 19$
        ld      a, ENEMY_POSITION_X(iy)
        ld      ENEMY_POSITION_X(ix), a
        ld      a, ENEMY_POSITION_Y(iy)
        ld      ENEMY_POSITION_Y(ix), a
        jr      99$
    19$:
        // アニメーションの監視
        ld      a, ENEMY_ANIMATION(ix)
        cp      #0x0c
        jr      nz, 20$
        // ＳＥの再生
        ld      hl, #_enemyBigCoreSeBomb
        ld      (_soundRequest + 0x0006), hl
    20$:
        // アニメーションの更新
        dec     ENEMY_ANIMATION(ix)
    jr      nz, 99$
        // 敵の削除
        xor     a
        ld      ENEMY_TYPE(ix), a
        // 更新の完了
    99$:
    pop ix
    ret
    __endasm;
}
short get_core_pos(char*ix){
    return (((short)(ix[ENEMY_POSITION_Y]&0xf8))<<2) +
               (ix[ENEMY_POSITION_X]>>3) - 6;
}
// 敵を描画する／コア
void EnemyBigCoreRender(char* ix) {
    // 位置の取得
    short bc = get_core_pos(ix);
    // パターンを置く
    if(ix[ENEMY_POSITION_Y]>=0xc0) return;
    char* de = appPatternName + bc;
    char* iy = enemyCollision + bc;
    char* hl = enemyBigCoreCorePatternName+(((ix[ENEMY_HP] + 1)<<3)&0xf0);
    for (char b=0x0c;b;de++,iy++,--b){
        char a = *hl++;
        if (a) {
            *de = a;
            *iy = ix[ENEMY_INDEX];
        }
    }
}
// 敵を描画する／ボディ
short get_body_pos(char*ix){
    return (((short)((ix[ENEMY_POSITION_Y]-0x20)&0xf8))<<2) +
               (ix[ENEMY_POSITION_X]>>3) - 6;
}
void EnemyBigCoreBodyRender(char* ix) {
    // パターンを置く
    signed char b1 = 0x1b - (ix[ENEMY_POSITION_Y]>>3);
    if (b1<0) return;
    b1++;
    if (b1 > 9) b1 = 9;
    // 位置の取得
    short bc = get_body_pos(ix);
    {
        char* de = appPatternName + bc;
        char* iy = enemyCollision + bc;
        char* hl = enemyBigCoreBodyPatternName;
        for(;b1;b1--){
            for(char b = ix[ENEMY_ANIMATION];b;de++,iy++,--b) {
                char a = *hl++;
                if (a) {
                    *de = a;
                    *iy = ix[ENEMY_INDEX];
                }
            }
            char a = 12 - ix[ENEMY_ANIMATION];
            hl += a;
            a += (32-12);
            de += a;
            iy += a;
        }
    }
}
