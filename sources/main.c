// main.c : メインプログラム
#include "bios.h"
#include "System.h"
#include "App.h"
#define H_TIMI_COUNT 0x02 /* タイマ割り込みカウンタ */
// 変数定義
static char h_timiRoutine[5];// タイマ割り込みルーチン
static char h_timiCount;// タイマ割り込みカウンタ
// メインプログラム
void main(void) __naked {
    __asm;    
    // 初期化
    // システムの初期化
    call    _SystemInitialize
    // アプリケーションの初期化
    call    _AppInitialize
    // 割り込みの禁止
    di
    // タイマ割り込み処理の保存
    ld      hl, #H_TIMI
    ld      de, #_h_timiRoutine
    ld      bc, #5
    ldir
    // タイマ割り込み処理の書き換え
    ld      a, #0xc3
    ld      (H_TIMI + 0), a
    ld      hl, #_H_timiEntry
    ld      (H_TIMI + 1), hl
    // タイマ割り込みカウンタの初期化
    xor     a
    ld      (_h_timiCount), a
    // 割り込み禁止の解除
    ei
    // キャンセル待ち
    0$:
        ld      a, (_flag)
        bit     #FLAG_CANCEL, a
    jr      z, 0$
    // 終了
    // アプリケーションの終了
    // システムの終了
    // キーボードバッファのクリア
    call    KILBUF
    // 割り込みの禁止
    di
    // タイマ割り込み処理の復帰
    ld      hl, #_h_timiRoutine
    ld      de, #H_TIMI
    ld      bc, #5
    ldir
    // 割り込み禁止の解除
    ei
    // 終了
    ret
    __endasm;
}
// タイマ割り込みのエントリ
static void H_timiEntry(void) __naked {
    __asm;
    // レジスタの保存
    push    af
    push    hl
    // 割り込み禁止の解除
    ei
    // タイマ割り込みカウンタの更新
    ld      hl, #_h_timiCount
    inc     (hl)
    // すでに割り込み済みかどうか
    ld      hl, #_flag
    bit     #FLAG_H_TIMI, (hl)
    jr      nz, H_timiEntryEnd
    // 指定されたタイマ割り込みカウンタに到達したか
    ld      a, (_h_timiCount)
    cp      #H_TIMI_COUNT
    jr      c, H_timiEntryEnd
    // 処理の開始
    set     #FLAG_H_TIMI, (hl)
    // 割り込みカウンタのクリア
    xor     a
    ld      (_h_timiCount), a
    // リクエストの取得
    ld      a, (_request)
    ld      h, a
    // ビデオレジスタの転送
    bit     #REQUEST_VIDEO_REGISTER, h
    call    nz, _SystemTransferVideoRegister
    // スプライトの転送
    call    _SystemTransferSprite
    // VRAM の転送
    bit     #REQUEST_VRAM, h
    call    nz, _SystemTransferVram
    // キー入力の更新
    call    _SystemUpdateInput
    // サウンドの更新
    call    _SystemUpdateSound
    // STOP キーによるキャンセル
    ld      a, (_input + INPUT_BUTTON_STOP)
    dec     a
    jr      nz, H_timiEntryDone
    ld      hl, #_flag
    set     #FLAG_CANCEL, (hl)
    // 処理の完了
    H_timiEntryDone:
        // アプリケーションの更新
        call    _AppUpdate
        // 割り込みの完了
        ld      hl, #_flag
        res     #FLAG_H_TIMI, (hl)
    // エントリの終了
    H_timiEntryEnd:
        // レジスタの復帰
        pop     hl
        pop     af
        // 保存されたタイマ割り込みルーチンの実行
        jp      _h_timiRoutine
        ret
    __endasm;
}
