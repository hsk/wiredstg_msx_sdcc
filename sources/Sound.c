#include "bios.h"
#include "vdp.h"
#include "System.h"
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
// ＳＥ
char const enemyBigCoreSeHit[] = "T1V15-L0O7A";
char const enemyBigCoreSeBomb[] =
    "T1V15L0"
    "O3GO2D-O3EO2D-O3CO2D-O2GD-ED-"
    "O2CO1D-O2D-O1CO2CO1D-O2D-O1CO2CO1D-O2D-O1CO2CO1D-O2D-O1C";
char const enemyBombSe[] = "T1V15L0O4GFEDCO3BAG";

// 操作
char const gamePlayBgm0[] =
    "T2V15-L1"
    //"O3G5G4O4CF3E5C3"
    //"O4G4O3B-B-6GA-B-O4E-FG"
    //"O4C4O3A4O4C5F3O3ABO4CD"
    //"O4E-3CO3ARO4E-3CF3DO3B-RO4DF3"
    //"O3G5G4O4CF3E5C3"
    //"O4G4O3B-B-6GA-B-O4E-FG"
    //"O4A3CC4F3G3A5F3"
    //"O4D4G8R"
    //"O5E-4C4E-6CDE-3"
    //"O5D4O4B-4G7R3"
    //"O5C4O4A4O5CCD4O4B4O5DD"
    //"O5E-4C4E-FF4D4G9R3"
    "O4F3DRO3B-O4DF3CE-3RO3AO4CE-3"
    "O4DCO3BAO4F3C5O3A4O4C4"
    "O4GFE-O3B-A-GB-6B-O4G4"
    "O4C3E5F3CO3G4G5"
    "O4G8RD4"
    "O4F3A5G3F3C4CA3"
    "O4GFE-O3B-A-GB-6B-O4G4"
    "O4C3E5F3CO3G4G5"
    "O5G9R3D4F4FE-C4E-4"
    "O5DDO4B4O5D4CCO4A4O5C4"
    "O4G7R3B-4O5D4"
    "O5E-3DCE-6C4E-4"
    "\xff";
char const gamePlayBgm1[] =
    "T2V14-L1"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CO4B-A-GFGA-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CDE-DCO4B-A-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CO4B-A-GFGA-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAF"
    //"O5DCO4BAGABO5CDCO4BAGBO5DG"
    //"V15-"
    //"O4C4E-4A-7R3"
    //"O3B-4O4D4E-7R3"
    //"O3AB-O4CF5RO3BO4CDG5R"
    //"O4CDE-A-5RDE-FB-4O5C7R3O4B7"
    "O4B-A-B-O5CDE-DCO4B-A-GFGA-B-O5C"
    "O4BAGFGABO5CO4BAGFGABO5C"
    "O4B-A-GFGA-B-O5CO4B-A-GFGA-B-O5C"
    "O4BAGFGABO5CO4BAGFGABO5C"
    "O5GDO4BGABO5CDCO4BAGABO5CD"
    "O4FAGFGABO5CO4BAGFGABO5C"
    "O4B-A-GFGA-B-O5CO4B-A-GFGA-B-O5C"
    "O4BAGFGABO5CO4BAGFGABO5C"
    "V15-"
    "O4B7O5C7R3O4B-4FE-DA-5RE-DC"
    "O4G5RDCO3BO4F5RCO3B-A"
    "O4E-7R3D4O3B-4"
    "O4A-7R3E-4C4"
    "\xff";
char const gamePlayBgm2[] =
    "T2V15-L1"
    //"O2C3O3CCO2C3O3CCO2C3O3CCO2C3O3CC"
    //"O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-"
    //"O2F3O3FFO2F3O3FFO2F3O3FFO2F3O3FF"
    //"O2A-3O3A-A-O2A-3O3A-A-O2A-3O3A-A-O2A-3O3A-A-"
    //"O2C3O3CCO2C3O3CCO2C3O3CCO2C3O3CC"
    //"O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-"
    //"O2F3O3FFO2F3O3FFO2F3O3FFO2F3O3FF"
    //"O2G3O3GGO2G3O3GGO2G3O3GGO2G3O3GG"
    //"O2A-O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-"
    //"O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-"
    //"O2FO3FCFO2FO3FCFO2GO3GDGO2GO3GDG"
    //"O2A-O3A-E-A-O2A-O3A-E-A-O2B-O3B-FB-O2B-O3B-FB-"
    //"O2G3O3GGO2G3O3GGO2G3O3GGO2G3O3GG"
    "O3A-A-O2A-3O3A-A-O2A-3O3A-A-O2A-3O3A-A-O2A-3"
    "O3FFO2F3O3FFO2F3O3FFO2F3O3FFO2F3"
    "O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3"
    "O3CCO2C3O3CCO2C3O3CCO2C3O3CCO2C3"
    "O3GGO2G3O3GGO2G3O3GGO2G3O3GGO2G3"
    "O3FFO2F3O3FFO2F3O3FFO2F3O3FFO2F3"
    "O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3"
    "O3CCO2C3O3CCO2C3O3CCO2C3O3CCO2C3"
    "O3GGO2G3O3GGO2G3O3GGO2G3O3GGO2G3"
    "O3B-FB-O2B-O3B-FB-O2B-O3A-E-A-O2A-O3A-E-A-O2A-"
    "O3GDGO2GO3GDGO2GO3FCFO2FO3FCFO2F"
    "O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-"
    "O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-O2A-"
    "\xff";
char const gameOverBgm0[] =
    "T2V15-L1"
    //"O4GECGECGECGEC"
    //"O5CO4GEO5CO4GEO5CO4GEO5CO4GE"
    //"O5ECO4GEGO5CGECO4GO5CE"
    //"O6CO5GEGECE6"
    "O5E6CEGEGO6C"
    "O5ECO4GO5CEGCO4GEGO5CE"
    "O4EGO5CO4EGO5CO4EGO5CO4EGO5C"
    "O4CEGCEGCEGCEG";
char const gameOverBgm1[] =
    "T2V15-L1"
    //"O4CO3GEO4CO3GEO4CO3GEO4CO3GE"
    //"O4ECO3GO4ECO3GO4ECO3GO4ECO3G"
    //"O4GECO3GO4CEO5CO4GECEG"
    //"O5ECO4GO5CO4GEG6"
    "O4G6EGO5CO4GO5CE"
    "O4GECEGO5CO4ECO3GO4CEG"
    "O3GO4CEO3GO4CEO3GO4CEO3GO4CE"
    "O3EGO4CO3EGO4CO3EGO4CO3EGO4C";
char const gameOverBgm2[] =
    "T2V15-L1"
    //"O2C5CCC4O1G3"
    //"O2E5EEE4C3"
    //"O2G5GGG4C3"
    //"O3C5O2CCC4R3"
    "O2C4R3CCO3C5"
    "O2C3G4GGG5"
    "O2C3E4EEE5"
    "O1G3O2C4CCC5";
char const shipSe[] =
    //"T1V15L0O4GO2D-O4EO2D-O4CO2D-O3GO2D-O3EO2D-O3CO2D-O2GD-ED-"
    "T1V15L0O3GO1D-O3EO1D-O3CO1D-O2GO1D-O2EO1D-O2CO1D-O1GD-ED-";
char const shotSe[] = "T1V13L0O6CO5F+O6CO5F+CO4F+";
// ジングル
char const titleJingle0[] = "T2V15-L1"/*"O4BO5DGBG5R7"*/"O5G5BGDO4BR7";
char const titleJingle1[] = "T2V15-L1"/*"O4DGBO5DO4B5R7"*/"O4B5O5DO4BGDR7";
char const titleJingle2[] = "T2V15-L1"/*"O3G4O4DO3G5R7"*/"O3G5O4DO3G4R7";
