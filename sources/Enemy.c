// Enemy.c : 敵
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
#include "string.h"
// 定数の定義
// ジェネレータ
static void EnemyNullGenerate(void);
typedef void(*FP)(void);
typedef void(*FP1)(char*);

static FP const enemyGenerateProc[] = {
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyRugalGenerate,
    EnemyRugalGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    0x0000,
    0x0000,
};
static char const enemyGenerateType[] = {
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_BACK,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_BACK,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_BACK,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DUCKER_UPPER,
    ENEMY_TYPE_DUCKER_UPPER,
    ENEMY_TYPE_DUCKER_LOWER,
    ENEMY_TYPE_DUCKER_LOWER,
};
// 更新
static void EnemyNullUpdate(char* ix);
static FP1 const enemyUpdateProc[] = {
    EnemyNullUpdate,
    EnemyBombUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyRugalUpdate,
    EnemyRugalUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
};
// 描画
static void EnemyNullRender(char*);
static void EnemyPutPattern16x16(char*);
static FP1 const enemyRenderProc[] = {
    EnemyNullRender,
    EnemyBombRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyPutPattern16x16,
    EnemyPutPattern16x16,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
};
// 変数の定義
char enemy[ENEMY_SIZE * ENEMY_N];// 敵
char enemyN;
char enemyGenerator[ENEMY_GENERATOR_SIZE];// ジェネレータ
char enemyCollision[0x0300];// コリジョン
// 敵を初期化する
void EnemyInitialize(void) {
    memset(enemy,0,ENEMY_SIZE * ENEMY_N);// 敵の初期化
    for(char b=ENEMY_N,*hl=enemy + ENEMY_INDEX,a=1;b;b--,hl+=ENEMY_SIZE,a++)
        *hl=a;
    enemyN = 7;
    // ジェネレータの初期化
    memset(enemyGenerator,0,ENEMY_GENERATOR_SIZE);
    enemyGenerator[ENEMY_GENERATOR_PHASE]=ENEMY_PHASE_NORMAL;
    // コリジョンの初期化
    memset(enemyCollision,0,0x0300);
    // パターンジェネレータの設定
    videoRegister[VDP_R4] = APP_PATTERN_GENERATOR_TABLE_1 >> 11;
    // ビデオレジスタの転送
    request |= 1<<REQUEST_VIDEO_REGISTER;
}
static void fpcall0(FP fp) {
    fp;
    __asm;
    push ix
    push iy
    ld de,#11$
    push de
    jp (hl)
    11$:
    pop iy
    pop ix
    ret
    __endasm;
}
static void fpcall1(FP fp,char* ix) {
    ix;fp;
    __asm;
    push iy// todo
    push ix
    ex de,hl
    push de
    pop ix
    ld de,#11$
    push de
    jp (ix)
    11$:
    pop ix
    pop iy
    ret
    __endasm;
}
static void fpcall1b(FP fp,char* ix) {
    ix;fp;
    __asm;
    //push ix
    push iy
    ex de,hl
    push de
    pop iy
    //push hl
    //pop ix
    ld de,#11$
    push de
    jp (iy)
    11$:

    pop iy
    //pop ix
    ret
    __endasm;
}
// 敵を更新する
void EnemyUpdate(void) {
    // コリジョンの初期化
    memset(enemyCollision,0,0x300);
    fpcall0(enemyGenerateProc[enemyGenerator[ENEMY_GENERATOR_TYPE]]);
    for(char b=enemyN,*ix=enemy;b;b--,ix+=ENEMY_SIZE) {// 敵の更新
        fpcall1b(enemyUpdateProc[ix[ENEMY_TYPE]],ix);
    }
}
// 敵を描画する
void EnemyRender(void) {
    memset(appPatternName,0,0x300);
    // 種類別の描画
    for(char b=enemyN,*ix=enemy;b;b--,ix+=ENEMY_SIZE) {
        fpcall1(enemyRenderProc[ix[ENEMY_TYPE]],ix);
    }
}
// ENEMY_TYPE_NULL を生成する
static void EnemyNullGenerate(void) {
    // ジェネレータの取得
    char *iy = (void*)enemyGenerator;
    // 通常戦の開始
    char a = iy[ENEMY_GENERATOR_PHASE];
    if (a==ENEMY_PHASE_NORMAL) {
        // 通常戦の初期化
        a = iy[ENEMY_GENERATOR_STATE];
        if (a==0) {
            a=8;
            iy[ENEMY_GENERATOR_TIMER]=a;
            iy[ENEMY_GENERATOR_STATE]++;
        }
        // タイマの更新
        iy[ENEMY_GENERATOR_TIMER]--;
        if(iy[ENEMY_GENERATOR_TIMER]) return;
        // 敵の生成
        iy[ENEMY_GENERATOR_STATE] = 0;
        iy[ENEMY_GENERATOR_TYPE] = enemyGenerateType[(SystemGetRandom()>>2)&0x1f];
    }
}
// ENEMY_TYPE_NULL を更新する
static void EnemyNullUpdate(char* ix) {
    ix;
}
// ENEMY_TYPE_NULL を描画する
static void EnemyNullRender(char* ix) {
    ix;
}
// 空の敵を取得する
char* EnemyGetEmpty(void) {
    for(char b=enemyN,*ix=enemy;b;b--,ix+=ENEMY_SIZE)
        if (!ix[ENEMY_TYPE]) return ix;
    return NULL;
}
// 16x16 サイズのパターンを置く
static void EnemyPutPattern16x16(char* ix) {
    // クリッピングの取得
    char c = 0b00001111;
    if (ix[ENEMY_POSITION_Y]<8) c &= ~((1<<0)|(1<<1));
    if (ix[ENEMY_POSITION_Y]>=0xc0) c &= ~((1<<2)|(1<<3));
    if (ix[ENEMY_POSITION_X]<8) c &= ~((1<<0)|(1<<2));

    // パターンを置く
    short de = (ix[ENEMY_POSITION_Y]&0xf8)*4 + (ix[ENEMY_POSITION_X] >> 3);
    char* hl = appPatternName - 0x0021 + de;
    char* iy = enemyCollision - 0x0021 + de;
    char a = ix[ENEMY_ANIMATION];
    char b = ix[ENEMY_INDEX];
    if (c&1) { *hl = a; *iy = b; } c=c>>1;
    hl++;iy++;a++;
    if (c&1) { *hl = a; *iy = b; } c=c>>1;
    hl+= 0x1f; iy+= 0x1f; a+= 0xf;
    if (c&1) { *hl = a; *iy = b; } c=c>>1;
    hl++;iy++;a++;
    if (c&1) { *hl = a; *iy = b; }
}
