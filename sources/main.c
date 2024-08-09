// main.c : ���C���v���O����
#include "bios.h"
#include "System.h"
#include "App.h"
#include "string.h"
#define H_TIMI_COUNT 0x02 /* �^�C�}���荞�݃J�E���^ */
// �ϐ���`
static char h_timiRoutine[5];// �^�C�}���荞�݃��[�`��
static char h_timiCount;// �^�C�}���荞�݃J�E���^
static void H_timiEntry(void);
// ���C���v���O����
void main(void) {
    // ������
    // �V�X�e���̏�����
    SystemInitialize();
    AppInitialize();// �A�v���P�[�V�����̏�����
    __asm di __endasm; // ���荞�݂̋֎~
    // �^�C�}���荞�ݏ����̕ۑ�
    //for(char *hl = (char*)H_TIMI,*de = h_timiRoutine,bc=5;bc;bc--)*de++=*hl++;
    memcpy(h_timiRoutine,(char*)H_TIMI,5);
    // �^�C�}���荞�ݏ����̏�������
    ((char*)H_TIMI)[0] = 0xc3;
    *(short*)&((char*)H_TIMI)[1] = (short)H_timiEntry;
    // �^�C�}���荞�݃J�E���^�̏�����
    h_timiCount = 0;
    __asm ei __endasm; // ���荞�݂̋֎~�̉���
    while((flag & (1<<FLAG_CANCEL)) == 0);// �L�����Z���҂�
    // �I��
    // �A�v���P�[�V�����̏I��
    // �V�X�e���̏I��
    // �L�[�{�[�h�o�b�t�@�̃N���A
    typedef void (*FP)(void);
    ((FP)KILBUF)();
    __asm di __endasm; // ���荞�݂̋֎~
    // �^�C�}���荞�ݏ����̕��A
    //for(char *hl = h_timiRoutine,*de = (char*)H_TIMI,bc=5;bc;bc--)*de++=*hl++;
    memcpy((char*)H_TIMI,h_timiRoutine,5);
    __asm ei __endasm; // ���荞�݂̋֎~�̉���
}
// �^�C�}���荞�݂̃G���g��
static void H_timiEntry(void) __naked {
    __asm ei __endasm; // ���荞�݂̋֎~�̉���
    h_timiCount++;// �^�C�}���荞�݃J�E���^�̍X�V
    if (!(flag & (1<<FLAG_H_TIMI)) &&// ���łɊ��荞�ݍς݂��ǂ���
        h_timiCount>=H_TIMI_COUNT) {// �w�肳�ꂽ�^�C�}���荞�݃J�E���^�ɓ��B������
        // �����̊J�n
        flag |= (1<<FLAG_H_TIMI); // ���荞�݂̊���
        h_timiCount = 0;// ���荞�݃J�E���^�̃N���A
        if (request&(1<<REQUEST_VIDEO_REGISTER)) SystemTransferVideoRegister(); // �r�f�I���W�X�^�̓]��
        // �X�v���C�g�̓]��
        SystemTransferSprite();
        if (request & (1 << REQUEST_VRAM)) SystemTransferVram(); // VRAM �̓]��
        SystemUpdateInput();// �L�[���͂̍X�V
        if (input[INPUT_BUTTON_STOP]==1) flag |= (1 << FLAG_CANCEL);// STOP �L�[�ɂ��L�����Z��
        // �����̊���
        AppUpdate(); // �A�v���P�[�V�����̍X�V
        flag &= ~(1<<FLAG_H_TIMI); // ���荞�݂̊���
    }
    __asm;
        jp      _h_timiRoutine// �ۑ����ꂽ�^�C�}���荞�݃��[�`���̎��s
    __endasm;
}
