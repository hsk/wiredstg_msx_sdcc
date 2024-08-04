// EnemyBomb.c : 敵／爆発
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
// 定数の定義
// ＳＥ
static char const enemyBombSe[] = "T1V15L0O4GFEDCO3BAG";
// 敵を生成する
void EnemyBombGenerate(void) {
}
// 敵を更新する
void EnemyBombUpdate(char* ix) __naked {
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
        ld      a, #0x40
        ld      ENEMY_ANIMATION(ix), a
        // タイマの設定
        ld      a, #0x02
        ld      ENEMY_TIMER(ix), a
        // 撃ち返し
        ld      h, ENEMY_POSITION_X(ix)
        ld      l, ENEMY_POSITION_Y(ix)
        call    _BulletGenerate
        // ＳＥの再生
        ld      hl, #_enemyBombSe
        ld      (_soundRequest + 0x0006), hl
        // 初期化の完了
        inc     ENEMY_STATE(ix)
    09$:
    // アニメーションの更新
    dec     ENEMY_TIMER(ix)
    jr      nz, 99$
    ld      a, ENEMY_ANIMATION(ix)
    add     a, #0x02
    cp      #0x46
    jr      nc, 98$
        ld      ENEMY_ANIMATION(ix), a
        ld      a, #0x02
        ld      ENEMY_TIMER(ix), a
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
// 敵を描画する
void EnemyBombRender(char* ix) {
    // クリッピングの取得
    char c = 0b00001111;
    if (ix[ENEMY_POSITION_Y]<8) c &= ~((1<<0)|(1<<1));
    if (ix[ENEMY_POSITION_Y]>=0xc0) c &= ~((1<<2)|(1<<3));
    if (ix[ENEMY_POSITION_X]<8) c &= ~((1<<0)|(1<<2));
    // パターンを置く
    short de = (ix[ENEMY_POSITION_Y]&0xf8)*4 + (ix[ENEMY_POSITION_X] >> 3);
    char* hl = appPatternName - 0x0021 + de;
    char a = ix[ENEMY_ANIMATION];
    if (c&1) { *hl = a; } c=c>>1;
    hl++;a++;
    if (c&1) { *hl = a; } c=c>>1;
    hl+= 0x1f;a+= 0xf;
    if (c&1) { *hl = a;} c=c>>1;
    hl++;a++;
    if (c&1) { *hl = a;}
}
