// App.c : アプリケーション
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "string.h"
// 外部変数宣言
extern char const patternTable[];
// 定数の定義
// VDP レジスタ値（スクリーン１）
static char const videoScreen1[] = {
    0b00000000,
    0b10100010,
    APP_PATTERN_NAME_TABLE >> 10,
    APP_COLOR_TABLE >> 6,
    APP_PATTERN_GENERATOR_TABLE_0 >> 11,
    APP_SPRITE_ATTRIBUTE_TABLE >> 7,
    APP_SPRITE_GENERATOR_TABLE >> 11,
    0b00000111,
};
// 変数の定義
char appState;// 状態
char appColor;// 色
char appPatternName[0x300];// パターンネーム
typedef void(*FP0)(void);
// アプリケーションを初期化する
#define _FILVRM(dst,value,size) {\
    __asm ld      hl, dst __endasm;\
    __asm ld      a,value __endasm;\
    __asm ld      bc, size __endasm;\
    __asm call    FILVRM __endasm;}
#define _LDIRVM(dst,src,size) {\
    __asm ld      hl, src __endasm;\
    __asm ld      de, dst __endasm;\
    __asm ld      bc, size __endasm;\
    __asm call    LDIRVM __endasm;}

static void SetVramAddress(u16 vram) __naked {
    vram;
    __asm;
        // ポート取得
            ld      a, (_videoPort+1)
            inc     a
            ld      c, a
        // 出力アドレス設定
            ld      a, l
            out     (c), a
            ld      a, h
            or      a, #0b01000000
            out     (c), a
            dec     c
        ret
    __endasm;
}
static void LoadVram1(u8*addr,u16 size) __naked {
    addr;// hl
    size;// de
    __asm;
        // 転送設定
            ld      a, (_videoPort+1)
            ld      c, a
        // 転送 256*8 = 2048 = 2kbytes
            xor     a
            cp      d
            jp      z,LoadVram1_loop1_end
            ld      b, #0x00
    LoadVram1_loop1:
            otir
            dec d
            jp nz, LoadVram1_loop1
    LoadVram1_loop1_end:
            xor     a
            cp      e
            ret     z
            ld      b,e
            otir
            ret
    __endasm;
}    
void AppInitialize(void) {
    // アプリケーションの初期化
    ((FP0)DISSCR)(); // 画面表示の停止
    memcpy(videoRegister,videoScreen1,8);// ビデオの設定
    request |= 1<<REQUEST_VIDEO_REGISTER;
    __asm di __endasm;// 割り込みの禁止
    SetVramAddress(APP_SPRITE_GENERATOR_TABLE);
    char* hl = (void*)patternTable;
    for (char i=0;i<8;i++) {
        for (char j=0;j<16;j++) {
            LoadVram1(hl,8);hl+=0x80;
            LoadVram1(hl,8);hl-=0x78;
        }
        hl +=0x80;
    }
    _LDIRVM(#APP_PATTERN_GENERATOR_TABLE_0,#_patternTable,#0x1000);// パターンジェネレータの転送
    // 組み直し
    _LDIRVM(#(APP_PATTERN_GENERATOR_TABLE_0+0x1000),#(_patternTable+0x800),#0x800);// コピー生成
    _LDIRVM(#(APP_PATTERN_GENERATOR_TABLE_0+16*8*38),#(_patternTable+16*8*12),#(16*8*6));// ビックコアボディ
    _LDIRVM(#(APP_PATTERN_GENERATOR_TABLE_0+16*8*16),#(_patternTable+16*8*4),#(16*8*4));// 文字
    _LDIRVM(#(APP_PATTERN_GENERATOR_TABLE_0+16*8*32),#(_patternTable+16*8*4),#(16*8*4));// 文字

    _FILVRM(#APP_COLOR_TABLE,#0x31,#0x0020);// カラーテーブルの転送
    _FILVRM(#APP_PATTERN_NAME_TABLE,#0,#0x0600);// パターンネームの初期化
    __asm ei __endasm; // 割り込み禁止の解除
    appColor = 3;// 色の初期化
    appState = APP_STATE_GAME_INITIALIZE;// 状態の初期化
}
// アプリケーションを更新する
void AppUpdate(void) {
    char a = appState;// 状態の取得
    if (a == APP_STATE_GAME_INITIALIZE) GameInitialize();  // ゲームの初期化
    else /*if (a == APP_STATE_GAME_UPDATE)*/ GameUpdate(); // ゲームの更新
}
