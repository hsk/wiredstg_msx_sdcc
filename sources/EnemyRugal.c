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
    #if 0
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
void EnemyRugalUpdate(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0){
        ix[ENEMY_PARAM_0] = 0;// パラメータの設定
        ix[ENEMY_PARAM_1] = ((ix[ENEMY_TYPE] - ENEMY_TYPE_RUGAL_FRONT)<<5)+0x68;
        // ショットの設定
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x3f)+0x40;
        ix[ENEMY_ANIMATION] = ix[ENEMY_PARAM_1];// アニメーションの設定
        ix[ENEMY_TIMER] = 0x40;// タイマの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    // 移動
    for(;;) {
        if (ix[ENEMY_TYPE] == ENEMY_TYPE_RUGAL_FRONT) {
            if(++ix[ENEMY_POSITION_X]==0) break;
        } else {
            if(ix[ENEMY_POSITION_X]==0) break;
            ix[ENEMY_POSITION_X]--;
        }
        ix[ENEMY_POSITION_Y]+=ix[ENEMY_PARAM_0];
        if(ix[ENEMY_POSITION_Y]>=0xc0) break;
        // 方向転換
        if(--ix[ENEMY_TIMER] == 0) {
            short a = ship[SHIP_POSITION_Y] - ix[ENEMY_POSITION_Y];
            if (a<0) {
                if (a < -8) a = -1; else a = 0;
            } else {
                if (a > 8) a = 1; else a = 0;
            }
            ix[ENEMY_PARAM_0] = a;
            ix[ENEMY_ANIMATION] = a*2 + ix[ENEMY_PARAM_1];
            ix[ENEMY_TIMER] = (SystemGetRandom()&0x3f)+0x10;
        }
        // ショットの更新
        if (--ix[ENEMY_SHOT] == 0) {
            BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
            ix[ENEMY_SHOT] = (SystemGetRandom()&0x3f)+0x40;
        }
        return;
    }
    ix[ENEMY_TYPE] = 0;// 敵の削除
}
