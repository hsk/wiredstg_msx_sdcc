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
// サウンド
static short const soundToneFrequencyTable[] = {
    0x0000, 0x0000, 0x0d5d, 0x0c9c, 0x0be7, 0x0b3c, 0x0a9b, 0x0a02, 0x0a02, 0x0973, 0x08eb, 0x086b, 0x07f2, 0x0780, 0x0714, 0x06af, // O1
    0x0000, 0x0714, 0x06af, 0x064e, 0x05f4, 0x059e, 0x054e, 0x0501, 0x0501, 0x04ba, 0x0476, 0x0436, 0x03f9, 0x03c0, 0x038a, 0x0357, // O2
    0x0000, 0x038a, 0x0357, 0x0327, 0x02fa, 0x02cf, 0x02a7, 0x0281, 0x0281, 0x025d, 0x023b, 0x021b, 0x01fd, 0x01e0, 0x01c5, 0x01ac, // O3
    0x0000, 0x01c5, 0x01ac, 0x0194, 0x017d, 0x0168, 0x0153, 0x0140, 0x0140, 0x012e, 0x011d, 0x010d, 0x00fe, 0x00f0, 0x00e3, 0x00d6, // O4
    0x0000, 0x00e3, 0x00d6, 0x00ca, 0x00be, 0x00b4, 0x00aa, 0x00a0, 0x00a0, 0x0097, 0x008f, 0x0087, 0x007f, 0x0078, 0x0071, 0x006b, // O5
    0x0000, 0x0071, 0x006b, 0x0065, 0x005f, 0x005a, 0x0055, 0x0050, 0x0050, 0x004c, 0x0047, 0x0043, 0x0040, 0x003c, 0x0039, 0x0035, // O6
    0x0000, 0x0039, 0x0035, 0x0032, 0x0030, 0x002d, 0x002a, 0x0028, 0x0028, 0x0026, 0x0024, 0x0022, 0x0020, 0x001e, 0x001c, 0x001b, // O7
    0x0000, 0x001c, 0x001b, 0x0019, 0x0018, 0x0016, 0x0015, 0x0014, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010, 0x000d, 0x000e, 0x000d, // O8
};
static char const soundLengthTable[] = {
    1,       // T1 L0 32
    2,       // T1 L1 16
    3,       // T1 L2 16.
    4,       // T1 L3  8
    6,       // T1 L4  8.
    8,       // T1 L5  4
    12,      // T1 L6  4.
    16,      // T1 L7  2
    24,      // T1 L8  2.
    32,      // T1 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    2,       // T2 L0 32
    4,       // T2 L1 16
    6,       // T2 L2 16.
    8,       // T2 L3  8
    12,      // T2 L4  8.
    16,      // T2 L5  4
    24,      // T2 L6  4.
    32,      // T2 L7  2
    48,      // T2 L8  2.
    64,      // T2 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    3,       // T3 L0 32
    6,       // T3 L1 16
    9,       // T3 L2 16.
    12,      // T3 L3  8
    18,      // T3 L4  8.
    24,      // T3 L5  4
    36,      // T3 L6  4.
    48,      // T3 L7  2
    72,      // T3 L8  2.
    96,      // T3 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    4,       // T4 L0 32
    8,       // T4 L1 16
    12,      // T4 L2 16.
    16,      // T4 L3  8
    24,      // T4 L4  8.
    32,      // T4 L5  4
    48,      // T4 L6  4.
    64,      // T4 L7  2
    96,      // T4 L8  2.
    128,     // T4 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    5,       // T5 L0 32
    10,      // T5 L1 16
    15,      // T5 L2 16.
    20,      // T5 L3  8
    30,      // T5 L4  8.
    40,      // T5 L5  4
    60,      // T5 L6  4.
    80,      // T5 L7  2
    120,     // T5 L8  2.
    160,     // T5 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    6,       // T6 L0 32
    12,      // T6 L1 16
    18,      // T6 L2 16.
    24,      // T6 L3  8
    32,      // T6 L4  8.
    48,      // T6 L5  4
    72,      // T6 L6  4.
    96,      // T6 L7  2
    144,     // T6 L8  2.
    192,     // T6 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    7,       // T7 L0 32
    14,      // T7 L1 16
    21,      // T7 L2 16.
    28,      // T7 L3  8
    42,      // T7 L4  8.
    56,      // T7 L5  4
    84,      // T7 L6  4.
    112,     // T7 L7  2
    168,     // T7 L8  2.
    224,     // T7 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    8,       // T8 L0 32
    16,      // T8 L1 16
    24,      // T8 L2 16.
    32,      // T8 L3  8
    48,      // T8 L4  8.
    64,      // T8 L5  4
    96,      // T8 L6  4.
    128,     // T8 L7  2
    192,     // T8 L8  2.
    0,       // T8 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
};
static unsigned short const soundEnvelopeTable[] = {
    0,    128,   256,   512,  1024,  2048,  4096,  8192, 16384, 32768,
};
static char const soundNoiseTable[] = {
    0,  1,  2,  4,  8, 12, 16, 20, 24, 31,
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
char* soundRequest[4];// サウンド
char soundHead[8];
char soundPlay[8];
static char soundS;
static char soundM[2];
static char soundN;
static char soundMixing;
static char soundT[8];
static char soundV[8];
static char soundVolume[8];
static char soundVminus[8];
static char soundO[8];
static char soundL[8];
static char soundTone[8];
static char soundNoise[8];
static char soundRest[8];
static char soundUpdate[8];
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
// サウンドを初期化する
void SystemInitializeSound(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    push    ix
    push    iy
    // PSG の初期化
    call    GICINI
    ld      e, #0b10111111
    ld      a, #0x07
    call    WRTPSG
    //   // サウンドレジスタの初期化
    //   ld      0x00(ix), #0b01010101
    //   ld      0x01(ix), #0b00000000
    //   ld      0x02(ix), #0b00000000
    //   ld      0x03(ix), #0b00000000
    //   ld      0x04(ix), #0b00000000
    //   ld      0x05(ix), #0b00000000
    //   ld      0x06(ix), #0b00000000
    //   ld      0x07(ix), #0b10111111
    //   ld      0x08(ix), #0b00000000
    //   ld      0x09(ix), #0b00000000
    //   ld      0x0a(ix), #0b00000000
    //   ld      0x0b(ix), #0b00001011
    //   ld      0x0c(ix), #0b00000000
    //   ld      0x0d(ix), #0b00000000
    //   ld      0x0e(ix), #0b00000000
    //   ld      0x0f(ix), #0b00000000
    // サウンドデータの初期化
    ld      hl, #0x0000
    ld      (_soundRequest + 0), hl
    ld      (_soundRequest + 2), hl
    ld      (_soundRequest + 4), hl
    ld      (_soundRequest + 6), hl
    ld      (_soundHead + 0), hl
    ld      (_soundHead + 2), hl
    ld      (_soundHead + 4), hl
    ld      (_soundHead + 6), hl
    ld      (_soundPlay + 0), hl
    ld      (_soundPlay + 2), hl
    ld      (_soundPlay + 4), hl
    ld      (_soundPlay + 6), hl
    // サウンドパラメータの初期化
    xor     a
    ld      (_soundS), a
    ld      (_soundM + 0), a
    ld      (_soundM + 1), a
    ld      (_soundN), a
    ld      a, #0b10111111
    ld      (_soundMixing), a
    0$:
        call    _SystemClearSoundChannel
        inc     a
        cp      #0x04
    jr      nz,  0$
    // レジスタの復帰
    pop     iy
    pop     ix
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}
// サウンドを更新する
void SystemUpdateSound(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    push    ix
    push    iy
    // スリープの確認
    ld      hl, #_flag
    bit     #FLAG_SOUND_SLEEP, (hl)
    jr      z, 90$
        ld      e, #0b10111111
        ld      a, #0x07
        call    WRTPSG
        jp      SystemUpdateSoundEnd
    90$:
    // チャンネルの走査
    ld      bc, #0x0000
    // １チャンネルの処理
    SystemUpdateSoundLoop:
        // リクエスト
        ld      ix, #_soundRequest
        add     ix, bc
        ld      a, 0(ix)
        or      1(ix)
        jr      z, 00$
            ld      l, 0(ix)
            ld      h, 1(ix)
            ld      0(ix), b
            ld      1(ix), b
            ld      ix, #_soundHead
            add     ix, bc
            ld      0(ix), l
            ld      1(ix), h
            ld      ix, #_soundPlay
            add     ix, bc
            ld      0(ix), l
            ld      1(ix), h
            ld      a, c
            srl     a
            call    _SystemClearSoundChannel
        00$:
        // サウンドデータの存在
        ld      ix, #_soundPlay
        add     ix, bc
        ld      a, 0(ix)
        or      1(ix)
        jp      z, SystemUpdateSoundNext
        // 待機
        ld      hl, #_soundRest
        add     hl, bc
        dec     (hl)
        jr      z, 01$
            ld      hl, #_soundVminus
            add     hl, bc
            ld      a, (hl)
            or      a
            jp      z, SystemUpdateSoundNext
            dec     (hl)
            jp      nz, SystemUpdateSoundNext
            ld      (hl), #0x04
            ld      hl, #_soundVolume
            add     hl, bc
            ld      a, (hl)
            cp      #0x01
            ccf
            sbc     b
            ld      (hl), a
            ld      e, a
            ld      a, c
            srl     a
            add     a, #0x08
            cp      #0x0b
            ccf
            sbc     b
            call    WRTPSG
            jp      SystemUpdateSoundNext
        01$:
        // 再生ポインタの取得
        //   ld      ix, #_soundPlay
        //   add     ix, bc
        ld      l, 0(ix)
        ld      h, 1(ix)
        // MML の解析
        SystemUpdateSoundMml:
            ld      a, (hl)
            inc     hl
            // 0x00 : 終端コード
            SystemUpdateSoundMml00:
                or      a
                jr      nz, SystemUpdateSoundMmlFf
                    ld      ix, #_soundHead
                    add     ix, bc
                    ld      0(ix), a
                    ld      1(ix), a
                    ld      ix, #_soundPlay
                    add     ix, bc
                    ld      0(ix), a
                    ld      1(ix), a
                    ld      ix, #_soundTone
                    add     ix, bc
                    ld      0(ix), a
                    ld      1(ix), a
                    ld      ix, #_soundNoise
                    add     ix, bc
                    ld      0(ix), a
                    ld      ix, #_soundUpdate
                    add     ix, bc
                    inc     0(ix)
                    jp      SystemUpdateSoundNext
            // $ff : 繰り返し
            SystemUpdateSoundMmlFf:
                cp      #0xff
                jr      nz, SystemUpdateSoundMmlS
                    ld      ix, #_soundHead
                    add     ix, bc
                    ld      iy, #_soundPlay
                    add     iy, bc
                    ld      l, 0(ix)
                    ld      0(iy), l
                    ld      h, 1(ix)
                    ld      1(iy), h
                    jr      SystemUpdateSoundMml
            // 'S' : エンベロープ波形（S0 ～ S15）
            SystemUpdateSoundMmlS:
                cp      #'S'
                jr      nz, SystemUpdateSoundMmlM
                ld      a, (hl)
                inc     hl
                sub     #'0'
                ld      (_soundS), a
                ld      a, (hl)
                cp      #'0'
                jr      c, SystemUpdateSoundMml
                cp      #('9' + 0x01)
                jr      nc, SystemUpdateSoundMml
                sub     #'0'
                add     a, #0x0a
                ld      (_soundS), a
                inc     hl
                jr      SystemUpdateSoundMml
            // 'M' : エンベロープ周期（M0 ～ M9）
            SystemUpdateSoundMmlM:
                cp      #'M'
                jr      nz, SystemUpdateSoundMmlN
                ld      a, (hl)
                inc     hl
                sub     #'0'
                add     a, a
                ld      e, a
                ld      d, #0x00
                ld      ix, #_soundEnvelopeTable
                add     ix, de
                ld      a, 0(ix)
                ld      (_soundM + 0), a
                ld      a, 1(ix)
                ld      (_soundM + 1), a
                jp      SystemUpdateSoundMml    
            // 'N' : ノイズ周波数（N0 ～ N9）
            SystemUpdateSoundMmlN:
                cp      #'N'
                jr      nz, SystemUpdateSoundMmlT
                ld      a, (hl)
                inc     hl
                sub     #'0'
                ld      e, a
                ld      d, b
                ld      ix, #_soundNoiseTable
                add     ix, de
                ld      a, 0(ix)
                ld      (_soundN), a
                jp      SystemUpdateSoundMml
            // 'T' : テンポ（T1 ～ T8）
            SystemUpdateSoundMmlT:
                cp      #'T'
                jr      nz, SystemUpdateSoundMmlV
                ld      a, (hl)
                inc     hl
                sub     #'1'
                ld      ix, #_soundT
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
            // 'V' : 音量（V0 ～ V16）
            SystemUpdateSoundMmlV:
                cp      #'V'
                jr      nz, SystemUpdateSoundMmlO
                ld      a, (hl)
                inc     hl
                ld      ix, #_soundV
                add     ix, bc
                sub     #'0'
                ld      0(ix), a
                ld      a, (hl)
                cp      #'0'
                jp      c, SystemUpdateSoundMmlVminus
                cp      #('9' + 0x01)
                jp      nc, SystemUpdateSoundMmlVminus
                sub     #'0'
                add     #0x0a
                ld      0(ix), a
                inc     hl    
            // 'V-' : 音量の減衰
            SystemUpdateSoundMmlVminus:
                ld      a, (hl)
                cp      #'-'
                ld      a, b
                jr      nz, 10$
                    ld      a, #0x04
                    inc     hl
                10$:
                ld      ix, #_soundVminus
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
            // 'O' : オクターブ（O1 ～ O8）
            SystemUpdateSoundMmlO:
                cp      #'O'
                jr      nz, SystemUpdateSoundMmlL
                ld      a, (hl)
                inc     hl
                sub     #'1'
                ld      ix, #_soundO
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
            // 'L' : 音の長さ（L0 ～ L9）
            SystemUpdateSoundMmlL:
                cp      #'L'
                jr      nz, SystemUpdateSoundMmlR
                ld      a, (hl)
                inc     hl
                sub     #'0'
                ld      ix, #_soundL
                add     ix, bc
                ld      0(ix), a
                jp      SystemUpdateSoundMml
            // 'R' : 休符
            SystemUpdateSoundMmlR:
                cp      #'R'
                jr      nz, SystemUpdateSoundMmlX
                ld      ix, #_soundTone
                add     ix, bc
                ld      0(ix), b
                ld      1(ix), b
                ld      ix, #_soundNoise
                add     ix, bc
                ld      0(ix), b
                jr      SystemUpdateSoundRest
            // 'X' : ノイズ
            SystemUpdateSoundMmlX:
                cp      #'X'
                jr      nz, SystemUpdateSoundMmlA
                ld      ix, #_soundTone
                add     ix, bc
                ld      0(ix), b
                ld      1(ix), b
                ld      ix, #_soundNoise
                add     ix, bc
                ld      0(ix), #0x01
                jr      SystemUpdateSoundRest
            // 'A' : 音符
            SystemUpdateSoundMmlA:
                sub     #'C'
                jr      nc, 20$
                    add     a, #0x07
                20$:
                add     a, a
                add     a, a
                ld      e, a
                ld      d, b
                ld      ix, #(_soundToneFrequencyTable + 0x04)
                add     ix, de
                ld      iy, #_soundO
                add     iy, bc
                ld      a, 0(iy)
                add     a, a
                add     a, a
                add     a, a
                add     a, a
                add     a, a
                ld      e, a
                add     ix, de
                ld      a, (hl)
                cp      #'+'
                jr      nz, 21$
                    inc     ix
                    inc     ix
                    inc     hl
                    jr      22$
                21$:
                cp      #'-'
                jr      nz, 22$
                    dec     ix
                    dec     ix
                    inc     hl
                22$:
                ld      iy, #_soundTone
                add     iy, bc
                ld      a, 0(ix)
                ld      0(iy), a
                ld      a, 1(ix)
                ld      1(iy), a
                ld      ix, #_soundNoise
                add     ix, bc
                ld      0(ix), b
                // jr      SystemUpdateSoundRest
            // 音の長さの設定
            SystemUpdateSoundRest:
                ld      a, (hl)
                cp      #('9' + 0x01)
                jr      nc, 30$
                sub     #'0'
                jr      c, 30$
                    inc     hl
                    jr      31$
                30$:
                    ld      ix, #_soundL
                    add     ix, bc
                    ld      a, 0(ix)
                31$:
                ld      e, a
                ld      d, b
                ld      ix, #_soundLengthTable
                add     ix, de
                ld      iy, #_soundT
                add     iy, bc
                ld      a, 0(iy)
                add     a, a
                add     a, a
                add     a, a
                add     a, a
                ld      e, a
                add     ix, de
                ld      iy, #_soundRest
                add     iy, bc
                ld      a, 0(ix)
                ld      0(iy), a
                ld      ix, #_soundUpdate
                add     ix, bc
                inc     0(ix)
                // 再生ポインタの保存
                ld      ix, #_soundPlay
                add     ix, bc
                ld      0(ix), l
                ld      1(ix), h
                // jr      SystemUpdateSoundNext
        // チャンネルの走査の完了
        SystemUpdateSoundNext:
            // 次のチャンネルへ
            inc     c
            inc     c
            ld      a, c
            cp      #0x08
            jp      nz, SystemUpdateSoundLoop
    // 設定の確認
    ld      iy, #_soundUpdate
    ld      a, 0(iy)
    or      2(iy)
    or      4(iy)
    or      6(iy)
    jp      z, SystemUpdateSoundEnd
    // エンベロープ周波数の設定
    ld      hl, #_soundM
    ld      e, (hl)
    ld      a, #0x0b
    call    WRTPSG
    inc     hl
    ld      e, (hl)
    ld      a, #0x0c
    call    WRTPSG
    // ノイズ周波数の設定
    ld      a, (_soundN)
    ld      e, a
    ld      a, #0x06
    call    WRTPSG
    // チャンネルの設定
    ld      a, (_soundMixing)
    ld      c, a
    ld      b, #0x00
    // チャンネルＡの設定
    ld      a, 0(iy)
    or      a
    jr      z, 49$
        ld      a, #0b00001001
        or      c
        ld      c, a
        ld      hl, (_soundTone + 0)
        ld      a, h
        or      l
        jr      z, 40$
            ld      a, #0b11111110
            and     c
            ld      c, a
            ld      e, l
            ld      a, #0x00
            call    WRTPSG
            ld      e, h
            ld      a, #0x01
            call    WRTPSG
            jr      41$
        40$:
            ld      a, (_soundNoise + 0)
            or      a
            jr      z, 42$
                ld      a, #0b11110111
                and     c
                ld      c, a
        41$: // else
            ld      a, (_soundV + 0)
        42$:
        ld      (_soundVolume + 0), a
        ld      e, a
        or      b
        ld      b, a
        ld      a, #0x08
        call    WRTPSG
    49$:
    // チャンネルＢの設定
    ld      a, 2(iy)
    or      a
    jr      z, 59$
        ld      a, #0b00010010
        or      c
        ld      c, a
        ld      hl, (_soundTone + 2)
        ld      a, h
        or      l
        jr      z, 50$
            ld      a, #0b11111101
            and     c
            ld      c, a
            ld      e, l
            ld      a, #0x02
            call    WRTPSG
            ld      e, h
            ld      a, #0x03
            call    WRTPSG
            jr      51$
        50$:// else if
            ld      a, (_soundNoise + 2)
            or      a
            jr      z, 52$
                ld      a, #0b11101111
                and     c
                ld      c, a
        51$:// else
            ld      a, (_soundV + 2)
        52$:// endif
        ld      (_soundVolume + 2), a
        ld      e, a
        or      b
        ld      b, a
        ld      a, #0x09
        call    WRTPSG
    59$:
    // チャンネルＤの設定
    ld      a, 6(iy)
    or      a
    jr      z, 69$
        ld      a, #0b00100100
        or      c
        ld      c, a
        ld      hl, (_soundTone + 6)
        ld      a, h
        or      l
        jr      z, 60$
            ld      a, #0b11111011
            and     c
            ld      c, a
            ld      e, l
            ld      a, #0x04
            call    WRTPSG
            ld      e, h
            ld      a, #0x05
            call    WRTPSG
            jr      61$
        60$:
            ld      a, (_soundNoise + 6)
            or      a
            jr      z, 62$
            ld      a, #0b11011111
            and     c
            ld      c, a
        61$:
            ld      a, (_soundV + 6)
        62$:
        ld      (_soundVolume + 6), a
        ld      e, a
        or      b
        ld      b, a
        ld      a, #0x0a
        call    WRTPSG
        jr      79$
    69$:
    // チャンネルＣの設定
    ld      a, 4(iy)
    or      a
    jr      z, 79$
        ld      a, #0b00100100
        or      c
        ld      c, a
        ld      hl, (_soundTone + 4)
        ld      a, h
        or      l
        jr      z, 70$
            ld      a, #0b11111011
            and     c
            ld      c, a
            ld      e, l
            ld      a, #0x04
            call    WRTPSG
            ld      e, h
            ld      a, #0x05
            call    WRTPSG
            jr      71$
        70$:
            ld      a, (_soundNoise + 4)
            or      a
            jr      z, 72$
                ld      a, #0b11011111
                and     c
                ld      c, a
        71$:
            ld      a, (_soundV + 4)
        72$:
        ld      (_soundVolume + 4), a
        ld      e, a
        or      b
        ld      b, a
        ld      a, #0x0a
        call    WRTPSG
    79$:
    // エンベロープ形状の設定
    ld      a, #0x10
    and     b
    jr      z, 80$
        ld      a, (_soundS)
        ld      e, a
        ld      a, #0x0d
        call    WRTPSG
    80$:
    // ミキシングの設定
    ld      a, c
    ld      (_soundMixing), a
    ld      e, a
    ld      a, #0x07
    call    WRTPSG
    // 設定の完了
    xor     a
    ld      0(iy), a
    ld      2(iy), a
    ld      4(iy), a
    ld      6(iy), a
    // 更新の終了
    SystemUpdateSoundEnd:
        // レジスタの復帰
        pop     iy
        pop     ix
        pop     de
        pop     bc
        pop     hl
        ret
    __endasm;
}
// サウンドを一時停止する
void SystemSuspendSound(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    // スリープの設定
    ld      hl, #_flag
    set     #FLAG_SOUND_SLEEP, (hl)
    // サウンドの停止
    ld      e, #0b10111111
    ld      a, #0x07
    call    WRTPSG
    // レジスタの復帰
    pop     hl
    ret
    __endasm;
}
// サウンドを再開する
void SystemResumeSound(void) __naked {
    __asm;
    // レジスタの保存
    push        hl
    // スリープの解除
    ld      hl, #_flag
    res     #FLAG_SOUND_SLEEP, (hl)
    // サウンドの更新
    ld      ix, #_soundUpdate
    ld      a, #0x01
    ld      0(ix), a
    ld      2(ix), a
    ld      4(ix), a
    ld      6(ix), a
    // レジスタの復帰
    pop     hl
    ret
    __endasm;
}
// サウンドのチャンネルをクリアする
static void SystemClearSoundChannel(void) __naked {
    __asm;
    // レジスタの保存
    push    af
    push    hl
    push    bc
    // チャンネルのクリア
    add     a, a
    ld      c, a
    ld      b, #0x00
    xor     a
    ld      hl, #_soundT
    add     hl, bc
    ld      (hl), a
    ld      hl, #_soundV
    add     hl, bc
    ld      (hl), #0x0f
    ld      hl, #_soundVolume
    add     hl, bc
    ld      (hl), #0x0f
    ld      hl, #_soundVminus
    add     hl, bc
    ld      (hl), a
    ld      hl, #_soundO
    add     hl, bc
    ld      (hl), #0x03
    ld      hl, #_soundL
    add     hl, bc
    ld      (hl), #0x05
    ld      hl, #_soundTone
    add     hl, bc
    ld      (hl), a
    inc     hl
    ld      (hl), a
    ld      hl, #_soundNoise
    add     hl, bc
    ld      (hl), a
    ld      hl, #_soundRest
    add     hl, bc
    ld      (hl), #0x01
    ld      hl, #_soundUpdate
    add     hl, bc
    ld      (hl), a
    // レジスタの復帰
    pop     bc
    pop     hl
    pop     af
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
static short const trigonometricTable[] = {
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
