// Star.c : 星
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Star.h"
// 星
#define STAR_POSITION_X 0x00
#define STAR_SPEED      0x01
#define STAR_SIZE       0x02
#define STAR_N          0x15
// 変数の定義
static char star[STAR_SIZE * STAR_N];// 星
// 星を初期化する
void StarInitialize(void) {
    // 星の初期化
    for (char *ix=star,b=STAR_N;b;b--,ix+=STAR_SIZE) {
        ix[STAR_POSITION_X] = SystemGetRandom();
        ix[STAR_SPEED] = (SystemGetRandom() & 3)+1;
    }
}
// 星を更新する
void StarUpdate(void) {
    for(char *ix=star,b=STAR_N;b;b--,ix+=STAR_SIZE) {
        ix[STAR_POSITION_X] += ix[STAR_SPEED];
        if(ix[STAR_POSITION_X] < ix[STAR_SPEED]) {
            ix[STAR_SPEED] = (SystemGetRandom()&3)+1;
        }
    }
}    
// 星を描画する
void StarRender(void) {
    // パターンネームの描画
    for(char* ix=star,*hl=appPatternName+0x40,b=STAR_N;b;b--,ix += STAR_SIZE) {
        char e = (ix[STAR_POSITION_X] >> 3) & 0x1f;
        hl += e;
        if (*hl == 0) *hl = (ix[STAR_POSITION_X]&7)+0x58;
        hl += -e + 0x20; 
    }
}
