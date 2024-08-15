// System.c : システムライブラリ
#include "bios.h"
#include "vdp.h"
#include "System.h"
// 定数の定義
// キー入力
static char const inputStickTable[] = {
    0x00,
    (1 << INPUT_KEY_UP),
    (1 << INPUT_KEY_UP)    | (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_DOWN)  | (1 << INPUT_KEY_RIGHT),
    (1 << INPUT_KEY_DOWN),
    (1 << INPUT_KEY_DOWN)  | (1 << INPUT_KEY_LEFT),
    (1 << INPUT_KEY_LEFT),
    (1 << INPUT_KEY_UP)    | (1 << INPUT_KEY_LEFT),
};
// ビデオ
static char const videoScreenMode[] = {
    0b00000000,          // text 1
    0b10110000,
    (0x0000 >> 10),
    (0x0000 >> 6),
    (0x0800 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b11110100,
    0b00000000,          // graphic 1
    0b10100000,
    (0x1800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
    0b00000010,          // graphic 2
    0b10100000,
    (0x1800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
    0b00000000,          // multi color
    0b10101000,
    (0x0800 >> 10),
    (0x2000 >> 6),
    (0x0000 >> 11),
    (0x1b00 >> 7),
    (0x3800 >> 11),
    0b00000111,
};
//  三角関数
static char const trigonometricAtanAngleTable[] = {
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x20, 0x12, 0x0d, 0x09, 0x08, 0x06, 0x05,
    0x40, 0x2d, 0x20, 0x17, 0x12, 0x0f, 0x0d, 0x0b,
    0x40, 0x32, 0x28, 0x20, 0x1a, 0x16, 0x12, 0x10,
    0x40, 0x36, 0x2d, 0x25, 0x20, 0x1b, 0x17, 0x15,
    0x40, 0x37, 0x30, 0x29, 0x24, 0x20, 0x1c, 0x19,
    0x40, 0x39, 0x32, 0x2d, 0x28, 0x23, 0x20, 0x1c,
    0x40, 0x3a, 0x34, 0x2f, 0x2a, 0x26, 0x23, 0x20,
};
static char const trigonometricAtanOffsetTable[] = {
    0x00, 0x80, 0x00, 0x80,
};
// デバッグ出力
static char const hexString[] = "0123456789ABCDEF";
// 変数の定義
char flag;// フラグ
char request;// リクエスト
char input[INPUT_SIZE];// キー入力
static char inputBuffer;
char videoPort[2];// ビデオ
char videoRegister[8];
char videoTransfer[VIDEO_TRANSFER_VRAM_SIZE * VIDEO_TRANSFER_SIZE];
char sprite[128];// スプライト
static char random;// 乱数
// システムを初期化する
void SystemInitialize(void) __naked {
    __asm;
    // フラグの初期化
    xor     a
    ld      (_flag), a
    // キー入力の初期化
    call    _SystemInitializeInput
    // ビデオの初期化
    call    _SystemInitializeVideo
    // サウンドの初期化
    call    _SystemInitializeSound
    // 終了
    ret
    __endasm;
}
// キー入力を初期化する  
void SystemInitializeInput(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    // キー入力の初期化
    ld      hl, #_input
    xor     a
    ld      b, #INPUT_SIZE
    0$:
        ld      (hl), a
        inc     hl
    djnz    0$
    // レジスタの復帰
    pop     bc
    pop     hl
    // 終了
    ret
    __endasm;
}
// キーの入力を更新する
void SystemUpdateInput(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    push    ix
    push    iy
    // キーの取得
    ld      a, #0x00
    call    GTSTCK
    ld      c, a
    ld      b, #0x00
    ld      hl, #_inputStickTable
    add     hl, bc
    ld      a, (hl)
    ld      (_inputBuffer + 0), a
    ld      a, #0x01
    call    GTSTCK
    ld      c, a
    ld      b, #0x00
    ld      hl, #_inputStickTable
    add     hl, bc
    ld      a, (_inputBuffer + 0)
    or      (hl)
    ld      b, a
    ld      hl, #(_input + INPUT_KEY_UP)
    ld      a, (hl)
    inc     a
    jr      nz, 00$
        ld      a, #0xff
    00$:
    bit     #INPUT_KEY_UP, b
    jr      nz, 01$
        xor     a
    01$:
    ld      (hl), a
    ld      hl, #(_input + INPUT_KEY_DOWN)
    ld      a, (hl)
    inc     a
    jr      nz, 02$
        ld      a, #0xff
    02$:
    bit     #INPUT_KEY_DOWN, b
    jr      nz, 03$
        xor     a
    03$:
    ld      (hl), a
    ld      hl, #(_input + INPUT_KEY_LEFT)
    ld      a, (hl)
    inc     a
    jr      nz, 04$
        ld      a, #0xff
    04$:
    bit     #INPUT_KEY_LEFT, b
    jr      nz, 05$
        xor     a
    05$:
    ld      (hl), a
    ld      hl, #(_input + INPUT_KEY_RIGHT)
    ld      a, (hl)
    inc     a
    jr      nz, 06$
        ld      a, #0xff
    06$:
    bit     #INPUT_KEY_RIGHT, b
    jr      nz, 07$
        xor     a
    07$:
    ld      (hl), a
    // ボタンの取得
    ld      a, #0x00
    call    GTTRIG
    ld      (_inputBuffer + 0), a
    ld      a, #0x01
    call    GTTRIG
    ld      hl, #(_inputBuffer + 0)
    or      (hl)
    ld      hl, #(_input + INPUT_BUTTON_SPACE)
    jr      z, 10$
        ld      a, (hl)
        inc     a
        jr      nz, 10$
            ld      a, #0xff
    10$:
    ld      (hl), a
    ld      a, #0x03
    call    GTTRIG
    ld      (_inputBuffer + 0), a
    ld      a, #0x06
    call    SNSMAT
    cpl
    and     #0x01
    ld      hl, #(_inputBuffer + 0)
    or      (hl)
    ld      hl, #(_input + INPUT_BUTTON_SHIFT)
    jr      z, 11$
    ld      a, (hl)
    inc     a
    jr      nz, 11$
        ld      a, #0xff
    11$:
    ld      (hl), a
    ld      a, #0x07
    call    SNSMAT
    ld      b, a
    ld      hl, #(_input + INPUT_BUTTON_ESC)
    ld      a, (hl)
    inc     a
    jr      nz, 12$
        ld      a, #0xff
    12$:
    bit     #0x02, b
    jr      z, 13$
        xor     a
    13$:
    ld      (hl), a
    ld      hl, #(_input + INPUT_BUTTON_STOP)
    ld      a, (hl)
    inc     a
    jr      nz, 14$
        ld      a, #0xff
    14$:
    bit     #0x04, b
    jr      z, 15$
        xor     a
    15$:
    ld      (hl), a
    // レジスタの復帰
    pop     iy
    pop     ix
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// ビデオを初期化する
void SystemInitializeVideo(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // ポートの取得
    ld      a, (0x0006)
    ld      (_videoPort + 0), a
    ld      a, (0x0007)
    ld      (_videoPort + 1), a
    // レジスタの取得
    ld      hl, #RG0SAV
    ld      de, #_videoRegister
    ld      bc, #0x08
    ldir
    // VRAM の転送の初期化
    ld      hl, #_videoTransfer
    xor     a
    ld      b, #(VIDEO_TRANSFER_VRAM_SIZE * VIDEO_TRANSFER_SIZE)
    0$:
        ld      (hl), a
        inc     hl
    djnz    0$
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// スクリーンモードを設定する
void SystemSetScreenMode(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // レジスタの設定
    add     a, a
    add     a, a
    add     a, a
    ld      c, a
    ld      b, #0x00
    ld      hl, #_videoScreenMode
    add     hl, bc
    ld      de, #_videoRegister
    ld      bc, #0x08
    ldir
    // レジスタの転送
    call    _SystemTransferVideoRegister
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// レジスタを転送する
void SystemTransferVideoRegister(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    // 割り込みの禁止
    di
    // レジスタの設定
    ld      hl, #_videoRegister
    ld      a, (_videoPort + 1)
    inc     a
    ld      c, a
    outi
    ld      a, #0x80
    out     (c), a
    outi
    ld      a, #0x81
    out     (c), a
    outi
    ld      a, #0x82
    out     (c), a
    outi
    ld      a, #0x83
    out     (c), a
    outi
    ld      a, #0x84
    out     (c), a
    outi
    ld      a, #0x85
    out     (c), a
    outi
    ld      a, #0x86
    out     (c), a
    outi
    ld      a, #0x87
    out     (c), a
    // 割り込み禁止の解除
    ei
    // レジスタの復帰
    pop     bc
    pop     hl
    ret
    __endasm;
}
// VRAM にデータを転送する
void SystemTransferVram(void) __naked {
    __asm;    
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // 割り込みの禁止
    di
    // ポートの取得
    ld      a, (_videoPort + 1)
    ld      d, a
    inc     a
    ld      e, a
    // VRAM 0 の転送
    ld      hl, (_videoTransfer + VIDEO_TRANSFER_VRAM_0_SRC)
    ld      a, l
    or      h
    jr      z, 01$
        ld      c, e
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_0_DST + 0)
        out     (c), a
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_0_DST + 1)
        or      #0b01000000
        out     (c), a
        ld      c, d
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_0_BYTES)
        ld      b, a
        otir
    // VRAM 1 の転送
    01$:
    ld      hl, (_videoTransfer + VIDEO_TRANSFER_VRAM_1_SRC)
    ld      a, l
    or      h
    jr      z, 02$
        ld      c, e
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_1_DST + 0)
        out     (c), a
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_1_DST + 1)
        or      #0b01000000
        out     (c), a
        ld      c, d
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_1_BYTES)
        ld      b, a
        otir
    // VRAM 2 の転送
    02$:
    ld      hl, (_videoTransfer + VIDEO_TRANSFER_VRAM_2_SRC)
    ld      a, l
    or      h
    jr      z, 03$
        ld      c, e
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_2_DST + 0)
        out     (c), a
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_2_DST + 1)
        or      #0b01000000
        out     (c), a
        ld      c, d
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_2_BYTES)
        ld      b, a
        otir
    // VRAM 3 の転送
    03$:
    ld      hl, (_videoTransfer + VIDEO_TRANSFER_VRAM_3_SRC)
    ld      a, l
    or      h
    jr      z, 90$
        ld      c, e
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_3_DST + 0)
        out     (c), a
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_3_DST + 1)
        or      #0b01000000
        out     (c), a
        ld      c, d
        ld      a, (_videoTransfer + VIDEO_TRANSFER_VRAM_3_BYTES)
        ld      b, a
        otir
        // 割り込み禁止の解除
        ei
        // 転送の完了
    90$:
    ld      hl, #_videoTransfer
    xor     a
    ld      b, #(VIDEO_TRANSFER_VRAM_SIZE * VIDEO_TRANSFER_SIZE)
    91$:
        ld      (hl), a
        inc     hl
    djnz    91$
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// スプライトをクリアする
void SystemClearSprite(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // スプライトのクリア
    ld      hl, #(_sprite + 0)
    ld      de, #(_sprite + 1)
    ld      bc, #(0x80 - 1)
    ld      a, #0xc0
    ld      (hl), a
    ldir
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// スプライトを転送する
void SystemTransferSprite(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    // スプライトアトリビュートテーブルの取得
    ld      a, (_videoRegister + VDP_R5)
    ld      l, a
    ld      h, #0x00
    sla     l
    rl      h
    sla     l
    rl      h
    sla     l
    rl      h
    sla     l
    rl      h
    sla     l
    rl      h
    sla     l
    rl      h
    sla     l
    rl      h
    // 割り込みの禁止
    di
    // VRAM アドレスの設定
    ld      a, (_videoPort + 1)
    inc     a
    ld      c, a
    //ld      a, #<VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE
    //out     (c), a
    //ld      a, #(>VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE | 0b01000000)
    out     (c), l
    ld      a, h
    or      #0b01000000
    out     (c), a
    dec     c
    // スプライトアトリビュートテーブルの転送
    ld      hl, #_sprite
    ld      b, #0x80
    otir
    // 割り込み禁止の解除
    ei
    // レジスタの復帰
    pop     bc
    pop     hl
    ret
    __endasm;
}
// スプライトを設定する
void SystemSetSprite(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // スプライトの設定
    ld      a, (hl)
    add     a, c
    ld      (de), a
    inc     hl
    inc     de
    ld      a, (hl)
    add     a, b
    ld      b, #0x00
    cp      #0xf0
    jr      c, 0$
        add     a, #0x20
        ld      b, #0x80
    0$:
    ld      (de), a
    inc     hl
    inc     de
    ld      a, (hl)
    ld      (de), a
    inc     hl
    inc     de
    ld      a, (hl)
    or      b
    ld      (de), a
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// 乱数を取得する
char SystemGetRandom(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    // 乱数の生成
    ld      hl, #_random
    ld      a, #0xaa
    xor     (hl)
    add     a, #73
    ld      (hl), a
    // レジスタの復帰
    pop     hl
    ret
    __endasm;
}
//  三角関数
static u16 const trigonometricTable[] = {
    0x0000, 0x0006, 0x000c, 0x0012, 0x0019, 0x001f, 0x0025, 0x002b, 0x0031, 0x0038, 0x003e, 0x0044, 0x004a, 0x0050, 0x0056, 0x005c,
    0x0061, 0x0067, 0x006d, 0x0073, 0x0078, 0x007e, 0x0083, 0x0088, 0x008e, 0x0093, 0x0098, 0x009d, 0x00a2, 0x00a7, 0x00ab, 0x00b0,
    0x00b5, 0x00b9, 0x00bd, 0x00c1, 0x00c5, 0x00c9, 0x00cd, 0x00d1, 0x00d4, 0x00d8, 0x00db, 0x00de, 0x00e1, 0x00e4, 0x00e7, 0x00ea,
    0x00ec, 0x00ee, 0x00f1, 0x00f3, 0x00f4, 0x00f6, 0x00f8, 0x00f9, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00fe, 0x00ff, 0x00ff, 0x00ff,
    0x0100, 0x00ff, 0x00ff, 0x00ff, 0x00fe, 0x00fe, 0x00fd, 0x00fc, 0x00fb, 0x00f9, 0x00f8, 0x00f6, 0x00f4, 0x00f3, 0x00f1, 0x00ee,
    0x00ec, 0x00ea, 0x00e7, 0x00e4, 0x00e1, 0x00de, 0x00db, 0x00d8, 0x00d4, 0x00d1, 0x00cd, 0x00c9, 0x00c5, 0x00c1, 0x00bd, 0x00b9,
    0x00b5, 0x00b0, 0x00ab, 0x00a7, 0x00a2, 0x009d, 0x0098, 0x0093, 0x008e, 0x0088, 0x0083, 0x007e, 0x0078, 0x0073, 0x006d, 0x0067,
    0x0061, 0x005c, 0x0056, 0x0050, 0x004a, 0x0044, 0x003e, 0x0038, 0x0031, 0x002b, 0x0025, 0x001f, 0x0019, 0x0012, 0x000c, 0x0006,
    0x0000, 0xfffa, 0xfff4, 0xffee, 0xffe7, 0xffe1, 0xffdb, 0xffd5, 0xffcf, 0xffc8, 0xffc2, 0xffbc, 0xffb6, 0xffb0, 0xffaa, 0xffa4,
    0xff9f, 0xff99, 0xff93, 0xff8d, 0xff88, 0xff82, 0xff7d, 0xff78, 0xff72, 0xff6d, 0xff68, 0xff63, 0xff5e, 0xff59, 0xff55, 0xff50,
    0xff4b, 0xff47, 0xff43, 0xff3f, 0xff3b, 0xff37, 0xff33, 0xff2f, 0xff2c, 0xff28, 0xff25, 0xff22, 0xff1f, 0xff1c, 0xff19, 0xff16,
    0xff14, 0xff12, 0xff0f, 0xff0d, 0xff0c, 0xff0a, 0xff08, 0xff07, 0xff05, 0xff04, 0xff03, 0xff02, 0xff02, 0xff01, 0xff01, 0xff01,
    0xff00, 0xff01, 0xff01, 0xff01, 0xff02, 0xff02, 0xff03, 0xff04, 0xff05, 0xff07, 0xff08, 0xff0a, 0xff0c, 0xff0d, 0xff0f, 0xff12,
    0xff14, 0xff16, 0xff19, 0xff1c, 0xff1f, 0xff22, 0xff25, 0xff28, 0xff2c, 0xff2f, 0xff33, 0xff37, 0xff3b, 0xff3f, 0xff43, 0xff47,
    0xff4b, 0xff50, 0xff55, 0xff59, 0xff5e, 0xff63, 0xff68, 0xff6d, 0xff72, 0xff78, 0xff7d, 0xff82, 0xff88, 0xff8d, 0xff93, 0xff99,
    0xff9f, 0xffa4, 0xffaa, 0xffb0, 0xffb6, 0xffbc, 0xffc2, 0xffc8, 0xffcf, 0xffd5, 0xffdb, 0xffe1, 0xffe7, 0xffee, 0xfff4, 0xfffa,
};
short SystemGetSin(char a) { // cos の値を取得する
    return trigonometricTable[a];
}
short SystemGetCos(char a) { // cos の値を取得する
    return trigonometricTable[(char)(a+0x40)];
}
// atan2 の値を取得する
char SystemGetAtan2(short a) __naked {
    a;
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    // 値の取得
    ld      c, #0x00
    ld      a, l
    sla     l
    rl      c
    bit     #0x00, c
    jr      z, 0$
        neg
        jp      po, 0$
        ld      a, #0x7f
    0$:
    ld      l, a
    ld      a, h
    sla     h
    rl      c
    bit     #0x00, c
    jr      z, 1$
        neg
        jp      po, 1$
            ld      a, #0x7f
    1$:
    ld      h, a
    cp      l
    jr      nc, 2$
        ld      a, l
    2$:
        cp      #0x08
        jr      c, 3$
            sra     a
            sra     h
            sra     l
            jr      2$
    3$:
    ld      a, l
    sla     a
    sla     a
    sla     a
    add     a, h
    ld      e, a
    ld      d, #0x00
    ld      hl, #_trigonometricAtanAngleTable
    add     hl, de
    ld      a, (hl)
    ld      b, c
    inc     b
    bit     #0x01, b
    jr      z, 4$
        neg
    4$:
    ld      b, #0x00
    ld      hl, #_trigonometricAtanOffsetTable
    add     hl, bc
    add     a, (hl)
    // レジスタの復帰
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// A レジスタの内容を出力する
void SystemPutRegisterA(void) __naked {
    __asm;
    // レジスタの保存
    push    af
    push    hl
    push    bc
    // 上位４ビットの出力
    push    af
    rra
    rra
    rra
    rra
    and     #0x0f
    ld      c, a
    ld      b, #0x00
    ld      hl, #_hexString
    add     hl, bc
    ld      a, (hl)
    call    CHPUT
    pop     af
    // 下位４ビットの出力
    and     #0b00001111
    ld      c, a
    ld      b, #0x00
    ld      hl, #_hexString
    add     hl, bc
    ld      a, (hl)
    call    CHPUT
    // レジスタの復帰
    pop     bc
    pop     hl
    pop     af
    ret
    __endasm;
}
