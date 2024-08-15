// Game.c : ゲーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "string.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
// ゲームを初期化する
void GameInitialize(void) {
    // ゲームの初期化
    ShipInitialize(); // 自機の初期化
    EnemyInitialize(); // 敵の初期化
    gamePause = 0;// 一時停止の初期化
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
// ゲームを更新する
void GameUpdate(void) {
    // ESC キーで一時停止
    if (input[INPUT_BUTTON_ESC] == 1) {
        gamePause = 1 - gamePause;
    }
    if (gamePause) return;
    SystemGetRandom();// 乱数の更新
    GamePlay();
}
// ゲームをプレイする
static void GamePlay(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        videoRegister[VDP_R1] |= 1<<VDP_R1_BL;// 描画の開始
        request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
        gameState++;// 初期化の完了
    }
    SystemClearSprite();// スプライトのクリア
    ShipUpdate(); // 自機の更新
    EnemyUpdate(); // 敵の更新
    ShipRender(); // 自機の描画
    EnemyRender(); // 敵の描画
    AppTransferPatternName(); // パターンネームの転送
}
