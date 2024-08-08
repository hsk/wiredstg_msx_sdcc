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
    #if 0
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
static void EnemyFansFrontMove(char *);
static void EnemyFansBackMove(char *);

static void up0(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0){
        // ショットの設定
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x1f)+0x08;
        ix[ENEMY_ANIMATION] = 0x60;// アニメーションの設定
        ix[ENEMY_TIMER] = 4;// タイマの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
}
static void up1(char* ix) {
    // ショットの更新
    if (--ix[ENEMY_SHOT] == 0) {
        BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x1f)+0x30;
    }
}
static void up2(char* ix) {
    if (--ix[ENEMY_TIMER] == 0) {
        char a = ix[ENEMY_ANIMATION] + 2;
        if (a >= 0x66) a = 0x60;
        ix[ENEMY_ANIMATION] = a;
        ix[ENEMY_TIMER] = 4;
    }
}
// 敵を更新する
void EnemyFansUpdate(char* ix) {
    up0(ix);
    // 移動
    if (ix[ENEMY_TYPE] == ENEMY_TYPE_FANS_FRONT) EnemyFansFrontMove(ix);
    else                                         EnemyFansBackMove(ix);
    up1(ix);
    up2(ix);
}
static void f1(char* ix) {
    if (ix[ENEMY_POSITION_X]>=256-4) {
        ix[ENEMY_TYPE]=0;
    } else {
        ix[ENEMY_POSITION_X] += 4;
        if (ix[ENEMY_POSITION_X]<0xd0) return;
        ix[ENEMY_STATE] = (ship[SHIP_POSITION_Y]<=ix[ENEMY_POSITION_Y]) ? 3 : 2;
    }
}
static void f2(char* ix) {
    if (ix[ENEMY_POSITION_X]<=4) ix[ENEMY_TYPE]=0;
    else {
        ix[ENEMY_POSITION_X] -= 4;
        ix[ENEMY_POSITION_Y] += 4;
        if (ix[ENEMY_POSITION_Y]>=0xc0) ix[ENEMY_TYPE]=0;
        else if (ship[SHIP_POSITION_Y] < ix[ENEMY_POSITION_Y]) ix[ENEMY_STATE] = 4;
    }
}
static void f3(char* ix) {
    if (ix[ENEMY_POSITION_X] < 4) ix[ENEMY_TYPE]=0;
    else {
        ix[ENEMY_POSITION_X] -= 4;
        if (ix[ENEMY_POSITION_Y] < 4) ix[ENEMY_TYPE]=0;
        else {
            ix[ENEMY_POSITION_Y] -= 4;
            if (ship[SHIP_POSITION_Y] > ix[ENEMY_POSITION_Y])
                ix[ENEMY_STATE] = 4;
        }
    }
}
static void f4(char* ix) {
    if (ix[ENEMY_POSITION_X] < 4) ix[ENEMY_TYPE]=0;
    else ix[ENEMY_POSITION_X] -= 4;
}
// 敵が移動する／→
static void EnemyFansFrontMove(char* ix) {
    char a = ix[ENEMY_STATE];
    if (a == 1)      f1(ix); // 移動（←）
    else if (a == 2) f2(ix); // 移動（↓）
    else if (a == 3) f3(ix); // 移動（↑）
    else             f4(ix); // 移動（→）
}
static void b1(char* ix) {
    ix[ENEMY_POSITION_X]-=4;
    if (ix[ENEMY_POSITION_X] >= 0x30) return;
    ix[ENEMY_STATE] = ship[SHIP_POSITION_Y]>ix[ENEMY_POSITION_Y]?2:3;
}
static void b2(char* ix) {
    if (ix[ENEMY_POSITION_X]>=256-4) ix[ENEMY_TYPE]=0;
    else {
        ix[ENEMY_POSITION_X] += 4;
        ix[ENEMY_POSITION_Y] += 4;
        if (ix[ENEMY_POSITION_Y]>=0xc0) ix[ENEMY_TYPE]=0;
        else if (ship[SHIP_POSITION_Y] < ix[ENEMY_POSITION_Y])
            ix[ENEMY_STATE] = 4;
    }
}
static void b3(char* ix) {
    if (ix[ENEMY_POSITION_X] >= 256-4) ix[ENEMY_TYPE]=0;
    else {
        ix[ENEMY_POSITION_X] += 4;
        if (ix[ENEMY_POSITION_Y] < 4) ix[ENEMY_TYPE]=0;
        else {
            ix[ENEMY_POSITION_Y] -= 4;
            if (ship[SHIP_POSITION_Y] > ix[ENEMY_POSITION_Y])
                ix[ENEMY_STATE] = 4;
        }
    }
}
static void b4(char* ix) {
    if (ix[ENEMY_POSITION_X] >= 256-4) ix[ENEMY_TYPE]=0;
    else ix[ENEMY_POSITION_X] += 4;
}
// 敵が移動する／←
static void EnemyFansBackMove(char* ix) {
    char a = ix[ENEMY_STATE];
    if (a == 1)      b1(ix); // 移動（←）
    else if (a == 2) b2(ix); // 移動（↓）
    else if (a == 3) b3(ix); // 移動（↑）
    else             b4(ix); // 移動（→）
}
