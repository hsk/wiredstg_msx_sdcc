// main.c : ���C���v���O����
#include "bios.h"
#include "System.h"
#include "App.h"
#define H_TIMI_COUNT 0x02 /* �^�C�}���荞�݃J�E���^ */
// �ϐ���`
static char h_timiRoutine[5];// �^�C�}���荞�݃��[�`��
static char h_timiCount;// �^�C�}���荞�݃J�E���^
// ���C���v���O����
void main(void) __naked {
    __asm;    
    // ������
    // �V�X�e���̏�����
    call    _SystemInitialize
    // �A�v���P�[�V�����̏�����
    call    _AppInitialize
    // ���荞�݂̋֎~
    di
    // �^�C�}���荞�ݏ����̕ۑ�
    ld      hl, #H_TIMI
    ld      de, #_h_timiRoutine
    ld      bc, #5
    ldir
    // �^�C�}���荞�ݏ����̏�������
    ld      a, #0xc3
    ld      (H_TIMI + 0), a
    ld      hl, #_H_timiEntry
    ld      (H_TIMI + 1), hl
    // �^�C�}���荞�݃J�E���^�̏�����
    xor     a
    ld      (_h_timiCount), a
    // ���荞�݋֎~�̉���
    ei
    // �L�����Z���҂�
    0$:
        ld      a, (_flag)
        bit     #FLAG_CANCEL, a
    jr      z, 0$
    // �I��
    // �A�v���P�[�V�����̏I��
    // �V�X�e���̏I��
    // �L�[�{�[�h�o�b�t�@�̃N���A
    call    KILBUF
    // ���荞�݂̋֎~
    di
    // �^�C�}���荞�ݏ����̕��A
    ld      hl, #_h_timiRoutine
    ld      de, #H_TIMI
    ld      bc, #5
    ldir
    // ���荞�݋֎~�̉���
    ei
    // �I��
    ret
    __endasm;
}
// �^�C�}���荞�݂̃G���g��
static void H_timiEntry(void) __naked {
    __asm;
    // ���W�X�^�̕ۑ�
    push    af
    push    hl
    // ���荞�݋֎~�̉���
    ei
    // �^�C�}���荞�݃J�E���^�̍X�V
    ld      hl, #_h_timiCount
    inc     (hl)
    // ���łɊ��荞�ݍς݂��ǂ���
    ld      hl, #_flag
    bit     #FLAG_H_TIMI, (hl)
    jr      nz, H_timiEntryEnd
    // �w�肳�ꂽ�^�C�}���荞�݃J�E���^�ɓ��B������
    ld      a, (_h_timiCount)
    cp      #H_TIMI_COUNT
    jr      c, H_timiEntryEnd
    // �����̊J�n
    set     #FLAG_H_TIMI, (hl)
    // ���荞�݃J�E���^�̃N���A
    xor     a
    ld      (_h_timiCount), a
    // ���N�G�X�g�̎擾
    ld      a, (_request)
    ld      h, a
    // �r�f�I���W�X�^�̓]��
    bit     #REQUEST_VIDEO_REGISTER, h
    call    nz, _SystemTransferVideoRegister
    // �X�v���C�g�̓]��
    call    _SystemTransferSprite
    // VRAM �̓]��
    bit     #REQUEST_VRAM, h
    call    nz, _SystemTransferVram
    // �L�[���͂̍X�V
    call    _SystemUpdateInput
    // �T�E���h�̍X�V
    call    _SystemUpdateSound
    // STOP �L�[�ɂ��L�����Z��
    ld      a, (_input + INPUT_BUTTON_STOP)
    dec     a
    jr      nz, H_timiEntryDone
    ld      hl, #_flag
    set     #FLAG_CANCEL, (hl)
    // �����̊���
    H_timiEntryDone:
        // �A�v���P�[�V�����̍X�V
        call    _AppUpdate
        // ���荞�݂̊���
        ld      hl, #_flag
        res     #FLAG_H_TIMI, (hl)
    // �G���g���̏I��
    H_timiEntryEnd:
        // ���W�X�^�̕��A
        pop     hl
        pop     af
        // �ۑ����ꂽ�^�C�}���荞�݃��[�`���̎��s
        jp      _h_timiRoutine
        ret
    __endasm;
}
