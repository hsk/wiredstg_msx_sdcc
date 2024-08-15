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
static void up1(char* ix) {
    char a;
    char h = (ship[SHIP_POSITION_X]>>1)-(ix[ENEMY_POSITION_X]>>1);
    if (!(h&0x80)) {
        char l = (ship[SHIP_POSITION_Y]>>1)-(ix[ENEMY_POSITION_Y]>>1);
        if(!(l&0x80)) {
            a = SystemGetAtan2((h<<8)|l);
        } else {
            a = 0;
        }
    } else {
        a = 0x40;
    }

    char c = 0;
    if (a >= 0x10) {
        c++;
        if (a >= 0x30) c++;
    }
    ix[ENEMY_ANIMATION] = (c<<5) + 0xac;
}
static void up2(char* ix) {
    char a;
    char h = (ship[SHIP_POSITION_X]>>1)-(ix[ENEMY_POSITION_X]>>1);
    if (!(h&0x80)) {
        char l = (ship[SHIP_POSITION_Y]>>1)-(ix[ENEMY_POSITION_Y]>>1);
        if(l&0x80) {
            a = SystemGetAtan2((h<<8)|l);
        } else {
            a = 0;
        }
    } else {
        a = 0xc0;
    }
    if (a) a--;
    char c = 0;
    if (a) {
        if (a < 0xf0) {
            c++;
            if (a <0xd0) c++;
        }
    }
    ix[ENEMY_ANIMATION] = (c<<5) + 0xae;
}
static void up4(char* ix) {
    if (ship[SHIP_POSITION_X] >= ix[ENEMY_POSITION_X]) {
        if (--ix[ENEMY_SHOT]) return;
        BulletGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
        ix[ENEMY_SHOT]=(SystemGetRandom()&0x3f)+0x30;
    }
}

// 敵を更新する
void EnemyDee01Update(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0) {
        ix[ENEMY_PARAM_0] = 0;// パラメータの設定
        ix[ENEMY_SHOT] = (SystemGetRandom()&0x3f)+0x40;// ショットの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    // 移動（→）
    if (gameScroll==0) {
        if (ix[ENEMY_POSITION_X]>=256-8) {
            ix[ENEMY_TYPE] = 0;// 敵の削除
            return;
        }
        ix[ENEMY_POSITION_X]+=8;
    }
    // 向きの取得
    if (ix[ENEMY_TYPE]==ENEMY_TYPE_DEE01_UPPER) {
        ix[ENEMY_ANIMATION]=0xac;
        up1(ix);
    } else {
        ix[ENEMY_ANIMATION]=0xae;
        up2(ix);
    }
    // ショットの更新
    up4(ix);
}
