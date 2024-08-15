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

static void up1(char* ix) {
    if (ix[ENEMY_POSITION_Y]>=0x61) ix[ENEMY_POSITION_Y] -= 4;
    else if (ix[ENEMY_POSITION_X]>=0x39) ix[ENEMY_POSITION_X] -= 4;
    else ix[ENEMY_STATE] = 2;
}
static void up2(char* ix) {
    EnemyBeamGenerate((ix[ENEMY_POSITION_X]<<8)|ix[ENEMY_POSITION_Y]);
    ix[ENEMY_TIMER] = 0x20;
    ix[ENEMY_STATE] = 3;
}
static void up3(char*ix) {
    if (--ix[ENEMY_TIMER] == 0) {
        char c = (SystemGetRandom()&0x18) + 0x18;
        char sy = ship[SHIP_POSITION_Y];
        char ey = ix[ENEMY_POSITION_Y];
        if (ey > 0x88 || ey >= 0x38 && (ey >= sy && (ey==sy || ey >= 0x64))) {
            ey -= c;
            if (ey < 0x28) ey = 0x28;
            ix[ENEMY_TIMER] = ey;
            ix[ENEMY_STATE] = 4;
        } else {
            ey += c;
            if (ey > 0x98) ey = 0x98;
            ix[ENEMY_TIMER] = ey;
            ix[ENEMY_STATE] = 5;
        }
    }
}
static void up4(char*ix) {
    ix[ENEMY_POSITION_Y] -= 4;
    if (ix[ENEMY_POSITION_Y] < ix[ENEMY_TIMER]){
        ix[ENEMY_POSITION_Y] = ix[ENEMY_TIMER];
        ix[ENEMY_STATE] = 2;
    }
}
static void up5(char*ix) {
    ix[ENEMY_POSITION_Y] += 4;
    if (ix[ENEMY_POSITION_Y]>=ix[ENEMY_TIMER]){
        ix[ENEMY_POSITION_Y] = ix[ENEMY_TIMER];
        ix[ENEMY_STATE] = 2;
    }
}
// 敵を更新する／コア
void EnemyBigCoreUpdateCore(char* ix) {
    if (ix[ENEMY_STATE]==0){
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    char a = ix[ENEMY_STATE];
    if     (--a == 0) up1(ix);// 登場
    else if(--a == 0) up2(ix);// ビームを撃つ
    else if(--a == 0) up3(ix);// 待機
    else if(--a == 0) up4(ix);// 移動↑
    else              up5(ix);// 移動↓

    if (ix[ENEMY_HP] != ix[ENEMY_PARAM_0]) {// ＨＰの監視
        ix[ENEMY_PARAM_0] = ix[ENEMY_HP];
        soundRequest[3] = (void*)enemyBigCoreSeHit;// ＳＥの再生
    }
}
// 敵を更新する／ボディ
void EnemyBigCoreUpdateBody(char* ix) {
    if (ix[ENEMY_STATE]==0) {// 初期化の開始
        ix[ENEMY_ANIMATION] = 0x0c;// アニメーションの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    ix[ENEMY_HP] = 0xff; // ＨＰの再設定
    //char* iy = (char*)((ix[ENEMY_PARAM_1]<<8)|ix[ENEMY_PARAM_0]);// コアの取得
    char* iy = *(char**)&ix[ENEMY_PARAM_0];// コアの取得
    if (iy[ENEMY_TYPE]==ENEMY_TYPE_BIGCORE_CORE) {// コアの監視
        ix[ENEMY_POSITION_X] = iy[ENEMY_POSITION_X];
        ix[ENEMY_POSITION_Y] = iy[ENEMY_POSITION_Y];
    } else {
        if (ix[ENEMY_ANIMATION] == 0x0c)// アニメーションの監視
            soundRequest[3] = (void*)enemyBigCoreSeBomb;// ＳＥの再生
        if (--ix[ENEMY_ANIMATION] == 0)// アニメーションの更新
            ix[ENEMY_TYPE] = 0;// 敵の削除
    }
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
    char* hl = (void*)(enemyBigCoreCorePatternName+(((ix[ENEMY_HP] + 1)<<3)&0xf0));
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
    if (++b1 > 9) b1 = 9;
    // 位置の取得
    short bc = get_body_pos(ix);
    {
        char* de = appPatternName + bc;
        char* iy = enemyCollision + bc;
        char* hl = (void*)enemyBigCoreBodyPatternName;
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
