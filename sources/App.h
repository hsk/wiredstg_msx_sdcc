// App.h : アプリケーション
//


// マクロの定義
//

// ビデオ
#define APP_PATTERN_NAME_TABLE          0x1800
#define APP_PATTERN_GENERATOR_TABLE_0   0x0000
#define APP_PATTERN_GENERATOR_TABLE_1   0x0800
#define APP_PATTERN_GENERATOR_TABLE_2   0x1000
#define APP_COLOR_TABLE                 0x2000
#define APP_SPRITE_ATTRIBUTE_TABLE      0x1b00
#define APP_SPRITE_GENERATOR_TABLE      0x3800

// 状態
#define APP_STATE_NULL                  0
#define APP_STATE_TITLE_INITIALIZE      1
#define APP_STATE_TITLE_UPDATE          2
#define APP_STATE_GAME_INITIALIZE       3
#define APP_STATE_GAME_UPDATE           4


// 外部関数宣言
//

void AppInitialize(void);
void AppUpdate(void);
void AppTransferPatternName(void);

// 外部変数宣言
//

// 状態
extern char appState;
extern char appColor;
extern char appScore[6];
extern char appPatternName[0x0300];
