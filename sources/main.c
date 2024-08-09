// main.c : メインプログラム
#include "bios.h"
#include "System.h"
#include "App.h"
#include "string.h"
#define H_TIMI_COUNT 0x02 /* タイマ割り込みカウンタ */
// 変数定義
static char h_timiRoutine[5];// タイマ割り込みルーチン
static char h_timiCount;// タイマ割り込みカウンタ
static void H_timiEntry(void);
// メインプログラム
void main(void) {
    // 初期化
    // システムの初期化
    SystemInitialize();
    AppInitialize();// アプリケーションの初期化
    __asm di __endasm; // 割り込みの禁止
    // タイマ割り込み処理の保存
    //for(char *hl = (char*)H_TIMI,*de = h_timiRoutine,bc=5;bc;bc--)*de++=*hl++;
    memcpy(h_timiRoutine,(char*)H_TIMI,5);
    // タイマ割り込み処理の書き換え
    ((char*)H_TIMI)[0] = 0xc3;
    *(short*)&((char*)H_TIMI)[1] = (short)H_timiEntry;
    // タイマ割り込みカウンタの初期化
    h_timiCount = 0;
    __asm ei __endasm; // 割り込みの禁止の解除
    while((flag & (1<<FLAG_CANCEL)) == 0);// キャンセル待ち
    // 終了
    // アプリケーションの終了
    // システムの終了
    // キーボードバッファのクリア
    typedef void (*FP)(void);
    ((FP)KILBUF)();
    __asm di __endasm; // 割り込みの禁止
    // タイマ割り込み処理の復帰
    //for(char *hl = h_timiRoutine,*de = (char*)H_TIMI,bc=5;bc;bc--)*de++=*hl++;
    memcpy((char*)H_TIMI,h_timiRoutine,5);
    __asm ei __endasm; // 割り込みの禁止の解除
}
// タイマ割り込みのエントリ
static void H_timiEntry(void) __naked {
    __asm ei __endasm; // 割り込みの禁止の解除
    h_timiCount++;// タイマ割り込みカウンタの更新
    if (!(flag & (1<<FLAG_H_TIMI)) &&// すでに割り込み済みかどうか
        h_timiCount>=H_TIMI_COUNT) {// 指定されたタイマ割り込みカウンタに到達したか
        // 処理の開始
        flag |= (1<<FLAG_H_TIMI); // 割り込みの完了
        h_timiCount = 0;// 割り込みカウンタのクリア
        if (request&(1<<REQUEST_VIDEO_REGISTER)) SystemTransferVideoRegister(); // ビデオレジスタの転送
        // スプライトの転送
        SystemTransferSprite();
        if (request & (1 << REQUEST_VRAM)) SystemTransferVram(); // VRAM の転送
        SystemUpdateInput();// キー入力の更新
        if (input[INPUT_BUTTON_STOP]==1) flag |= (1 << FLAG_CANCEL);// STOP キーによるキャンセル
        // 処理の完了
        AppUpdate(); // アプリケーションの更新
        flag &= ~(1<<FLAG_H_TIMI); // 割り込みの完了
    }
    __asm;
        jp      _h_timiRoutine// 保存されたタイマ割り込みルーチンの実行
    __endasm;
}
