// System.h : システムライブラリ
// フラグ
#define FLAG_H_TIMI             0
#define FLAG_SOUND_SLEEP        1
#define FLAG_CANCEL             7
// リクエスト
#define REQUEST_NULL            0
#define REQUEST_VIDEO_REGISTER  1
#define REQUEST_VRAM            2
// キー入力
#define INPUT_KEY_UP            0
#define INPUT_KEY_DOWN          1
#define INPUT_KEY_LEFT          2
#define INPUT_KEY_RIGHT         3
#define INPUT_BUTTON_SPACE      4
#define INPUT_BUTTON_SHIFT      5
#define INPUT_BUTTON_ESC        6
#define INPUT_BUTTON_STOP       7
#define INPUT_SIZE              8
// ビデオ
#define VIDEO_TEXT1                                 0
#define VIDEO_TEXT1_PATTERN_NAME_TABLE              0x0000
#define VIDEO_TEXT1_PATTERN_GENERATOR_TABLE         0x0800
#define VIDEO_TEXT1_COLOR_TABLE                     0x0000
#define VIDEO_TEXT1_SPRITE_ATTRIBUTE_TABLE          0x1b00
#define VIDEO_TEXT1_SPRITE_GENERATOR_TABLE          0x3800
#define VIDEO_GRAPHIC1                              1
#define VIDEO_GRAPHIC1_PATTERN_NAME_TABLE           0x1800
#define VIDEO_GRAPHIC1_PATTERN_GENERATOR_TABLE      0x0000
#define VIDEO_GRAPHIC1_COLOR_TABLE                  0x2000
#define VIDEO_GRAPHIC1_SPRITE_ATTRIBUTE_TABLE       0x1b00
#define VIDEO_GRAPHIC1_SPRITE_GENERATOR_TABLE       0x3800
#define VIDEO_GRAPHIC2                              2
#define VIDEO_GRAPHIC2_PATTERN_NAME_TABLE           0x1800
#define VIDEO_GRAPHIC2_PATTERN_GENERATOR_TABLE      0x0000
#define VIDEO_GRAPHIC2_COLOR_TABLE                  0x2000
#define VIDEO_GRAPHIC2_SPRITE_ATTRIBUTE_TABLE       0x1b00
#define VIDEO_GRAPHIC2_SPRITE_GENERATOR_TABLE       0x3800
#define VIDEO_MULTICOLOR                            3
#define VIDEO_MULTICOLOR_PATTERN_NAME_TABLE         0x0800
#define VIDEO_MULTICOLOR_PATTERN_GENERATOR_TABLE    0x0000
#define VIDEO_MULTICOLOR_COLOR_TABLE                0x2000
#define VIDEO_MULTICOLOR_SPRITE_ATTRIBUTE_TABLE     0x1b00
#define VIDEO_MULTICOLOR_SPRITE_GENERATOR_TABLE     0x3800
//#define VIDEO_TRANSFER_VRAM_0                       0
//#define VIDEO_TRANSFER_VRAM_1                       1
//#define VIDEO_TRANSFER_VRAM_2                       2
//#define VIDEO_TRANSFER_VRAM_3                       3
#define VIDEO_TRANSFER_VRAM_SIZE                    4
#define VIDEO_TRANSFER_SRC                          0
#define VIDEO_TRANSFER_DST                          2
#define VIDEO_TRANSFER_BYTES                        4
#define VIDEO_TRANSFER_SIZE                         5
#define VIDEO_TRANSFER_VRAM_0                       0
#define VIDEO_TRANSFER_VRAM_0_SRC                   0
#define VIDEO_TRANSFER_VRAM_0_DST                   2
#define VIDEO_TRANSFER_VRAM_0_BYTES                 4
#define VIDEO_TRANSFER_VRAM_1                       5
#define VIDEO_TRANSFER_VRAM_1_SRC                   5
#define VIDEO_TRANSFER_VRAM_1_DST                   7
#define VIDEO_TRANSFER_VRAM_1_BYTES                 9
#define VIDEO_TRANSFER_VRAM_2                       10
#define VIDEO_TRANSFER_VRAM_2_SRC                   10
#define VIDEO_TRANSFER_VRAM_2_DST                   12
#define VIDEO_TRANSFER_VRAM_2_BYTES                 14
#define VIDEO_TRANSFER_VRAM_3                       15
#define VIDEO_TRANSFER_VRAM_3_SRC                   15
#define VIDEO_TRANSFER_VRAM_3_DST                   17
#define VIDEO_TRANSFER_VRAM_3_BYTES                 19
// 外部関数宣言
// システム
void SystemInitialize(void);
// キー入力
void SystemInitializeInput(void);
void SystemUpdateInput(void);
// ビデオ
void SystemInitializeVideo(void);
void SystemSetScreenMode(void);
void SystemTransferVideoRegister(void);
void SystemTransferVram(void);
// スプライト
void SystemClearSprite(void);
void SystemTransferSprite(void);
void SystemSetSprite(void);
// サウンド
void SystemInitializeSound(void);
void SystemUpdateSound(void);
void SystemSuspendSound(void);
void SystemResumeSound(void);
// 乱数
void SystemGetRandom(void);
// 三角関数
void SystemGetSin(void);
void SystemGetCos(void);
void SystemGetAtan2(void);
// デバッグ
void SystemPutRegisterA(void);
// 外部変数宣言
extern char flag;// フラグ
extern char request;// リクエスト
extern char input[INPUT_SIZE];// キー入力
extern char videoPort[2];// ビデオ
extern char videoRegister[8];
extern char videoTransfer[VIDEO_TRANSFER_VRAM_SIZE * VIDEO_TRANSFER_SIZE];
extern char sprite[128];// スプライト
extern char* soundRequest[4];// サウンド
extern char soundHead[8];
extern char soundPlay[8];
