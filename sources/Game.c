// Game.c : ゲーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ground.h"
#include "Star.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
#include "string.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
#define GAME_STATE_OVER 0x20
// 定数の定義
// ゲームオーバー
static char const gameOverString[] = {
    0x27, 0x21, 0x2d, 0x25, 0x00, 0x00, 0x2f, 0x36, 0x25, 0x32,
};
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
static char gameScorePlus;
char gameScroll;// スクロール
// ゲームを初期化する
void GameInitialize(void) {
    // ゲームの初期化
    GroundInitialize(); // 地面の初期化
    StarInitialize(); // 星の初期化
    ShipInitialize(); // 自機の初期化
    ShotInitialize(); // ショットの初期化
    EnemyInitialize(); // 敵の初期化
    BulletInitialize(); // 弾の初期化
    gamePause = 0;// 一時停止の初期化
    gameScroll = 0;// スクロールの初期化
    memset(appPatternName,0,0x300);// パターンのクリア
    // パターンネームの転送
    AppTransferPatternName();
    // videoRegister[VDP_R1] |= 1<<VDP_R1_BL;// 描画の開始
    // request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
    // 状態の設定
    gameState = GAME_STATE_PLAY;
    appState = APP_STATE_GAME_UPDATE;
}
static void GamePlay(void);
static void GameOver(void);
static void GameHitCheck(void);
// ゲームを更新する
void GameUpdate(void) {
    // ESC キーで一時停止
    if (input[INPUT_BUTTON_ESC] == 1) {
        gamePause = 1 - gamePause;
    }
    if (gamePause) return;
    SystemGetRandom();// 乱数の更新
    char a = gameState & 0xf0;// 状態別の処理
    if (a == GAME_STATE_PLAY) GamePlay();
    else GameOver();
}
// ゲームをプレイする
static void GamePlay(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        videoRegister[VDP_R1] |= 1<<VDP_R1_BL;// 描画の開始
        request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
        gameState++;// 初期化の完了
    }
    SystemClearSprite();// スプライトのクリア
    gameScroll = (gameScroll+1)&0x0f;// スクロールの更新
    GameHitCheck(); // ヒットチェック
    GroundUpdate(); // 地面の更新
    StarUpdate(); // 星の更新
    ShipUpdate(); // 自機の更新
    ShotUpdate(); // ショットの更新
    EnemyUpdate(); // 敵の更新
    BulletUpdate(); // 弾の更新
    GroundRender(); // 地面の描画
    StarRender(); // 星の描画
    ShipRender(); // 自機の描画
    ShotRender(); // ショットの描画
    EnemyRender(); // 敵の描画
    BulletRender(); // 弾の描画
    AppTransferPatternName(); // パターンネームの転送
    if (ship[SHIP_TYPE])return;// ゲームオーバーの条件
    gameState = GAME_STATE_OVER; // ゲームオーバー
}
// ゲームオーバーになる
static void GameOver(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        // ゲームオーバーの表示
        memcpy(&appPatternName[0x016b],gameOverString,0xa);
        AppTransferPatternName();// パターンネームの転送
        gameState++;// 初期化の完了
        gameScorePlus=60;
    }
    if(--gameScorePlus) return;
    appState = APP_STATE_GAME_INITIALIZE; // ゲーム初期化
}
// ヒットチェックを行う
static void GameHitCheck(void) {
    // ショットのチェック
    for(char *ix = shot, b = SHOT_N,c = 0;b;ix += SHOT_SIZE,--b) {
        if(ix[SHOT_STATE]==0)continue;
        unsigned short de = ((unsigned short)(ix[SHOT_POSITION_Y]&0xf8))*4+(ix[SHOT_POSITION_X]>>3);
        char a = enemyCollision[de]; 
        if (a) {
            char* iy = enemy -16 + a*16;
            if (--iy[ENEMY_HP]==0) {
                iy[ENEMY_TYPE] = ENEMY_TYPE_BOMB;
                iy[ENEMY_STATE] = 0;
            }
        } else if(ground[de]==0)continue;
        ix[SHOT_STATE] = 0;
    }
    #if 1
    // 弾のチェック
    for(char b=bulletN,*ix=bullet;b;b--,ix+=BULLET_SIZE) {
        if (ix[BULLET_STATE]==0)continue;
        unsigned short de = ((unsigned short)(ix[BULLET_POSITION_YI]&0xf8))*4+(ix[BULLET_POSITION_XI]>>3);
        if (ground[de]==0) {
            if (ship[SHIP_TYPE]!=SHIP_TYPE_VICVIPER) continue;
            signed short a = ((signed short)ship[SHIP_POSITION_X])-ix[BULLET_POSITION_XI];
            if (a < -4 || 4 < a) continue;
            a = ((signed short)ship[SHIP_POSITION_Y])-ix[BULLET_POSITION_YI];
            if (a < -4 || 4 < a) continue;
            //if (--ship[SHIP_HP]) break;
            ship[SHIP_TYPE] = SHIP_TYPE_BOMB;
            ship[SHIP_STATE] = 0;
        }
        ix[BULLET_STATE] = 0;
    }
    // 自機のチェック
    if (ship[SHIP_TYPE] != SHIP_TYPE_VICVIPER) return;
    u16 tmp =
        ((ship[SHIP_POSITION_Y]&0xf8)<<2)+(ship[SHIP_POSITION_X]>>3);
    if (enemyCollision[tmp]==0 && ground[tmp]==0) return;
    //if(--ship[SHIP_HP]) return;
    ship[SHIP_TYPE] = SHIP_TYPE_BOMB;
    ship[SHIP_STATE] = 0;
    #endif
}
