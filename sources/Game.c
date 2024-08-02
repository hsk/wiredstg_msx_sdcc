// Game.c : ゲーム
#include "bios.h"
#include "vdp.h"
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Ground.h"
#include "Star.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
#define GAME_STATE_OVER 0x20
// 定数の定義
// スコア
static char const gameScoreString[] = {
    0x11, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x28, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
// 速度
static char const gameSpeedString[] = {
    0x00, 0x00,
    0x00, 0x46,
    0x00, 0x47,
    0x46, 0x47,
    0x47, 0x47,
};
// 操作
static char const gamePlayBgm0[] =
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
static char const gamePlayBgm1[] =
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
static char const gamePlayBgm2[] =
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
// ゲームオーバー
static char const gameOverString[] = {
    0x27, 0x21, 0x2d, 0x25, 0x00, 0x00, 0x2f, 0x36, 0x25, 0x32,
};
static char const gameOverBgm0[] =
    "T2V15-L1"
    //"O4GECGECGECGEC"
    //"O5CO4GEO5CO4GEO5CO4GEO5CO4GE"
    //"O5ECO4GEGO5CGECO4GO5CE"
    //"O6CO5GEGECE6"
    "O5E6CEGEGO6C"
    "O5ECO4GO5CEGCO4GEGO5CE"
    "O4EGO5CO4EGO5CO4EGO5CO4EGO5C"
    "O4CEGCEGCEGCEG";
static char const gameOverBgm1[] =
    "T2V15-L1"
    //"O4CO3GEO4CO3GEO4CO3GEO4CO3GE"
    //"O4ECO3GO4ECO3GO4ECO3GO4ECO3G"
    //"O4GECO3GO4CEO5CO4GECEG"
    //"O5ECO4GO5CO4GEG6"
    "O4G6EGO5CO4GO5CE"
    "O4GECEGO5CO4ECO3GO4CEG"
    "O3GO4CEO3GO4CEO3GO4CEO3GO4CE"
    "O3EGO4CO3EGO4CO3EGO4CO3EGO4C";
static char const gameOverBgm2[] =
    "T2V15-L1"
    //"O2C5CCC4O1G3"
    //"O2E5EEE4C3"
    //"O2G5GGG4C3"
    //"O3C5O2CCC4R3"
    "O2C4R3CCO3C5"
    "O2C3G4GGG5"
    "O2C3E4EEE5"
    "O1G3O2C4CCC5";
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
static char gameScore[6];// スコア
static char gameScorePlus;
char gameScroll;// スクロール
// ゲームを初期化する
void GameInitialize(void) __naked {
    __asm;
    // ゲームの初期化
    // 地面の初期化
    call    _GroundInitialize
    // 星の初期化
    call    _StarInitialize
    // 自機の初期化
    call    _ShipInitialize
    // ショットの初期化
    call    _ShotInitialize
    // 敵の初期化
    call    _EnemyInitialize
    // 弾の初期化
    call    _BulletInitialize
    // 一時停止の初期化
    xor     a
    ld      (_gamePause), a
    // スコアの初期化
    ld      hl, #(_gameScore + 0x0000)
    ld      de, #(_gameScore + 0x0001)
    ld      bc, #0x0005
    xor     a
    ld      (hl), a
    ldir
    ld      (_gameScorePlus), a
    // スクロールの初期化
    xor     a
    ld      (_gameScroll), a
    // パターンのクリア
    ld      hl, #(_appPatternName + 0x0000)
    ld      de, #(_appPatternName + 0x0001)
    ld      bc, #0x02ff
    xor     a
    ld      (hl), a
    ldir
    // パターンネームの転送
    call    _AppTransferPatternName
    //// 描画の開始
    //ld      hl, #(_videoRegister + VDP_R1)
    //set     #VDP_R1_BL, (hl)
    //// ビデオレジスタの転送
    //ld      hl, #_request
    //set     #REQUEST_VIDEO_REGISTER, (hl)
    // 状態の設定
    ld      a, #GAME_STATE_PLAY
    ld      (_gameState), a
    ld      a, #APP_STATE_GAME_UPDATE
    ld      (_appState), a
    ret
    __endasm;
}
static void GamePlay(void);
static void GameOver(void);
static void GameHitCheck(void);
static void GameUpdateScore(void);
// ゲームを更新する
void GameUpdate(void) __naked {
    __asm;
    // レジスタの保存
    push    hl
    push    bc
    push    de
    push    ix
    push    iy
    // ESC キーで一時停止
    ld      a, (_input + INPUT_BUTTON_ESC)
    cp      #0x01
    jr      nz, 09$
    ld      hl, #_gamePause
    sub     (hl)
    ld      (hl), a
    or      a
    jr      z, 00$
        call    _SystemSuspendSound
        jr      09$
    00$:
        call    _SystemResumeSound
        //jr      09$
    09$:
    ld      a, (_gamePause)
    or      a
    jr      nz, 99$
        // 乱数の更新
        call    _SystemGetRandom
        // 状態別の処理
        ld      a, (_gameState)
        and     #0xf0
        cp      #GAME_STATE_PLAY
        jr      nz, 10$
            call    _GamePlay
            jr      19$
        10$:
            //cp      #GAME_STATE_OVER
            //jr      nz, 19$
            call    _GameOver
            //jr      19$
        19$:
        // 更新の完了
    99$:
    // レジスタの復帰
    pop     iy
    pop     ix
    pop     de
    pop     bc
    pop     hl
    ret
    __endasm;
}    
// ゲームをプレイする
static void GamePlay(void) __naked {
    __asm;
    // 初期化の開始
    ld      a, (_gameState)
    and     #0x0f
    jr      nz, 09$
        // 描画の開始
        ld      hl, #(_videoRegister + VDP_R1)
        set     #VDP_R1_BL, (hl)
        // ビデオレジスタの転送
        ld      hl, #_request
        set     #REQUEST_VIDEO_REGISTER, (hl)
        // ＢＧＭの再生
        ld      hl, #_gamePlayBgm0
        ld      (_soundRequest + 0x0000), hl
        ld      hl, #_gamePlayBgm1
        ld      (_soundRequest + 0x0002), hl
        ld      hl, #_gamePlayBgm2
        ld      (_soundRequest + 0x0004), hl
        // 初期化の完了
        ld      hl, #_gameState
        inc     (hl)
    09$:
    // スプライトのクリア
    call    _SystemClearSprite
    // 加算されるスコアのクリア
    xor     a
    ld      (_gameScorePlus), a
    // スクロールの更新
    ld      hl, #_gameScroll
    ld      a, (hl)
    inc     a
    and     #0x0f
    ld      (hl), a
    // ヒットチェック
    call    _GameHitCheck
    // 地面の更新
    call    _GroundUpdate
    // 星の更新
    call    _StarUpdate
    // 自機の更新
    call    _ShipUpdate
    // ショットの更新
    call    _ShotUpdate
    // 敵の更新
    call    _EnemyUpdate
    // 弾の更新
    call    _BulletUpdate
    // 地面の描画
    call    _GroundRender
    // 星の描画
    call    _StarRender
    // 自機の描画
    call    _ShipRender
    // ショットの描画
    call    _ShotRender
    // 敵の描画
    call    _EnemyRender
    // 弾の描画
    call    _BulletRender
    // スコアの更新
    call    _GameUpdateScore
    // パターンネームの転送
    call    _AppTransferPatternName
    // ゲームオーバーの条件
    ld      a, (_ship + SHIP_TYPE)
    or      a
    jr      nz, 19$
        // ゲームオーバー
        ld      a, #GAME_STATE_OVER
        ld      (_gameState), a
    19$:
    ret
    __endasm;
}
// ゲームオーバーになる
static void GameOver(void) __naked {
    __asm;
    // 初期化の開始
    ld      a, (_gameState)
    and     #0x0f
    jr      nz, 09$
        // ゲームオーバーの表示
        ld      hl, #_gameOverString
        ld      de, #(_appPatternName + 0x016b)
        ld      bc, #0x000a
        ldir
        // パターンネームの転送
        call    _AppTransferPatternName
        // ＢＧＭの再生
        ld      hl, #_gameOverBgm0
        ld      (_soundRequest + 0x0000), hl
        ld      hl, #_gameOverBgm1
        ld      (_soundRequest + 0x0002), hl
        ld      hl, #_gameOverBgm2
        ld      (_soundRequest + 0x0004), hl
        // 初期化の完了
        ld      hl, #_gameState
        inc     (hl)
    09$:
    // ＢＧＭの監視
    ld      hl, (_soundRequest + 0x0000)
    ld      a, h
    or      l
    ret     nz
    ld      hl, (_soundPlay + 0x0000)
    ld      a, h
    or      l
    ret     nz
    // タイトルへ戻る
    ld      a, #APP_STATE_TITLE_INITIALIZE
    ld      (_appState), a
    ret
    __endasm;
}
// ヒットチェックを行う
static void GameHitCheck(void) __naked {
    __asm;
    // ショットのチェック
    ld      ix, #_shot
    ld      bc, #(SHOT_N << 8)
    10$:
        ld      a, SHOT_STATE(ix)
        or      a
        jr      z, 19$
        ld      a, SHOT_POSITION_Y(ix)
        and     #0xf8
        ld      d, #0x00
        add     a, a
        rl      d
        add     a, a
        rl      d
        ld      e, ENEMY_POSITION_X(ix)
        srl     e
        srl     e
        srl     e
        add     a, e
        ld      e, a
        ld      hl, #_enemyCollision
        add     hl, de
        ld      a, (hl)
        or      a
        jr      z, 11$
            dec     a
            add     a, a
            add     a, a
            add     a, a
            add     a, a
            ld      e, a
            ld      d, #0x00
            ld      iy, #_enemy
            add     iy, de
            dec     ENEMY_HP(iy)
            jr      nz, 18$
            ld      a, #ENEMY_TYPE_BOMB
            ld      ENEMY_TYPE(iy), a
            xor     a
            ld      ENEMY_STATE(iy), a
            inc     c
            jr      18$
        11$:
            ld      hl, #_ground
            add     hl, de
            ld      a, (hl)
            or      a
            jr      z, 19$
        18$:
            xor     a
            ld      SHOT_STATE(ix), a
        19$:
        ld      de, #SHOT_SIZE
        add     ix, de
    djnz    10$
    // 加算されるスコアの設定
    ld      a, c
    ld      (_gameScorePlus), a
    // 弾のチェック
    ld      ix, #_bullet
    ld      iy, #_ship
    ld      a, (_bulletN)
    ld      b, a
    20$:
        ld      a, BULLET_STATE(ix)
        or      a
        jr      z, 29$
        ld      a, BULLET_POSITION_YI(ix)
        and     #0xf8
        ld      d, #0x00
        add     a, a
        rl      d
        add     a, a
        rl      d
        ld      e, BULLET_POSITION_XI(ix)
        srl     e
        srl     e
        srl     e
        add     a, e
        ld      e, a
        ld      hl, #_ground
        add     hl, de
        ld      a, (hl)
        or      a
        jr      nz, 28$
        ld      a, SHIP_TYPE(iy)
        cp      #SHIP_TYPE_VICVIPER
        jr      nz, 29$
        ld      a, SHIP_POSITION_X(iy)
        sub     BULLET_POSITION_XI(ix)
        jr      c, 21$
            cp      #0x04
            jr      nc, 29$
            jr      22$
        21$:
            cp      #0xfc
            jr      c, 29$
        22$:
            ld      a, SHIP_POSITION_Y(iy)
            sub     BULLET_POSITION_YI(ix)
            jr      c, 23$
            cp      #0x04
            jr      nc, 29$
            jr      24$
        23$:
            cp      #0xfc
            jr      c, 29$
        24$:
            //dec     SHIP_HP(iy)
            //jr      nz, 28$
            jr      28$ // debug
            ld      a, #SHIP_TYPE_BOMB
            ld      SHIP_TYPE(iy), a
            xor     a
            ld      SHIP_STATE(iy), a
        28$:
            xor     a
            ld      BULLET_STATE(ix), a
        29$:
        ld      de, #SHOT_SIZE
        add     ix, de
    djnz    20$
    // 自機のチェック
    ld      ix, #_ship
    ld      a, SHIP_TYPE(ix)
    cp      #SHIP_TYPE_VICVIPER
    jr      nz, 39$
    ld      a, SHIP_POSITION_Y(ix)
    and     #0xf8
    ld      d, #0x00
    add     a, a
    rl      d
    add     a, a
    rl      d
    ld      e, SHIP_POSITION_X(ix)
    srl     e
    srl     e
    srl     e
    add     a, e
    ld      e, a
    ld      hl, #_enemyCollision
    add     hl, de
    ld      a, (hl)
    or      a
    jr      nz, 38$
        ld      hl, #_ground
        add     hl, de
        ld      a, (hl)
        or      a
        jr      z, 39$
    38$:
        //dec     SHIP_HP(iy)
        //jr      nz, 39$
        jr 39$ // debug
        ld      a, #SHIP_TYPE_BOMB
        ld      SHIP_TYPE(iy), a
        xor     a
        ld      SHIP_STATE(iy), a
    39$:
    ret
    __endasm;
}
// スコアを更新する
static void GameUpdateScore(void) __naked {
    __asm;
    // スコアの更新
    ld      a, (_gameScorePlus)
    or      a
    jr      z, 09$
        ld      hl, #(_gameScore + 0x0005)
        ld      b, #0x06
        00$:
            add     a, (hl)
            ld      (hl), a
            sub     #0x0a
            jr      c, 01$
                ld      (hl), a
                dec     hl
                ld      a, #0x01
            djnz    00$
            ld      hl, #(_gameScore + 0x0000)
            ld      de, #(_gameScore + 0x0001)
            ld      bc, #0x0005
            ld      a, #0x09
            ld      (hl), a
            ldir
        01$:
        ld      hl, #_gameScore
        ld      de, #_appScore
        ld      b, #0x06
        02$:
            ld      a, (de)
            cp      (hl)
            jr      c, 03$
            jr      nz, 09$
            inc     hl
            inc     de
        djnz    02$
        jr      09$
        03$:
            ld      hl, #_gameScore
            ld      de, #_appScore
            ld      bc, #0x0006
            ldir
        09$:
    // 初期文字列の転送
    ld      hl, #_gameScoreString
    ld      de, #(_appPatternName + 0x0000)
    ld      bc, #0x0020
    ldir
    // スコアの描画
    ld      hl, #_gameScore
    ld      de, #(_appPatternName + 0x0003)
    ld      b, #0x06
    10$:
        ld      a, (hl)
        or      a
        jr      nz, 11$
        inc     hl
        inc     de
    djnz    10$
    jr      19$
    11$:
        ld      a, (hl)
        add     a, #0x10
        ld      (de), a
        inc     hl
        inc     de
    djnz    11$
    ld      a, #0x10
    ld      (de), a
    19$:
    // ハイスコアの描画
    ld      hl, #_appScore
    ld      de, #(_appPatternName + 0x000f)
    ld      b, #0x06
    20$:
        ld      a, (hl)
        or      a
        jr      nz, 21$
            inc     hl
            inc     de
        djnz    20$
        jr      29$
        21$:
            ld      a, (hl)
            add     a, #0x10
            ld      (de), a
            inc     hl
            inc     de
        djnz    21$
        ld      a, #0x10
        ld      (de), a
    29$:
    // 速度の描画
    ld      a, (_ship + SHIP_SPEED)
    add     a, a
    ld      e, a
    ld      d, #0x00
    ld      hl, #_gameSpeedString
    add     hl, de
    ld      de, #(_appPatternName + 0x1d)
    ld      bc, #0x0002
    ldir
    ret
    __endasm;
}
