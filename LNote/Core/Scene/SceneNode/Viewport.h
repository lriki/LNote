//=============================================================================
//�y Viewport �z
//-----------------------------------------------------------------------------
///**
//  @file       Viewport.h
//  @brief      Viewport
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Animation/FadeValue.h"
#include "../../Graphics/Common/Model/Model.h"
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// �� Viewport �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Viewport
    : public SceneNode
{
public:

    /// �R���X�g���N�^
    Viewport( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~Viewport();

public:

    /// ������
    LNRESULT initialize( const Geometry::Size& size_ );

    

    /// �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_��ǉ����� (priority_ �͒l���傫��������ɏ��������BScriptOrder �� preprocess �܂��� postprocess ���w�肳��Ă���V�F�[�_�̂ݒǉ��\)
    void addPostEffectShader( SceneShader* shader_, int priority_ );

    /// �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_���O��
    void removePostEffectShader( SceneShader* shader_ );

    /// �F���̐ݒ�
    void setTone( const LTone& tone_, double duration_ );

    /// �G�t�F�N�g�K�p�����𔻒肷��
    bool isEffect() const { return mTone.isFinished(); }

public:

    /// �`��R�}���h���K�w�I�ɍ쐬����
    virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_VIEWPORT; }

    /// �t���[���X�V
    virtual void update( const Game::GameTime& time_ );

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    

private:

    struct PPShader
    {
        int          Priority;
        SceneShader* Shader;

        bool operator() ( const PPShader& e_ ) const { return e_.Shader == Shader; }
    };

    typedef std::vector< PPShader >    PPShaderArray;

    /// �\�[�g�p�̔�r�֐� (�~��)
    static bool _cmpPPShader( const PPShader& l_, const PPShader& r_ ) { return l_.Priority >= r_.Priority; }

private:

    PPShaderArray                       mPPShaderArray;
    Animation::FadeValue< LVector4, lnfloat >    mTone;
    LVector2                            mViewSize;

    

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================