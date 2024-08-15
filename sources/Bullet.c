// Bullet.c : 弾
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Bullet.h"
#include "string.h"
// 変数の定義
char bullet[BULLET_SIZE * BULLET_N];// 弾
char bulletN;
static char bulletTimer;// タイマ
// 弾を初期化する
void BulletInitialize(void) {
    // 弾の初期化
    memset(bullet,0,BULLET_SIZE * BULLET_N);
    bulletN = 7;
    bulletTimer = 0;// タイマの初期化
}
static char* get_bullet(void) {
    // 空の取得
    for(char b = bulletN, *ix = bullet;b;b--,ix+=BULLET_SIZE)
        if (ix[BULLET_STATE]==0) return ix;
    return 0;
}
char gen1(short hl,char* ix) {
    char h = hl>>8,l=hl&255;
    // 生成の開始
    ix[BULLET_POSITION_XD] = 0;
    ix[BULLET_POSITION_XI] = h;
    ix[BULLET_POSITION_YD] = 0;
    ix[BULLET_POSITION_YI] = l;
    h = (ship[SHIP_POSITION_X]>>1)-(h>>1);
    l = (ship[SHIP_POSITION_Y]>>1)-(l>>1);
    return SystemGetAtan2((h<<8)|l);
}
void gen2(char a,char* ix) {
    if (a < 0x40) {
        *(short*)&ix[BULLET_SPEED_YD] = SystemGetSin(a);
        *(short*)&ix[BULLET_SPEED_XD] = SystemGetCos(a);
        ix[BULLET_STATE] = 1;
    } else if (a < 0x80) {
        *(short*)&ix[BULLET_SPEED_YD] = SystemGetSin(a);
        a = 0x80-a;
        *(short*)&ix[BULLET_SPEED_XD] = SystemGetCos(a);
        ix[BULLET_STATE] = 2;
    } else if (a < 0xc0) {
        a -= 0x80;
        *(short*)&ix[BULLET_SPEED_YD] = SystemGetSin(a);
        *(short*)&ix[BULLET_SPEED_XD] = SystemGetCos(a);
        ix[BULLET_STATE] = 3;
    } else {
        a = -a;
        *(short*)&ix[BULLET_SPEED_YD] = SystemGetSin(a);
        *(short*)&ix[BULLET_SPEED_XD] = SystemGetCos(a);
        ix[BULLET_STATE] = 4;
    }
}
void gen9(char* ix) {
    *(short*)&ix[BULLET_SPEED_XD] = (*(short*)&ix[BULLET_SPEED_XD])*2;
    *(short*)&ix[BULLET_SPEED_YD] = (*(short*)&ix[BULLET_SPEED_YD])*2;
}
// 弾を生成する
void BulletGenerate(short hl) {
    if (ship[SHIP_TYPE] != SHIP_TYPE_VICVIPER) return;// 自機の存在
    // 距離の取得
    char a = ship[SHIP_POSITION_X]-(hl>>8);
    if (a < 0x10 || a >= 0xf0) return;
    a = ship[SHIP_POSITION_Y] - (hl&255);
    if (a < 0x10 || a >= 0xf0) return;
    char* ix = get_bullet();// 空の取得
    if (ix==0) return;
    gen2(gen1(hl,ix),ix);
    gen9(ix);
}
// 弾を更新する
void BulletUpdate(void) {
    for(char *ix = bullet, b = bulletN;b;ix += BULLET_SIZE,b--) {// 弾の走査
        char a = ix[BULLET_STATE];
        if (a == 0) continue;
        // 弾の移動
        if (--a == 0) {
            u16 hl = *(u16*)&ix[BULLET_POSITION_XD];
            u16 de = *(u16*)&ix[BULLET_SPEED_XD];
            if ((hl>>1)+(de>>1) >= 0x8000-2) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_XD] = hl+de;
            hl = *(u16*)&ix[BULLET_POSITION_YD];
            de = *(u16*)&ix[BULLET_SPEED_YD];
            if ((hl>>1)+(de>>1) >= 0x8000-2) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_YD] = hl+de;
            continue;
        }
        if (--a == 0) {
            u16 hl = *(u16*)&ix[BULLET_POSITION_XD];
            u16 de = *(u16*)&ix[BULLET_SPEED_XD];
            if (hl<de) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_XD] = hl-de;
            hl = *(u16*)&ix[BULLET_POSITION_YD];
            de = *(u16*)&ix[BULLET_SPEED_YD];
            if ((hl>>1)+(de>>1) >= 0x8000-2) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_YD] = hl+de;
            continue;
        }
        if (--a == 0) {
            u16 hl = *(u16*)&ix[BULLET_POSITION_XD];
            u16 de = *(u16*)&ix[BULLET_SPEED_XD];
            if (hl<de) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_XD] = hl-de;
            hl = *(u16*)&ix[BULLET_POSITION_YD];
            de = *(u16*)&ix[BULLET_SPEED_YD];
            if (hl<de) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_YD] = hl-de;
            continue;
        }
        {
            u16 hl = *(u16*)&ix[BULLET_POSITION_XD];
            u16 de = *(u16*)&ix[BULLET_SPEED_XD];
            if ((hl>>1)+(de>>1) >= 0x8000-2) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_XD] = hl+de;
            hl = *(u16*)&ix[BULLET_POSITION_YD];
            de = *(u16*)&ix[BULLET_SPEED_YD];
            if (hl<de) {
                ix[BULLET_STATE] = 0;// 弾の削除
                continue;
            }
            *(u16*)&ix[BULLET_POSITION_YD] = hl-de;
            continue;
        }
    }
    bulletTimer++;// タイマの更新
}
// 弾を描画する
void BulletRender(void) {
    // スプライトの描画
    char* hl = sprite + GAME_SPRITE_BULLET;
    char c = (bulletTimer & 0x0f)<<2;
    for (char b = bulletN,*ix=bullet;b;ix+=BULLET_SIZE,b--) {
        if (ix[BULLET_STATE]) {
            char* hl = sprite + GAME_SPRITE_BULLET + c;
            *hl++ = ix[BULLET_POSITION_YI] - 0x09;
            *hl++ = ix[BULLET_POSITION_XI] - 0x08;
            *hl++ = 0x3c;
            *hl++ = appColor;
            c = (c+0x04)&0x3f;
        }
    }
}

