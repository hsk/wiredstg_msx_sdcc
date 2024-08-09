// Game.c : ゲーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
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
    ShipInitialize(); // 自機の初期化
    gamePause = 0;// 一時停止の初期化
    gameScroll = 0;// スクロールの初期化
    memset(appPatternName,0,0x300);// パターンのクリア
    // videoRegister[VDP_R1] |= 1<<VDP_R1_BL;// 描画の開始
    // request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
    // 状態の設定
    gameState = GAME_STATE_PLAY;
    appState = APP_STATE_GAME_UPDATE;
}
static void GamePlay(void);
static void GameOver(void);
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
    ShipUpdate(); // 自機の更新
    ShipRender(); // 自機の描画
    if (ship[SHIP_TYPE])return;// ゲームオーバーの条件
    gameState = GAME_STATE_OVER; // ゲームオーバー
}
// ゲームオーバーになる
static void GameOver(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        // ゲームオーバーの表示
        memcpy(&appPatternName[0x016b],gameOverString,0xa);
        gameState++;// 初期化の完了
        gameScorePlus=60;
    }
    if(--gameScorePlus) return;
    appState = APP_STATE_GAME_INITIALIZE; // ゲーム初期化
}
