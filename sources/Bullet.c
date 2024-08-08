// Bullet.c : 弾
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Bullet.h"
// 変数の定義
char bullet[BULLET_SIZE * BULLET_N];// 弾
char bulletN;
static char bulletTimer;// タイマ
// 弾を初期化する
void BulletInitialize(void) __naked {
    __asm;
    // 弾の初期化
    ld      hl, #(_bullet + 0x0000)
    ld      de, #(_bullet + 0x0001)
    ld      bc, #(BULLET_SIZE * BULLET_N - 1)
    xor     a
    ld      (hl), a
    ldir
    ld      a, #0x07
    ld      (_bulletN), a
    // タイマの初期化
    xor     a
    ld      (_bulletTimer), a
    ret
    __endasm;
}
// 弾を生成する
void BulletGenerate(short hl) __naked {
    hl;
    __asm;
    // レジスタの保存
    push    ix
    push    iy
    push hl
    pop ix
    // 自機の存在
    ld      iy, #_ship
    ld      a, SHIP_TYPE(iy)
    cp      #SHIP_TYPE_VICVIPER
    jp      nz, 99$
    // 距離の取得
    ld      a, SHIP_POSITION_X(iy)
    sub     h
    cp      #0x10
    jp      c, 99$
    cp      #0xf0
    jp      nc, 99$
    ld      a, SHIP_POSITION_Y(iy)
    sub     l
    cp      #0x10
    jp      c, 99$
    cp      #0xf0
    jp      nc, 99$
    // 空の取得
    ld      ix, #_bullet
    ld      de, #BULLET_SIZE
    ld      a, (_bulletN)
    ld      b, a
    10$:
        ld      a, BULLET_STATE(ix)
        or      a
        jr      z, 11$
        add     ix, de
    djnz    10$
    jp      99$
    // 生成の開始
    11$:
    xor     a
    ld      BULLET_POSITION_XD(ix), a
    ld      BULLET_POSITION_XI(ix), h
    ld      BULLET_POSITION_YD(ix), a
    ld      BULLET_POSITION_YI(ix), l
    ld      a, SHIP_POSITION_X(iy)
    srl     a
    srl     h
    sub     h
    ld      h, a
    ld      a, SHIP_POSITION_Y(iy)
    srl     a
    srl     l
    sub     l
    ld      l, a
    call    _SystemGetAtan2
    cp      #0x40
    jr      nc, 12$
        call    _SystemGetCos
        ld      BULLET_SPEED_XD(ix), l
        ld      BULLET_SPEED_XI(ix), h
        call    _SystemGetSin
        ld      BULLET_SPEED_YD(ix), l
        ld      BULLET_SPEED_YI(ix), h
        ld      a, #0x01
        ld      BULLET_STATE(ix), a
        jr      18$
    12$:
    cp      #0x80
    jr      nc, 13$
        call    _SystemGetSin
        ld      BULLET_SPEED_YD(ix), l
        ld      BULLET_SPEED_YI(ix), h
        ld      b, a
        ld      a, #0x80
        sub     b
        call    _SystemGetCos
        ld      BULLET_SPEED_XD(ix), l
        ld      BULLET_SPEED_XI(ix), h
        ld      a, #0x02
        ld      BULLET_STATE(ix), a
        jr      18$
    13$:
    cp      #0xc0
    jr      nc, 14$
        sub     #0x80
        call    _SystemGetCos
        ld      BULLET_SPEED_XD(ix), l
        ld      BULLET_SPEED_XI(ix), h
        call    _SystemGetSin
        ld      BULLET_SPEED_YD(ix), l
        ld      BULLET_SPEED_YI(ix), h
        ld      a, #0x03
        ld      BULLET_STATE(ix), a
        jr      18$
    14$:
        ld      b, a
        xor     a
        sub     b
        call    _SystemGetCos
        ld      BULLET_SPEED_XD(ix), l
        ld      BULLET_SPEED_XI(ix), h
        call    _SystemGetSin
        ld      BULLET_SPEED_YD(ix), l
        ld      BULLET_SPEED_YI(ix), h
        ld      a, #0x04
        ld      BULLET_STATE(ix), a
    //   jr      18$
        // 速度の設定
    18$:
    ld      l, BULLET_SPEED_XD(ix)
    ld      h, BULLET_SPEED_XI(ix)
    add     hl, hl
    ld      BULLET_SPEED_XD(ix), l
    ld      BULLET_SPEED_XI(ix), h
    ld      l, BULLET_SPEED_YD(ix)
    ld      h, BULLET_SPEED_YI(ix)
    add     hl, hl
    ld      BULLET_SPEED_YD(ix), l
    ld      BULLET_SPEED_YI(ix), h
    // 生成の完了
    99$:
    // レジスタの復帰
    pop     iy
    pop     ix
    // 終了
    ret
    __endasm;
}
// 弾を更新する
void BulletUpdate(void) __naked {
    __asm;
    // レジスタの保存
    // 弾の走査
    ld      ix, #_bullet
    ld      a, (_bulletN)
    ld      b, a
    10$:// for(;;) {
        // for(;;) {
        ld      a, BULLET_STATE(ix)
        or      a
        jp      z, 19$// continue
        // 弾の移動
        dec     a
        jr      nz, 11$
            ld      l, BULLET_POSITION_XD(ix)
            ld      h, BULLET_POSITION_XI(ix)
            ld      e, BULLET_SPEED_XD(ix)
            ld      d, BULLET_SPEED_XI(ix)
            add     hl, de
            jp      c, 18$// break
            ld      BULLET_POSITION_XD(ix), l
            ld      BULLET_POSITION_XI(ix), h
            ld      l, BULLET_POSITION_YD(ix)
            ld      h, BULLET_POSITION_YI(ix)
            ld      e, BULLET_SPEED_YD(ix)
            ld      d, BULLET_SPEED_YI(ix)
            add     hl, de
            jp      c, 18$// break
            ld      BULLET_POSITION_YD(ix), l
            ld      BULLET_POSITION_YI(ix), h
            jp      19$// continue
        11$:
        dec     a
        jr      nz, 12$
            ld      l, BULLET_POSITION_XD(ix)
            ld      h, BULLET_POSITION_XI(ix)
            ld      e, BULLET_SPEED_XD(ix)
            ld      d, BULLET_SPEED_XI(ix)
            or      a
            sbc     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_XD(ix), l
            ld      BULLET_POSITION_XI(ix), h
            ld      l, BULLET_POSITION_YD(ix)
            ld      h, BULLET_POSITION_YI(ix)
            ld      e, BULLET_SPEED_YD(ix)
            ld      d, BULLET_SPEED_YI(ix)
            add     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_YD(ix), l
            ld      BULLET_POSITION_YI(ix), h
            jr      19$// continue
        12$:
        dec     a
        jr      nz, 13$
            ld      l, BULLET_POSITION_XD(ix)
            ld      h, BULLET_POSITION_XI(ix)
            ld      e, BULLET_SPEED_XD(ix)
            ld      d, BULLET_SPEED_XI(ix)
            or      a
            sbc     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_XD(ix), l
            ld      BULLET_POSITION_XI(ix), h
            ld      l, BULLET_POSITION_YD(ix)
            ld      h, BULLET_POSITION_YI(ix)
            ld      e, BULLET_SPEED_YD(ix)
            ld      d, BULLET_SPEED_YI(ix)
            or      a
            sbc     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_YD(ix), l
            ld      BULLET_POSITION_YI(ix), h
            jr      19$// continue
        13$:
            ld      l, BULLET_POSITION_XD(ix)
            ld      h, BULLET_POSITION_XI(ix)
            ld      e, BULLET_SPEED_XD(ix)
            ld      d, BULLET_SPEED_XI(ix)
            add     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_XD(ix), l
            ld      BULLET_POSITION_XI(ix), h
            ld      l, BULLET_POSITION_YD(ix)
            ld      h, BULLET_POSITION_YI(ix)
            ld      e, BULLET_SPEED_YD(ix)
            ld      d, BULLET_SPEED_YI(ix)
            or      a
            sbc     hl, de
            jr      c, 18$// break
            ld      BULLET_POSITION_YD(ix), l
            ld      BULLET_POSITION_YI(ix), h
            jr      19$// continue
        // 弾の削除
        18$: // }
            xor     a
            ld      BULLET_STATE(ix), a
            // 次の弾へ
        19$:
            ld      de, #BULLET_SIZE
            add     ix, de
            dec     b
    jp      nz, 10$ // }
    // タイマの更新
    ld      hl, #_bulletTimer
    inc     (hl)
    // レジスタの復帰
    // 終了
    ret
    __endasm;
}
// 弾を描画する
void BulletRender(void) __naked {
    __asm;    
    // スプライトの描画
    ld      ix, #_bullet
    ld      hl, #_sprite + GAME_SPRITE_BULLET
    ld      a, (_bulletTimer)
    and     #0x0f
    add     a, a
    add     a, a
    ld      c, a
    ld      a, (_bulletN)
    ld      b, a
    10$:
        ld      a, BULLET_STATE(ix)
        or      a
        jr      z, 19$
            ld      hl, #(_sprite + GAME_SPRITE_BULLET)
            ld      e, c
            ld      d, #0x00
            add     hl, de
            ld      a, #0x04
            add     a, c
            and     #0x3f
            ld      c, a
            ld      a, BULLET_POSITION_YI(ix)
            sub     #0x09
            ld      (hl), a
            inc     hl
            ld      a, BULLET_POSITION_XI(ix)
            sub     #0x08
            ld      (hl), a
            inc     hl
            ld      a, #0x3c
            ld      (hl), a
            inc     hl
            ld      a, (_appColor)
            ld      (hl), a
            inc     hl
        19$:
        ld      de, #BULLET_SIZE
        add     ix, de
    djnz    10$
    ret
    __endasm;
}
