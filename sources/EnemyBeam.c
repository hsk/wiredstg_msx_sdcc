// EnemyBeam.c : 敵／ビーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Enemy.h"
// 敵を生成する
void EnemyBeamGenerate(char* hl) __naked {
    hl;
    __asm;
    push    ix
    push hl
    // 敵の生成
    call    _EnemyGetEmpty
    pop hl
    ld a,d
    or e
    jr      z, 09$
        push de
        pop ix
        ld      a, #ENEMY_TYPE_BEAM
        ld      ENEMY_TYPE(ix), a
        xor     a
        ld      ENEMY_STATE(ix), a
        ld      a, #0x18
        add     a, h
        ld      ENEMY_POSITION_X(ix), a
        ld      a, #0xe8
        add     a, l
        ld      ENEMY_POSITION_Y(ix), a
        ld      a, #0xff
        ld      ENEMY_HP(ix), a
    09$:
    pop     ix
    ret
    __endasm;
}
// 敵を更新する
void EnemyBeamUpdate(char* ix) __naked {
    ix;
    __asm
    push ix
    push hl
    pop ix
    // 初期化の開始
    ld      a, ENEMY_STATE(ix)
    or      a
    jr      nz, 09$
        // ショットの設定
        xor     a
        ld      ENEMY_SHOT(ix), a
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // ＨＰの再設定
    ld      a, #0xff
    ld      ENEMY_HP(ix), a
    // ショットの更新
    ld      a, ENEMY_SHOT(ix)
    cp      #0x04
    jr      nc, 19$
        inc     ENEMY_SHOT(ix)
        jr      99$
    19$:
    // 移動
    ld      a, ENEMY_POSITION_X(ix)
    add     a, #0x08
    ld      ENEMY_POSITION_X(ix), a
    jr      nc, 99$
        // 敵の削除
        xor     a
        ld      ENEMY_TYPE(ix), a
    99$:
    pop ix
    ret
    __endasm;
}
// 敵を描画する
void EnemyBeamRender(char* ix) __naked {
    ix;
    __asm
    push ix
    push hl
    pop ix
    // 位置の取得
    ld      a, ENEMY_POSITION_Y(ix)
    and     #0xf8
    ld      b, #0x00
    add     a, a
    rl      b
    add     a, a
    rl      b
    ld      c, ENEMY_POSITION_X(ix)
    srl     c
    srl     c
    srl     c
    ld      e, c
    add     a, c
    ld      c, a
    ld      ENEMY_PARAM_0(ix), c
    ld      ENEMY_PARAM_1(ix), b
    // 長さの取得
    ld      a, #0x20
    sub     e
    cp      #0x04
    jr      c, 00$
        ld      a, ENEMY_SHOT(ix)
    00$:
    ld      ENEMY_PARAM_2(ix), a
    ld      a, #0x1d
    sub     e
    jr      nc, 01$
        xor     a
        jr      02$
    01$:
    cp      #0x04
    jr      c, 02$
        ld      a, ENEMY_SHOT(ix)
    02$:
    ld      ENEMY_PARAM_3(ix), a
    // パターンを置く／外側
    ld      hl, #_appPatternName
    add     hl, bc
    ex      de, hl
    ld      hl, #_enemyCollision
    add     hl, bc
    ld      c, ENEMY_INDEX(ix)
    ld      a, #0xb4
    ld      b, ENEMY_PARAM_2(ix)
    10$:
        ld      (de), a
        ld      (hl), c
        inc     de
        inc     hl
    djnz    10$
    ld      a, #0xc0
    sub     ENEMY_PARAM_2(ix)
    ld      c, a
    //ld      b, #0x00
    add     hl, bc
    ex      de, hl
    add     hl, bc
    ex      de, hl
    ld      c, ENEMY_INDEX(ix)
    ld      a, #0xb4
    ld      b, ENEMY_PARAM_2(ix)
    11$:
        ld      (de), a
        ld      (hl), c
        inc     de
        inc     hl
    djnz    11$
    // パターンを置く／内側
    ld      a, ENEMY_PARAM_3(ix)
    or      a
    jr      z, 29$
        ld      c, ENEMY_PARAM_0(ix)
        ld      b, ENEMY_PARAM_1(ix)
        ld      hl, #(_appPatternName + 0x0043)
        add     hl, bc
        ex      de, hl
        ld      hl, #(_enemyCollision + 0x0043)
        add     hl, bc
        ld      c, ENEMY_INDEX(ix)
        ld      a, #0xb4
        ld      b, ENEMY_PARAM_3(ix)
        20$:
            ld      (de), a
            ld      (hl), c
            inc     de
            inc     hl
        djnz    20$
        ld      a, #0x40
        sub     ENEMY_PARAM_3(ix)
        ld      c, a
        //   ld      b, #0x00
        add     hl, bc
        ex      de, hl
        add     hl, bc
        ex      de, hl
        ld      c, ENEMY_INDEX(ix)
        ld      a, #0xb4
        ld      b, ENEMY_PARAM_3(ix)
        21$:
            ld      (de), a
            ld      (hl), c
            inc     de
            inc     hl
        djnz    21$
    29$:
    pop ix
    ret
    __endasm;
}
