;============================================================
; �� LNote �w���v�t�@�C��
;============================================================

%dll
LNote

%ver
1.0

%date
2013/2/10

%author
Riki

%note
lnote.as ���C���N���[�h���Ă�������

%type
�g������

%url
http://vianote.sakura.ne.jp/
;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_Init
�����@�\�̏�����

%group
�����@�\
%prm


%inst
�����@�\�݂̂��g�p����ݒ�ŏ��������s���܂��B

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGM
BGM �����t����

%group
�����@�\
%prm
filename, volume, pitch, fade_time
[in] filename     : �t�@�C����
[in] volume(100)  : �{�����[�� (0 �` 100)
[in] pitch(100)   : �s�b�` (50 �` 200)
[in] fade_time(0) : �t�F�[�h�C���ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGMMem
��������̉����f�[�^���� BGM �����t����

%group
�����@�\
%prm
data, data_size, volume, pitch, fade_time
[in] data         : ��������̉����f�[�^��
[in] data_size    : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(100)  : �{�����[�� (0 �` 100)
[in] pitch(100)   : �s�b�` (50 �` 200)
[in] fade_time(0) : �t�F�[�h�C���ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopBGM
BGM �̉��t���~����

%group
�����@�\
%prm
fade_time
[in] fade_time(0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGS
BGS �����t����

%group
�����@�\
%prm
filename, volume, pitch, fade_time
[in] filename     : �t�@�C����
[in] volume(100)  : �{�����[�� (0 �` 100)
[in] pitch(100)   : �s�b�` (50 �` 200)
[in] fade_time(0) : �t�F�[�h�C���ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGSMem
��������̉����f�[�^���� BGS �����t����

%group
�����@�\
%prm
data, data_size, volume, pitch, fade_time
[in] data         : ��������̉����f�[�^��
[in] data_size    : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(100)  : �{�����[�� (0 �` 100)
[in] pitch(100)   : �s�b�` (50 �` 200)
[in] fade_time(0) : �t�F�[�h�C���ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopBGS
BGS �̉��t���~����

%group
�����@�\
%prm
fade_time
[in] fade_time(0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�~���b)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayME
ME �����t����

%group
�����@�\
%prm
filename, volume, pitch
[in] filename    : �t�@�C����
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayMEMem
��������̉����f�[�^���� ME �����t����

%group
�����@�\
%prm
data, data_size, volume, pitch
[in] data        : ��������̉����f�[�^��
[in] data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopME
ME �̉��t���~����

%group
�����@�\
%prm


stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE
SE �����t����

%group
�����@�\
%prm
filename, volume, pitch
[in] filename    : �t�@�C����
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE3D
SE �����t���� (3D �T�E���h)

%group
�����@�\
%prm
filename, x, y, z, distance, volume, pitch
[in] filename    : �t�@�C����
[in] x, y, z     : 3D ���W
[in] distance    : ��������
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySEMem
��������̉����f�[�^���� SE �����t����

%group
�����@�\
%prm
data, data_size, volume, pitch
[in] data        : ��������̉����f�[�^��
[in] data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE3DMem
��������̉����f�[�^���� SE �����t���� (3D �T�E���h)

%group
�����@�\
%prm
data, data_size, x, y, z, distance, volume, pitch
[in] data        : ��������̉����f�[�^��
[in] data_size   : �f�[�^�T�C�Y (�o�C�g�P��)
[in] x, y, z     : 3D ���W
[in] distance    : ��������
[in] volume(100) : �{�����[�� (0 �` 100)
[in] pitch(100)  : �s�b�` (50 �` 200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopSE
���ׂĂ� SE �̉��t���~����

%group
�����@�\
%prm


stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_SetMetreUnitDistance
3D ��Ԃ�1���[�g�������̋����̐ݒ�

%group
�����@�\
%prm


stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetPosition
���X�i�[�̈ʒu�̐ݒ�

%group
�����@�\
%prm
x, y, z
[in] x, y, z : ���W

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetDirection
���X�i�[�̐��ʕ����̐ݒ�

%group
�����@�\
%prm
x, y, z
[in] x, y, z : ����

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetUpDirection
���X�i�[�̏�����̐ݒ�

%group
�����@�\
%prm
x, y, z
[in] x, y, z : ���� (���ʕ����Ƃ͒����ł��邱��)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_Velocity
���X�i�[�̑��x�̐ݒ�

%group
�����@�\
%prm
x, y, z
[in] x, y, z : ���x

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_CreateMem
��������̃f�[�^���特�����쐬����

%group
�����@�\
%prm
data, data_size, enable_3d
[in] data             : ��������̉����f�[�^��
[in] data_size        : �f�[�^�T�C�Y (�o�C�g�P��)
[in] enable_3d(false) : true �̏ꍇ�A3D �����Ƃ��č쐬����

stat : �T�E���h�n���h��
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Load
�t�@�C�����特�����쐬����

%group
�����@�\
%prm
out_handle, filename, enable_3d, play_type
[out] out_handle                       : �T�E���h�n���h�����i�[����ϐ�
[in]  filename                         : �t�@�C����
[in]  enable_3d(false)                 : true �̏ꍇ�A3D �����Ƃ��č쐬����
[in]  play_type(LN_SOUNDPLAYTYPE_AUTO) : �����̍Đ����@

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Release
�������폜����

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetVolume
�{�����[���̐ݒ�

%group
�����@�\
%prm
sound_handle, volume
[in] sound_handle : �T�E���h�n���h��
[in] volume       : �{�����[�� (0�`100)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetVolume
�{�����[���̎擾

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : �{�����[��
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetPitch
�s�b�`�̐ݒ�

%group
�����@�\
%prm
sound_handle, pitch
[in] sound_handle : �T�E���h�n���h��
[in] pitch        : �s�b�` (50�`200)

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetPitch
�s�b�`�̎擾

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : �s�b�`
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetLoopState
���[�v�Đ��̗L���ݒ�

%group
�����@�\
%prm
sound_handle, flag, begin, length
[in] sound_handle : �T�E���h�n���h��
[in] flag         : true = ���[�v�Đ����� / false = ���Ȃ�
[in] begin(0)     : ���[�v�̈�̐擪�ʒu (�T���v�����P��)
[in] length(0)    : ���[�v�̈撷�� (�T���v�����P��)

stat : ���U���g�R�[�h
%inst
begin �� length �� 0 ���w�肷��ƁA�S�̂����[�v�̈�Ƃ��Đݒ肵�܂��B

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_IsLoop
���[�v�Đ����L�����𒲂ׂ�

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : true = ���[�v�Đ����� / false = ���[�v�Đ����Ȃ�
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetPlayState
�T�E���h�̍Đ���Ԃ��擾����

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : LN_SOUNDPLAYSTATE_STOP    : ��~��
       LN_SOUNDPLAYSTATE_PLAYING : �Đ���
       LN_SOUNDPLAYSTATE_PAUSING : �ꎞ��~��
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Play
�������Đ�����

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Stop
�Đ����~����

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Pause
�ꎞ��~

%group
�����@�\
%prm
sound_handle, pause
[in] sound_handle : �T�E���h�n���h��
[in] pause        : true = �ꎞ��~ / false = �ꎞ��~����

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_FadeVolume
���ʂ̃t�F�[�h

%group
�����@�\
%prm
sound_handle, target_volume, time, fade_state
[in] sound_handle  : �T�E���h�n���h��
[in] target_volume : �ύX��̉���
[in] time          : �t�F�[�h�ɂ����鎞�� (�~���b)
[in] fade_state    : �t�F�[�h������̓���̎w��
                       LN_SOUNDFADE_CONTINUE : �p��
                       LN_SOUNDFADE_STOP : ��~����
                       LN_SOUNDFADE_STOP_RESET : ��~���āA���ʂ����̒l�ɖ߂�
                       LN_SOUNDFADE_PAUSE : �ꎞ��~����
                       LN_SOUNDFADE_PAUSE_RESET : �ꎞ��~���āA���ʂ����̒l�ɖ߂�

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetUnitsPlayed
�Đ������T���v�����̎擾

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : �Đ������T���v����
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetTotalUnits
�����f�[�^�S�̂̃T���v�����̎擾

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : �����f�[�^�S�̂̃T���v����
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetUnitsParSecond
�T���v�����O���g���̎擾

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : �T���v�����O���g��
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Is3DSound
3D �������𒲂ׂ�

%group
�����@�\
%prm
sound_handle
[in] sound_handle : �T�E���h�n���h��

stat : true=3D���� / false=3D�����ł͂Ȃ�
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterPosition
3D �����Ƃ��Ă̈ʒu��ݒ肷��

%group
�����@�\
%prm
sound_handle, x, y ,z
[in] sound_handle : �T�E���h�n���h��
[in] x, y ,z      : 3D ���W

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterVelocity
3D �����Ƃ��Ă̑��x��ݒ肷��

%group
�����@�\
%prm
sound_handle, x, y, z
[in] sound_handle : �T�E���h�n���h��
[in] x, y, z      : ���x

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterDistance
3D �����̌������� (�������Ȃ��Ȃ鋗��) ��ݒ肷��

%group
�����@�\
%prm
sound_handle, distance
[in] sound_handle : �T�E���h�n���h��
[in] distance     : ����

stat : ���U���g�R�[�h
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetEnableLogFile
���O�t�@�C���̏o�͐ݒ�

%group
�����ݒ�
%prm
flag
[in] flag : true=���O�t�@�C�����o�͂��� / false=���Ȃ�

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetEnableConsole
�R���\�[���̊��蓖�Đݒ�

%group
�����ݒ�
%prm
flag
[in] flag : true=���蓖�Ă� / false=���蓖�ĂȂ�

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetUserWindowHandle
���[�U�[��`�̃E�B���h�E�n���h���̐ݒ�

%group
�����ݒ�
%prm
window_handle
[in] window_handle : ���[�U�[��`�̃E�B���h�E�n���h��

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetDirectMusicUsingMode
DirectMusic �̎g�p�ݒ�

%group
�����ݒ�
%prm
mode
[in] mode : DirectMusic �̎g�p���@
              LN_DMUSINGMODE_NOTUSE         : �g�p���Ȃ�
              LN_DMUSINGMODE_USE            : �g�p����
              LN_DMUSINGMODE_THREAD_REQUEST : �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͍Đ���\�񂷂�

%inst

%href

