// Enemy.h : 敵
// 敵
#define ENEMY_TYPE              0x00
#define ENEMY_STATE             0x01
#define ENEMY_POSITION_X        0x02
#define ENEMY_POSITION_Y        0x03
#define ENEMY_INDEX             0x04
#define ENEMY_HP                0x05
#define ENEMY_SHOT              0x06
#define ENEMY_ANIMATION         0x0a
#define ENEMY_TIMER             0x0b
#define ENEMY_PARAM_0           0x0c
#define ENEMY_PARAM_1           0x0d
#define ENEMY_PARAM_2           0x0e
#define ENEMY_PARAM_3           0x0f
#define ENEMY_SIZE              0x10
#define ENEMY_N                 0x10
// ジェネレータ
#define ENEMY_GENERATOR_PHASE   0x00
#define ENEMY_GENERATOR_TYPE    0x01
#define ENEMY_GENERATOR_STATE   0x02
#define ENEMY_GENERATOR_LENGTH  0x03
#define ENEMY_GENERATOR_TIMER   0x04
#define ENEMY_GENERATOR_PARAM_0 0x05
#define ENEMY_GENERATOR_PARAM_1 0x06
#define ENEMY_GENERATOR_PARAM_2 0x07
#define ENEMY_GENERATOR_SIZE    0x08
// 段階
#define ENEMY_PHASE_NULL        0x00
#define ENEMY_PHASE_NORMAL      0x01
// 種類
#define ENEMY_TYPE_NULL         0x00
#define ENEMY_TYPE_BOMB         0x01
#define ENEMY_TYPE_FANS_FRONT   0x02
#define ENEMY_TYPE_FANS_BACK    0x03
#define ENEMY_TYPE_RUGAL_FRONT  0x04
#define ENEMY_TYPE_RUGAL_BACK   0x05
#define ENEMY_TYPE_GARUN_FRONT  0x06
#define ENEMY_TYPE_GARUN_BACK   0x07
#define ENEMY_TYPE_DEE01_UPPER  0x08
#define ENEMY_TYPE_DEE01_LOWER  0x09
#define ENEMY_TYPE_DUCKER_UPPER 0x0a
#define ENEMY_TYPE_DUCKER_LOWER 0x0b
#define ENEMY_TYPE_BIGCORE_CORE 0x0c
#define ENEMY_TYPE_BIGCORE_BODY 0x0d
#define ENEMY_TYPE_BEAM         0x0e
// 状態
#define ENEMY_STATE_NULL        0x00
// 外部関数宣言
void EnemyInitialize(void);
void EnemyUpdate(void);
void EnemyRender(void);
char* EnemyGetEmpty(void);
void EnemyBombUpdate(char* ix);
void EnemyBombRender(char* ix);
void EnemyFansGenerate(void);
void EnemyFansUpdate(char* ix);
void EnemyRugalGenerate(void);
void EnemyRugalUpdate(char* ix);
void EnemyGarunGenerate(void);
void EnemyGarunUpdate(char* ix);
void EnemyDee01Generate(void);
void EnemyDee01Update(char* ix);
void EnemyDuckerGenerate(void);
void EnemyDuckerUpdate(char* ix);
void EnemyBigCoreGenerate(void);
void EnemyBigCoreUpdateCore(char* ix);
void EnemyBigCoreUpdateBody(char* ix);
void EnemyBigCoreRender(char* ix);
void EnemyBigCoreBodyRender(char* ix);
void EnemyBeamGenerate(short hl);
void EnemyBeamUpdate(char* ix);
void EnemyBeamRender(char* ix);
// 外部変数宣言
extern char enemy[ENEMY_SIZE * ENEMY_N];
extern char eneyN;
extern char enemyGenerator[ENEMY_GENERATOR_SIZE];
extern char enemyCollision[0x0300];
