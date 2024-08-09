// EnemyBeam.c : 敵／ビーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Enemy.h"
// 敵を生成する
void EnemyBeamGenerate(short hl) {
    // 敵の生成
    char *ix = EnemyGetEmpty();
    if (ix==0) return;
    ix[ENEMY_TYPE] = ENEMY_TYPE_BEAM;
    ix[ENEMY_STATE] = 0;
    ix[ENEMY_POSITION_X] = (((short)hl)>>8)+0x18;
    ix[ENEMY_POSITION_Y] = (((short)hl)&255)+0xe8;
    ix[ENEMY_HP] = 0xff;
}
// 敵を更新する
void EnemyBeamUpdate(char* ix) {
    // 初期化の開始
    if (ix[ENEMY_STATE]==0) {
        ix[ENEMY_SHOT] = 0;// ショットの設定
        ix[ENEMY_STATE]++;// 初期化の完了
    }
    ix[ENEMY_HP] = 0xff;// ＨＰの再設定
    
    if (ix[ENEMY_SHOT]<4) {// ショットの更新
        ix[ENEMY_SHOT]++;
        return;
    }
    // 移動
    short x = ((short)ix[ENEMY_POSITION_X]) + 8;
    if (x < 256) ix[ENEMY_POSITION_X] = x;
    else         ix[ENEMY_TYPE] = 0;// 敵の削除
}
// 敵を描画する
void EnemyBeamRender(char* ix) {
    // 位置の取得
    *(short*)&ix[ENEMY_PARAM_0] = (((short)(ix[ENEMY_POSITION_Y]&0xf8))<<2)+(ix[ENEMY_POSITION_X]>>3);
    // 長さの取得
    {
        char e = (ix[ENEMY_POSITION_X]>>3);
        char a1 = 0x20-e;
        if (4<=a1) a1 = ix[ENEMY_SHOT];
        ix[ENEMY_PARAM_2] = a1;
        short a = 0x1d-e;
        if (a < 0) {
            a = 0;
        } else if (a >= 4) {
            a = ix[ENEMY_SHOT];
        }
        ix[ENEMY_PARAM_3] = a;
    }
    // パターンを置く／外側
    {
        short bc = *(short*)&ix[ENEMY_PARAM_0];
        char* de = appPatternName + bc;
        char* hl = enemyCollision + bc;
        for(char b = ix[ENEMY_PARAM_2];b;b--) {
            *de++=0xb4;
            *hl++=ix[ENEMY_INDEX];
        }
        char c = 0xc0-ix[ENEMY_PARAM_2];
        hl += c;
        de += c;
        for(char b = ix[ENEMY_PARAM_2];b;b--) {
            *de++ = 0xb4;
            *hl++ = ix[ENEMY_INDEX];
        }
    }
    // パターンを置く／内側
    if (ix[ENEMY_PARAM_3]) {
        short bc = *(short*)&ix[ENEMY_PARAM_0];
        char *de = appPatternName + 0x0043 + bc;
        char *hl = enemyCollision + 0x0043 + bc;
        char a = ix[ENEMY_INDEX];
        for(char b = ix[ENEMY_PARAM_3];b;b--){
            *de++ = 0xb4;
            *hl++ = a;
        }
        char c = #0x40 - ix[ENEMY_PARAM_3];
        hl += c;
        de += c;
        for(char b = ix[ENEMY_PARAM_3];b;b--){
            *de++ = 0xb4;
            *hl++ = a;
        }
    }
}
