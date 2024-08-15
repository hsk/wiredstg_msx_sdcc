// Ship.c : 自機
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "string.h"
// 定数の定義
// 自機の初期値
typedef union SHORT {
    short v;
    struct {
        char l;
        char h;
    };
} SHORT;
typedef struct SHIP {
    char kind;//0
    char state;//1
    char x;//2
    char y;//3
    char hp;//4
    char speed;//5
    char animation;//6
    char timer;//7
    SHORT shot;//8
    char dummy0;//a
    char dummy1;//b
    char* sprite0;//c
    char* sprite1;//e
} SHIP;
static SHIP const shipDefault = {
    SHIP_TYPE_VICVIPER,
    SHIP_STATE_NULL,
    0xc0, 0x60,
    0x01,
    0x02,
    0x00,
    0x00,
    {0x0000},
    0x00, 0x00,
    &sprite[GAME_SPRITE_SHIP_0],
    &sprite[GAME_SPRITE_SHIP_1]
};
// スプライト
static char const shipSprite[] = {
    0xf7, 0xf4, 0x20, 0x00, 0xf7, 0x04, 0x24, 0x00,
    0xf7, 0xf4, 0x28, 0x00, 0xf7, 0x04, 0x2c, 0x00,
    0xf7, 0xf4, 0x30, 0x00, 0xf7, 0x04, 0x34, 0x00,
    0xf7, 0xf8, 0x10, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x14, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x18, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x1c, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
    0xf7, 0xf8, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00,
};
// 変数の定義
char ship[SHIP_SIZE];// 自機
// 自機を初期化する
void ShipInitialize(void) {
    memcpy(ship,(void*)shipDefault,SHIP_SIZE);// 自機の初期化
}
static void ShipPlay(void);
static void ShipBomb(void);
// 自機を更新する
void ShipUpdate(void) {
    char a = ship[SHIP_TYPE];
    // 種類別の処理
    if (a == SHIP_TYPE_VICVIPER)  ShipPlay();
    else if (a == SHIP_TYPE_BOMB) ShipBomb();
    // スプライトの更新
    short hl = *(short*)&ship[SHIP_SPRITE_0];
    *(short*)&ship[SHIP_SPRITE_0] = *(short*)&ship[SHIP_SPRITE_1];
    *(short*)&ship[SHIP_SPRITE_1] = hl;
} 
// 自機を操作する
static void ShipPlay(void) {
    // スピードアップ
    if (input[INPUT_BUTTON_SHIFT]==1) {
        if(++ship[SHIP_SPEED] == 5) ship[SHIP_SPEED] = 1;
    }
    // ↑↓の移動
    if (input[INPUT_KEY_UP]) {
        ship[SHIP_POSITION_Y] -= ship[SHIP_SPEED];
        if (ship[SHIP_POSITION_Y]<0x10)ship[SHIP_POSITION_Y]=0x10;
        ship[SHIP_ANIMATION] = 0x02;
    } else if (input[INPUT_KEY_DOWN]) {
        ship[SHIP_POSITION_Y] += ship[SHIP_SPEED];
        if (ship[SHIP_POSITION_Y]>0xb7)ship[SHIP_POSITION_Y]=0xb7;
        ship[SHIP_ANIMATION] = 0x01;
    } else {
        ship[SHIP_ANIMATION] = 0x0;
    }
    // ←→の移動
    if (input[INPUT_KEY_LEFT]) {
        ship[SHIP_POSITION_X] -= ship[SHIP_SPEED];
        if (ship[SHIP_POSITION_X]<0xc)ship[SHIP_POSITION_X]=0xc;
    } else if (input[INPUT_KEY_RIGHT]) {
        ship[SHIP_POSITION_X] += ship[SHIP_SPEED];
        if (ship[SHIP_POSITION_X]>0xf6)ship[SHIP_POSITION_X]=0xf6;
    }
    // ショット
    if (input[INPUT_BUTTON_SPACE] == 1) {
        ShotGenerate();
        (*(short*)&ship[SHIP_SHOT_L])++;
    }
}
// 自機が爆発する
static void ShipBomb(void) {
    // 初期化の開始
    if (ship[SHIP_STATE]==0) {
        ship[SHIP_ANIMATION] = 3;// アニメーションの設定
        ship[SHIP_TIMER] = 4;// タイマの設定
        soundRequest[3] = (void*)shipSe;// ＳＥの再生
        ship[SHIP_STATE]++;// 初期化の完了
    }
    // アニメーションの更新
    if (--ship[SHIP_TIMER] == 0) {
        ship[SHIP_TIMER] = 4;
        if (++ship[SHIP_ANIMATION] == 0x0b) {
            // 自機の削除
            ship[SHIP_TYPE] = 0;
            ship[SHIP_STATE] = 0;
        }
    }
}
// 自機を描画する
void ShipRender(void) {
    // スプライトの描画
    if (ship[SHIP_TYPE]==0)return;
    // スプライトの描画
    char *de = (void*)&shipSprite[ship[SHIP_ANIMATION]<<3];
    char *hl = *(char**)&ship[SHIP_SPRITE_0];
    *hl++ = *de++ + ship[SHIP_POSITION_Y];
    *hl++ = *de++ + ship[SHIP_POSITION_X];
    *hl++ = *de++;
    *hl++ = appColor; de++;
    *hl++ = *de++ + ship[SHIP_POSITION_Y];
    *hl++ = *de++ + ship[SHIP_POSITION_X];
    *hl++ = *de++;
    *hl   = appColor;
}
