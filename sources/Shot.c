// Shot.c : ショット
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "string.h"
// 変数の定義
char shot[SHOT_SIZE * SHOT_N];// ショット
static char shotTimer;// タイマ
// ショットを初期化する
void ShotInitialize(void) {
    memset(shot,0,SHOT_SIZE * SHOT_N); // ショットの初期化
    shotTimer = 0; // タイマの初期化
}
// ショットを生成する
void ShotGenerate(void) {
    // 空きを探す
    for(char *ix = shot, b = #SHOT_N;b;b--,ix+=SHOT_SIZE) {
        if (ix[SHOT_STATE]) continue;
        // ショットの生成
        char a = ship[SHIP_POSITION_X] - 0x0c;
        ix[SHOT_POSITION_X] = a;
        ix[SHOT_RANGE_LEFT] = a;
        ix[SHOT_RANGE_RIGHT] = a + 7;
        a = ship[SHIP_POSITION_Y];
        ix[SHOT_POSITION_Y] = a;
        ix[SHOT_RANGE_TOP] = a;
        ix[SHOT_RANGE_BOTTOM] = a;
        ix[SHOT_STATE]++;
        soundRequest[3] = (void*)shotSe;// ＳＥの再生
        break;
    }
}
// ショットを更新する
void ShotUpdate(void) {
    // ショットの走査
    for(char *ix = shot,b=SHOT_N;b;ix+=SHOT_SIZE,b--) {
        if (ix[SHOT_STATE]==0) continue;
        // 移動
        char a = ix[SHOT_POSITION_X]-8; 
        if (a >= 0xf8) {
            ix[SHOT_STATE] = 0;
            continue;
        }
        ix[SHOT_POSITION_X] = a;
        ix[SHOT_RANGE_LEFT] = a;// 範囲の取得
        ix[SHOT_RANGE_RIGHT] = a + 7;
    }
    shotTimer++;// タイマの更新
}
// ショットを描画する
void ShotRender(void) {
    // ショットの走査
    char c = (shotTimer&3)<<2;
    for(char *ix=shot,b=SHOT_N;b;ix+=SHOT_SIZE,b--) {
        if (ix[SHOT_STATE]==0) continue;
        // スプライトの描画
        char *hl = sprite + GAME_SPRITE_SHOT + c;
        *hl++ = ix[SHOT_POSITION_Y]-1;
        *hl++ = ix[SHOT_POSITION_X];
        *hl++ = 0x38;
        *hl = appColor;
        c = (c + 4)&0xf;
    }
}

