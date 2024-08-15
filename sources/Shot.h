// Shot.h : ショット
// ショット
#define SHOT_TYPE         0x00
#define SHOT_STATE        0x01
#define SHOT_POSITION_X   0x02
#define SHOT_POSITION_Y   0x03
#define SHOT_RANGE_LEFT   0x04
#define SHOT_RANGE_TOP    0x05
#define SHOT_RANGE_RIGHT  0x06
#define SHOT_RANGE_BOTTOM 0x07
#define SHOT_SIZE         0x08
#define SHOT_N            0x04
// 種類
#define SHOT_TYPE_NULL    0x00
// 状態
#define SHOT_STATE_NULL   0x00
// 外部関数宣言
void ShotInitialize(void);
void ShotGenerate(void);
void ShotUpdate(void);
void ShotRender(void);
// 外部変数宣言
extern char shot[SHOT_SIZE * SHOT_N];
