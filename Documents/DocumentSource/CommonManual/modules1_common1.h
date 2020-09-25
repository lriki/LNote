//=============================================================================
//�y ����/�x�[�X�@�\ �z
//-----------------------------------------------------------------------------
/**
    @page modules1_common1 ����/�x�[�X�@�\

    - @ref modules1_common1_section1   "�G���[����"
    - @ref modules1_common1_section2   "���U���g�R�[�h"
    <br>

    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    <hr>
    @anchor modules1_common1_section1
    @par �G���[����
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    �@�\�̏������⃊�\�[�X�̓ǂݍ��ݓ��̏����́A<br>
    �I����ɐ����܂��͎��s�Ƃ��̌������������l(���U���g�R�[�h)��Ԃ��܂��B<br>
    <br>
    SUCCEEDED�AFAILED �}�N�����g�p���邱�ƂŁA����/���s���`�F�b�N���邱�Ƃ��ł��܂��B<br>
    <br>
@ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
HRESULT hr = LAudio_Init();
if (SUCCEEDED(hr))
{
    // ����
}
if (FAILED(hr))
{
    // ���s
}

// �����U���g�R�[�h�̒��g�͓Ǝ���`�� HRESULT �^�ł��B
        @ln_endcode
        @ln_code{tab_hsp,cpp}
// ���߃��t�@�����X�Ɂustat:���U���g�R�[�h�v�Ə�����Ă��閽�߂� stat �Ƀ��U���g�R�[�h���i�[���܂�
LAudio_Init
if SUCCEEDED(stat) {
    // ����
}
if FAILED(stat) {
    // ���s
}
        @ln_endcode
    @ln_endcodeblock
    <br>
    <br>
    ���U���g�R�[�h�͐���/���s�̑��ɁA���s�̌�����\���܂��B
    �ȉ��̂悤�ɃR�[�f�B���O���邱�ƂŁA���s�̌�����m�邱�Ƃ��ł��܂��B<br>
    <br>
    @ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
HRESULT hr = LAudio_Init();
switch (hr)
{
    case LN_OK:
        // ����
        break;
    case LN_ERR_INITIALIZED:
        // ���ɏ���������Ă���
        break;
    case LN_ERR_DIRECTX:
        // DirectX �̋@�\�ŃG���[������
        break;
    default:
        // ���̑�
        break;
}
        @ln_endcode
        @ln_code{tab_hsp,cpp}
// ���߃��t�@�����X�Ɂustat:���U���g�R�[�h�v�Ə�����Ă��閽�߂� stat �Ƀ��U���g�R�[�h���i�[���܂�
LAudio_Init
switch stat
    case LN_OK
        // ����
        swbreak
    case LN_ERR_INITIALIZED
        // ���ɏ���������Ă���
        swbreak
    case LN_ERR_DIRECTX:
        // DirectX �̋@�\�ŃG���[������
        swbreak
    default
        // ���̑�
        swbreak
swend
        @ln_endcode
    @ln_endcodeblock
    <br>
    ���U���g�R�[�h�̒�`�Ɛ����� @ref modules1_common1_section2 "���U���g�R�[�h" ���Q�Ƃ��Ă��������B
    <br>
    <br>
    
    
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    <hr>
    @anchor modules1_common1_section2
    @par ���U���g�R�[�h
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    ���U���g�R�[�h�͈ȉ��̒l����`����Ă��܂��B<br>
    LN_ERR_�`�͂��ׂĕ��̒l�ł��B<br>
    <br>
    <table>
    <tr bgcolor="#e0e0e0">
        <td>�萔</td><td>����</td>
    </tr>
    <tr>
        <td>LN_OK</td><td>�����B�l�� 0 �ł��B</td>
    </tr>
    <tr>
        <td>LN_ERR_INVALIDCALL</td><td>����������</td>
    </tr>
    <tr>
        <td>LN_ERR_OUTOFMEMORY</td><td>�������s��</td>
    </tr>
    <tr>
        <td>LN_ERR_NULLPOINTER</td><td>NULL �A�N�Z�X</td>
    </tr>
    <tr>
        <td>LN_ERR_FILENOTFOUND</td><td>�t�@�C�������� (�I�[�v���Ɏ��s)</td>
    </tr>
    <tr>
        <td>LN_ERR_SYSTEM</td><td>�V�X�e��API �Ăяo���G���[</td>
    </tr>
    <tr>
        <td>LN_ERR_INITIALIZED</td><td>�������ς�</td>
    </tr>
    <tr>
        <td>LN_ERR_NOINITIALIZE</td><td>����������Ă��Ȃ�</td>
    </tr>
    <tr>
        <td>LN_ERR_FORMAT</td><td>��Ή��t�@�C���`��</td>
    </tr>
    <tr>
        <td>LN_ERR_NOTIMPL</td><td>��������Ă��Ȃ��@�\���Ăяo�����Ƃ���</td>
    </tr>
    <tr>
        <td>LN_ERR_FATAL</td><td>�p���s�\�ȃG���[</td>
    </tr>
    <tr>
        <td>LN_ERR_DIRECTX</td><td>DirectX �֌W�̃G���[</td>
    </tr>
    <tr>
        <td>LN_ERR_OPENGL</td><td>OpenGL �֌W�̃G���[</td>
    </tr>
    </table>
    
    <br>
    <br>
*/
//=============================================================================

