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
void EnemyBombUpdate(char* ix) {
    // 初期化の開始
    if(ix[ENEMY_STATE]==0){
        ix[ENEMY_ANIMATION] = 0x40;// アニメーションの設定
        ix[ENEMY_TIMER] = 2;// タイマの設定
        BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);// 撃ち返し
        soundRequest[3] = enemyBombSe;// ＳＥの再生
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    // アニメーションの更新
    if(--ix[ENEMY_TIMER]) return;
    char a = ix[ENEMY_ANIMATION]+2;
    if (a < 0x46) {
        ix[ENEMY_ANIMATION] = a;
        ix[ENEMY_TIMER] = 2;
        return;
    }
    ix[ENEMY_TYPE] = 0;// 敵の削除
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
