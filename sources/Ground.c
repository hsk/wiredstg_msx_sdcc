// Ground.c : 地面
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ground.h"
#include "string.h"
// 変数の定義
char ground[0x0300];// 地面
char groundGenerator[GROUND_GENERATOR_SIZE];// ジェネレータ
static char groundGeneratorRow[0x18];
// 地面を初期化する
void GroundInitialize(void) {
    memset(ground,0,0x300);// 地面の初期化
    // ジェネレータの初期化
    groundGenerator[GROUND_GENERATOR_UPPER_STATE] = 0;
    groundGenerator[GROUND_GENERATOR_LOWER_STATE] = 0;
    groundGenerator[GROUND_GENERATOR_UPPER_HEIGHT] = 0;
    groundGenerator[GROUND_GENERATOR_LOWER_HEIGHT] = 0;
    groundGenerator[GROUND_GENERATOR_UPPER_LENGTH] = 1;
    groundGenerator[GROUND_GENERATOR_LOWER_LENGTH] = 1;
}
static void genup1(void) {
    char* ix = groundGenerator;
    if (--ix[GROUND_GENERATOR_UPPER_LENGTH]) return;
    groundGeneratorRow[1]=0x4a;
    ix[GROUND_GENERATOR_UPPER_HEIGHT]++;
    ix[GROUND_GENERATOR_UPPER_LENGTH]=(SystemGetRandom()&0x1f) + 0x11;
    ix[GROUND_GENERATOR_UPPER_STATE] = GROUND_GENERATOR_STATE_FLAT;
}
static void genup2(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 1;
    for(char b= ix[GROUND_GENERATOR_UPPER_HEIGHT]-1;b;b--) *hl++ = 0x4f;
    *hl = 0x48;
    if(--ix[GROUND_GENERATOR_UPPER_LENGTH]) return;
    ix[GROUND_GENERATOR_UPPER_LENGTH] = (SystemGetRandom()&7)+1;
    ix[GROUND_GENERATOR_UPPER_STATE] = GROUND_GENERATOR_STATE_UP;
}
static void genup3(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 1;
    for(char b = ix[GROUND_GENERATOR_UPPER_HEIGHT];b;b--) *hl++ = 0x4f;
    *hl = 0x4a;
    ix[GROUND_GENERATOR_UPPER_HEIGHT]++;
    if(--ix[GROUND_GENERATOR_UPPER_LENGTH]) return;
    ix[GROUND_GENERATOR_UPPER_LENGTH] = ix[GROUND_GENERATOR_UPPER_HEIGHT] - 1;
    ix[GROUND_GENERATOR_UPPER_STATE] = GROUND_GENERATOR_STATE_DOWN;
}
static void genup4(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 1;
    for(char b = ix[GROUND_GENERATOR_UPPER_HEIGHT]-1;b;b--) *hl++ = 0x4f;
    *hl = 0x4b;
    ix[GROUND_GENERATOR_UPPER_HEIGHT]--;
    if(--ix[GROUND_GENERATOR_UPPER_LENGTH]) return;
    ix[GROUND_GENERATOR_UPPER_LENGTH] = (SystemGetRandom()&0x1f)+0x11;
    ix[GROUND_GENERATOR_UPPER_STATE] = GROUND_GENERATOR_STATE_FLAT;
}
static void genup(void) {
    // ↑の生成
    char a = groundGenerator[GROUND_GENERATOR_UPPER_STATE];
    if (a == 0)                                genup1();// 地面なし／↑
    else if (a == GROUND_GENERATOR_STATE_FLAT) genup2();// 平地／↑
    else if (a == GROUND_GENERATOR_STATE_UP)   genup3();// 上る／↑
    else                                       genup4();// 下る／↑
}
static void down1(void) {
    char* ix = groundGenerator;
    if (--ix[GROUND_GENERATOR_LOWER_LENGTH]) return;
    groundGeneratorRow[0x17]=0x4b;
    ix[GROUND_GENERATOR_LOWER_HEIGHT]++;
    ix[GROUND_GENERATOR_LOWER_LENGTH]=(SystemGetRandom()&0x1f) + 0x11;
    ix[GROUND_GENERATOR_LOWER_STATE] = GROUND_GENERATOR_STATE_FLAT;
}
static void down2(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 0x17;
    for(char b= ix[GROUND_GENERATOR_LOWER_HEIGHT]-1;b;b--) *hl++ = 0x4f;
    *hl = 0x49;
    if(--ix[GROUND_GENERATOR_LOWER_LENGTH]) return;
    ix[GROUND_GENERATOR_LOWER_LENGTH] = (SystemGetRandom()&7)+1;
    ix[GROUND_GENERATOR_LOWER_STATE] = GROUND_GENERATOR_STATE_UP;
}
static void down3(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 0x17;
    for(char b = ix[GROUND_GENERATOR_LOWER_HEIGHT];b;b--) *hl-- = 0x4f;
    *hl = 0x4b;
    ix[GROUND_GENERATOR_LOWER_HEIGHT]++;
    if(--ix[GROUND_GENERATOR_LOWER_LENGTH]) return;
    ix[GROUND_GENERATOR_LOWER_LENGTH] = ix[GROUND_GENERATOR_LOWER_HEIGHT] - 1;
    ix[GROUND_GENERATOR_LOWER_STATE] = GROUND_GENERATOR_STATE_DOWN;
}
static void down4(void) {
    char* ix = groundGenerator;
    char* hl = groundGeneratorRow + 0x17;
    for(char b = ix[GROUND_GENERATOR_LOWER_HEIGHT]-1;b;b--) *hl-- = 0x4f;
    *hl = 0x4a;
    ix[GROUND_GENERATOR_LOWER_HEIGHT]--;
    if(--ix[GROUND_GENERATOR_LOWER_LENGTH]) return;
    ix[GROUND_GENERATOR_LOWER_LENGTH] = (SystemGetRandom()&0x1f)+0x8;
    ix[GROUND_GENERATOR_LOWER_STATE] = GROUND_GENERATOR_STATE_FLAT;
}
static void gendown(void) {
    char a = groundGenerator[GROUND_GENERATOR_LOWER_STATE];
    if (a == 0)                                down1(); // 地面なし／↓
    else if (a == GROUND_GENERATOR_STATE_FLAT) down2(); // 平地／↓
    else if (a == GROUND_GENERATOR_STATE_UP)   down3(); // 上る／↓
    else                                       down4(); // 下る／↓
}
static void scroll1(void) {
    __asm;
    // １キャラ分のスクロール
    ld      hl, #(_ground + 0x02fe)
    ld      de, #(_ground + 0x02ff)
    ld      bc, #0x02ff
    lddr
    __endasm;
}
// 地面を更新する
void GroundUpdate(void) {
    // スクロールの開始
    if (gameScroll) return;
    scroll1();
    memset(groundGeneratorRow,0,0x018);// 生成する列の初期化
    genup();
    gendown();
    for(char* hl=groundGeneratorRow+1,*ix=ground+0x20,b=0x17;b;ix+=0x20,hl++,b--)
        *ix=*hl;
}
// 地面を描画する
void GroundRender(void) {
    memcpy(appPatternName,ground,0x300);// パターンネームの描画
}
