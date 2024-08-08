// App.c : アプリケーション
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Title.h"
#include "Game.h"
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
// スコア
//static char const appScoreDefault[] = { 0x00, 0x00, 0x00, 0x05, 0x07, 0x03, };
static char const appScoreDefault[] = { 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, };
// 変数の定義
char appState;// 状態
char appColor;// 色
char appScore[6];// スコア
char appPatternName[0x300];// パターンネーム
// アプリケーションを初期化する
void AppInitialize(void) __naked {
    __asm;
    // アプリケーションの初期化
    // 画面表示の停止
    call    DISSCR
    // ビデオの設定
    ld      hl, #_videoScreen1
    ld      de, #_videoRegister
    ld      bc, #0x08
    ldir
    ld      hl, #_request
    set     #REQUEST_VIDEO_REGISTER, (hl)
    // 割り込みの禁止
    di
    // VDP ポートの取得
    ld      a, (_videoPort + 1)
    ld      c, a
    // スプライトジェネレータの転送
    inc     c
    ld      a, #<APP_SPRITE_GENERATOR_TABLE
    out     (c), a
    ld      a, #(>APP_SPRITE_GENERATOR_TABLE | 0b01000000)
    out     (c), a
    dec     c
    ld      hl, #(_patternTable + 0x0000)
    ld      d, #0x08
    0$:
        ld      e, #0x10
        1$:
            push    de
            ld      b, #0x08
            otir
            ld      de, #0x78
            add     hl, de
            ld      b, #0x08
            otir
            ld      de, #0x80
            or      a
            sbc     hl, de
            pop     de
            dec     e
        jr      nz, 1$
        ld      a, #0x80
        add     a, l
        ld      l, a
        ld      a, h
        adc     a, #0x00
        ld      h, a
        dec     d
    jr      nz, 0$
    // パターンジェネレータの転送
    ld      hl, #(_patternTable + 0x0000)
    ld      de, #APP_PATTERN_GENERATOR_TABLE_0
    ld      bc, #0x1800
    call    LDIRVM
    // カラーテーブルの転送
    ld      hl, #APP_COLOR_TABLE
    ld      a, #0x31
    ld      bc, #0x0020
    call    FILVRM
    // パターンネームの初期化
    ld      hl, #APP_PATTERN_NAME_TABLE
    xor     a
    ld      bc, #0x0600
    call    FILVRM
    // 割り込み禁止の解除
    ei
    // 色の初期化
    ld      a, #0x03
    ld      (_appColor), a
    // スコアの初期化
    ld      hl, #_appScoreDefault
    ld      de, #_appScore
    ld      bc, #0x0006
    ldir
    // 状態の初期化
    ld      a, #APP_STATE_TITLE_INITIALIZE
    ld      (_appState), a
    ret
    __endasm;
}
// アプリケーションを更新する
void AppUpdate(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    push    ix
    push    iy
    // 状態の取得
    ld      a, (_appState)
    // タイトルの初期化
    cp      #APP_STATE_TITLE_INITIALIZE
    jr      nz, 2$
        call    _TitleInitialize
        jr      9$
        // タイトルの更新
    2$:
        cp      #APP_STATE_TITLE_UPDATE
        jr      nz, 3$
        // レジスタの保存
        push    hl
        push    bc
        push    de
        push    ix
        push    iy
        call    _TitleUpdate
        // レジスタの復帰
        pop     iy
        pop     ix
        pop     de
        pop     bc
        pop     hl
        jr      9$
        // ゲームの初期化
    3$:
        cp      #APP_STATE_GAME_INITIALIZE
        jr      nz, 4$
        call    _GameInitialize
        jr      9$
        // ゲームの更新
    4$:
        cp      #APP_STATE_GAME_UPDATE
        jr      nz, 3$
        // レジスタの保存
        call    _GameUpdate
        //jr      9$
        // 更新の終了
    9$:
    // レジスタの復帰
    pop     iy
    pop     ix
    pop     de
    pop     bc
    pop     hl
    // 終了
    ret
    __endasm;
}
// パターンネームを転送する
void AppTransferPatternName(void) __naked {
    __asm;
    // パターンネームの転送
    xor     a
    ld      hl, #(_appPatternName + 0x0000)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_0_SRC), hl
    ld      hl, #(APP_PATTERN_NAME_TABLE + 0x0000)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_0_DST), hl
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_0_BYTES), a
    ld      hl, #(_appPatternName + 0x0100)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_1_SRC), hl
    ld      hl, #(APP_PATTERN_NAME_TABLE + 0x0100)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_1_DST), hl
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_1_BYTES), a
    ld      hl, #(_appPatternName + 0x0200)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_2_SRC), hl
    ld      hl, #(APP_PATTERN_NAME_TABLE + 0x0200)
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_2_DST), hl
    ld      (_videoTransfer + VIDEO_TRANSFER_VRAM_2_BYTES), a
    ld      hl, #(_request)
    set     #REQUEST_VRAM, (hl)
    // 終了
    ret
    __endasm;
}
