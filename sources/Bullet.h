// Bullet.h : 弾
// 弾
#define BULLET_TYPE        0x00
#define BULLET_STATE       0x01
#define BULLET_POSITION_XD 0x02
#define BULLET_POSITION_XI 0x03
#define BULLET_POSITION_YD 0x04
#define BULLET_POSITION_YI 0x05
#define BULLET_SPEED_XD    0x06
#define BULLET_SPEED_XI    0x07
#define BULLET_SPEED_YD    0x08
#define BULLET_SPEED_YI    0x09
#define BULLET_SIZE        0x10
#define BULLET_N           0x10
// 種類
#define BULLET_TYPE_NULL   0x00
// 状態
#define BULLET_STATE_NULL  0x00
// 外部関数宣言
void BulletInitialize(void);
void BulletGenerate(short hl);
void BulletUpdate(void);
void BulletRender(void);
// 外部変数宣言
extern char bullet[BULLET_SIZE * BULLET_N];
extern char bulletN;
