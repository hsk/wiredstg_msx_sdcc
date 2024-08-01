// Game.h : ゲーム
//


// マクロの定義
//

// スプライト
#define GAME_SPRITE_SHIP_0      0x00
#define GAME_SPRITE_BULLET      0x10
#define GAME_SPRITE_SHOT        0x50
#define GAME_SPRITE_SHIP_1      0x60

// 外部関数宣言
//

void GameInitialize(void);
void GameUpdate(void);

// 外部変数宣言
//

extern char gameScroll;
