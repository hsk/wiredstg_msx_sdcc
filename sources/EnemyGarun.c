// EnemyGarun.c : 敵／ガルン
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
// 定数の定義
// カーブ
static char const enemyGarunCurve[] = {
    0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02,
    0x02, 0x01, 0xff, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd,
    0xfc, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe,
    0xfe, 0xff, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03,
    //0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01,
    //0x00, 0x00, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfd,
    //0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe, 0xff, 0xff,
    //0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
};
// アニメーション
static char const enemyGarunAnimation[] = {
    0x80, 0x82, 0x84, 0x82,
    0xa0, 0xa2, 0xa4, 0xa2,
};
// 敵を生成する
void EnemyGarunGenerate(void) {
    char* iy = enemyGenerator;
    #if 0
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
    return;
    #endif
    // 初期化の開始
    if (iy[ENEMY_GENERATOR_STATE] == 0) {
        iy[ENEMY_GENERATOR_LENGTH] = 4;// 生成数の設定
        iy[ENEMY_GENERATOR_TIMER] = 1;// タイマの設定
        iy[ENEMY_GENERATOR_STATE]++;// 初期化の完了
    }
    if (--iy[ENEMY_GENERATOR_TIMER]) return;// タイマの更新
    iy[ENEMY_GENERATOR_TIMER] = 0x10;
    char* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix[ENEMY_TYPE] = iy[ENEMY_GENERATOR_TYPE];// 敵の生成
    ix[ENEMY_STATE] = 0;
    ix[ENEMY_POSITION_X] = (ix[ENEMY_TYPE]==ENEMY_TYPE_GARUN_BACK) ? 0xff : 0;
    ix[ENEMY_POSITION_Y] = ((SystemGetRandom()&3)<<5)+0x30;
    ix[ENEMY_HP] = 0x01;
    if(--iy[ENEMY_GENERATOR_LENGTH])return;// 生成数の更新
    iy[ENEMY_GENERATOR_TYPE] = 0;
    iy[ENEMY_GENERATOR_STATE] = 0;
}
static void up0(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0){
        ix[ENEMY_PARAM_0] = 0;// パラメータの設定
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x1f)+0x10;// ショットの設定
        ix[ENEMY_TIMER] = 0;// タイマの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
}
// 敵を更新する
void EnemyGarunUpdate(char* ix) {
    up0(ix);
    for(;;) {
        {// 移動（←→）
            if (ix[ENEMY_TYPE] == ENEMY_TYPE_GARUN_FRONT) {
                if (ix[ENEMY_POSITION_X]+2>255) break;
                ix[ENEMY_POSITION_X]+=0x02;
            } else {
                if (ix[ENEMY_POSITION_X]-2 < 0) break;
                ix[ENEMY_POSITION_X]-=0x02;
            }
        }
        {// 移動（↑↓）
        ix[ENEMY_PARAM_0]++;
        char a = enemyGarunCurve[ix[ENEMY_PARAM_0]&0x1f]+ix[ENEMY_POSITION_Y];
        if (a>=0xc0) break;
        ix[ENEMY_POSITION_Y] = a;}
        // ショットの更新
        if (--ix[ENEMY_SHOT] == 0) {
            BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
            ix[ENEMY_SHOT]=(SystemGetRandom()&0x1f)+0x18;
        }
        // アニメーションの更新
        ix[ENEMY_TIMER]++;
        ix[ENEMY_ANIMATION]=enemyGarunAnimation[
            ((ix[ENEMY_TYPE]-ENEMY_TYPE_GARUN_FRONT)<<2)+
            ((ix[ENEMY_TIMER] & 0x0c)>>2)];
        return;
    }
    ix[ENEMY_TYPE] = 0;// 敵の削除
}
