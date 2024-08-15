// Ground.h : 地面
// ジェネレータ
#define GROUND_GENERATOR_UPPER_STATE    0x00
#define GROUND_GENERATOR_UPPER_HEIGHT   0x01
#define GROUND_GENERATOR_UPPER_LENGTH   0x02
#define GROUND_GENERATOR_LOWER_STATE    0x03
#define GROUND_GENERATOR_LOWER_HEIGHT   0x04
#define GROUND_GENERATOR_LOWER_LENGTH   0x05
#define GROUND_GENERATOR_COUNT          0x06
#define GROUND_GENERATOR_SIZE           0x07
// ジェネレータの状態
#define GROUND_GENERATOR_STATE_NULL     0x00
#define GROUND_GENERATOR_STATE_FLAT     0x01
#define GROUND_GENERATOR_STATE_UP       0x02
#define GROUND_GENERATOR_STATE_DOWN     0x03
// 外部関数宣言
void GroundInitialize(void);
void GroundUpdate(void);
void GroundRender(void);
// 外部変数宣言
extern char ground[0x0300];
extern char groundGenerator[GROUND_GENERATOR_SIZE];
