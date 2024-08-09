// Title.c : タイトル
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Title.h"
#include "string.h"
// 定数の定義
static char const titleLogoString[] = {// ロゴ
    0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0xa0, 0xa1,
    0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0xb0, 0xb1,
    0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x8b, 0x8c, 0x8d, 0x9a,
    0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static char const titleSpaceString[] = {// SPACE キー
    0x70, 0x72, 0x65, 0x73, 0x73, 0x00, 0x73, 0x70, 0x61, 0x63, 0x65, 0x00, 0x62, 0x61, 0x72, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
// 変数の定義
static char titleState;    // 状態
static char titleAnimation;// アニメーション
static char titleTimer;    // タイマ
// タイトルを初期化する
void TitleInitialize(void) {
    SystemClearSprite();// スプライトのクリア
    memset(appPatternName,0,0x300);
    AppTransferPatternName();// パターンネームの転送
    // パターンジェネレータの設定
    videoRegister[VDP_R4] = APP_PATTERN_GENERATOR_TABLE_0 >> 11;
    videoRegister[VDP_R1] |= 1<<VDP_R1_BL;// 描画の開始
    request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
    titleState = 0;// 状態の設定
    appState = APP_STATE_TITLE_UPDATE;
}
static void updateInit(void) {
    if (titleState==0) {// 初期化の開始
        titleAnimation = 0; // アニメーションの初期化
        titleTimer = 0;     // タイマの初期化
        titleState++;       // 初期化の完了
    }
}
static void updateAnim(void) {
    // アニメーションの更新
    if (titleAnimation<0x60)titleAnimation++;
    memcpy(appPatternName + 0x00c0,titleLogoString,titleAnimation);
}
static void drawSpace(void) {
    // ハイスコアの描画
    if (titleAnimation<0x60)return;
    // SPACE キーの描画
    memcpy(appPatternName + 0x0228,titleSpaceString+(titleTimer&0x10),0xf);
}
// タイトルを更新する
void TitleUpdate(void) {
    updateInit();
    SystemGetRandom();// 乱数の更新
    if (titleState==1) {// SPACE キーの監視
        titleTimer++;// タイマの更新
        if (input[INPUT_BUTTON_SPACE]==1) {// SPACE キーの押下
            titleState++;
            titleAnimation=0x60;// アニメーションの設定
        }
    } else {
        titleTimer += 8;// タイマの更新
        if(titleTimer >= 255-8) {// ジングルの監視
            //videoRegister[VDP_R1] &= ~(1<<VDP_R1_BL); // 描画の停止
            //request |= 1<<REQUEST_VIDEO_REGISTER;// ビデオレジスタの転送
            appState = APP_STATE_GAME_INITIALIZE;// ゲームの開始
        }
    }
    updateAnim();
    drawSpace();
    AppTransferPatternName();
}
