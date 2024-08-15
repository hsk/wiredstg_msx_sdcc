// Game.c : ゲーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
#include "string.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
// 定数の定義
// ゲームオーバー
static char const gameOverString[] = {
    0x27, 0x21, 0x2d, 0x25, 0x00, 0x00, 0x2f, 0x36, 0x25, 0x32,
};
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
// ゲームを初期化する
void GameInitialize(void) {
    // ゲームの初期化
    ShipInitialize(); // 自機の初期化
    ShotInitialize(); // ショットの初期化
    EnemyInitialize(); // 敵の初期化
    BulletInitialize(); // 弾の初期化
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
static void GameHitCheck(void);
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
    GameHitCheck(); // ヒットチェック
    ShipUpdate(); // 自機の更新
    ShotUpdate(); // ショットの更新
    EnemyUpdate(); // 敵の更新
    BulletUpdate(); // 弾の更新
    ShipRender(); // 自機の描画
    ShotRender(); // ショットの描画
    EnemyRender(); // 敵の描画
    BulletRender(); // 弾の描画
    AppTransferPatternName(); // パターンネームの転送
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
            ix[SHOT_STATE] = 0;
        }
    }
}
