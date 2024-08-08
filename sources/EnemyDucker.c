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
    #if 0
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
static void up1(char a,char* ix) {
    ix[ENEMY_POSITION_X] = a;
    ix[ENEMY_ANIMATION] = ((ix[ENEMY_TIMER] & 0x04)>>1) + ix[ENEMY_PARAM_2];
    if (--ix[ENEMY_TIMER] == 0) {
        ix[ENEMY_TIMER] = (SystemGetRandom() & 0x1f) + 0x10;
        ix[ENEMY_STATE] = 3;
    }
}

static void up2(char a,char* ix) {
    ix[ENEMY_POSITION_X] = a;
    ix[ENEMY_ANIMATION] = ((ix[ENEMY_TIMER] & 0x04)>>1) + 6 + ix[ENEMY_PARAM_2];
    if (--ix[ENEMY_TIMER] == 0) {
        ix[ENEMY_TIMER] = (SystemGetRandom() & 0x1f) + 0x10;
        ix[ENEMY_STATE] = 3;
    }
}

static void up3(char* ix) {
    if (--ix[ENEMY_SHOT] == 0) {
        BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x0f)+0x10;
    }
    char a = (ship[SHIP_POSITION_X] > ix[ENEMY_POSITION_X]) ? 4 : 10;
    ix[ENEMY_ANIMATION] = a + ix[ENEMY_PARAM_2];
    if (--ix[ENEMY_TIMER] == 0) {
        ix[ENEMY_TIMER] = (--ix[ENEMY_PARAM_1]) ? (SystemGetRandom()&0x3f)+0x10 : 0;
        ix[ENEMY_STATE] = (ship[SHIP_POSITION_X] < ix[ENEMY_POSITION_X]) ? 2 : 1;
    }
}

static void up4(char* ix) {
    char c;
    if (ix[ENEMY_TYPE] == ENEMY_TYPE_DUCKER_UPPER) {
        char* hl = ground + 0x0020 + (ix[ENEMY_POSITION_X] >> 3);
        for(c = 0x10;*hl;hl += 0x20) c += 8;
    } else {
        char *hl = ground + 0x002e0 + (ix[ENEMY_POSITION_X] >> 3);
        for(c = 0xb8;*hl; hl-=0x20) c -= 8;
    }
    ix[ENEMY_POSITION_Y] = c;
}

// 敵を更新する
void EnemyDuckerUpdate(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0){
        ix[ENEMY_PARAM_0] = 0;// パラメータの設定
        ix[ENEMY_PARAM_1] = 4;
        ix[ENEMY_PARAM_2] = ((ix[ENEMY_TYPE]-ENEMY_TYPE_DUCKER_UPPER)<<5)+0xc0;
        ix[ENEMY_SHOT] = 0x10;// ショットの設定
        ix[ENEMY_TIMER] = (SystemGetRandom()&0x3f)+0x20;// タイマの設定
        ix[ENEMY_STATE] = (ix[ENEMY_POSITION_X]>0x80) ? 2 : 3;// 初期化の完了
    }
    do {
        char a = ix[ENEMY_STATE];
        if (a==1) {// 移動（→）
            if (ix[ENEMY_POSITION_X]>=254) break;
            up1(ix[ENEMY_POSITION_X]+2,ix);
        } else if (a==2) { // 移動（←）
            if (ix[ENEMY_POSITION_X] < 2) break;
            up2(ix[ENEMY_POSITION_X]-2,ix);
        } else { // 待機
            if (gameScroll==0) {
                if (ix[ENEMY_POSITION_X]>=256-8) break;
                ix[ENEMY_POSITION_X]+=8;
            }
            up3(ix);
        }
        up4(ix);// 高さの取得
        return;
    } while(0);
    ix[ENEMY_TYPE] = 0;
}
