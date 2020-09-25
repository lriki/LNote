//=============================================================================
//�y Tilemap �z
//-----------------------------------------------------------------------------
///**
//  @file       Tilemap.h
//  @brief      Tilemap
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// �� Tilemap
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Tilemap
    : public SceneNode
{
public:

    Tilemap( SceneGraph* scene_ );

    virtual ~Tilemap();

public:

    /// ������
    void initialize( u32 w_size_, u32 h_size_, LNTilemapDirection dir_ );

    /// �\���ʒu�̐ݒ�
    //virtual void setPosition( int x_, int y_ );

    /// �^�C���Z�b�g�e�N�X�`���̐ݒ�
    virtual void setTilesetTexture( Graphics::ITexture* texture_ );

    /// �}�b�v�f�[�^�z��̐ݒ�
    virtual void setMapData( int* mapdata_ ) { mMapData = mapdata_; }


public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

    /// �t���[���X�V
    virtual void update( const Game::GameTime& time_ );

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// ���݂̏�Ԃ����ɒ��_�o�b�t�@�����
	LNRESULT _makeVertexBuffer( LNTilemapDirection dir_ );

    /// �e�N�X�`�����W����������
	void _writeVertexUV();

private:

    u32                         mVertexNum;     ///< ���_�̑��� (���_�o�b�t�@�ɓ����Ă邩�炢��Ȃ�����)
    Graphics::IVertexBuffer*    mVertexBuffer;  ///< ���_�o�b�t�@
    Graphics::IIndexBuffer*     mIndexBuffer;   ///< �C���f�b�N�X�o�b�t�@
    Graphics::ITexture*         mTilesetTexture;

    u32         mXSize;         ///< X �����̃^�C���̐�
    u32         mYSize;         ///< Y �����̃^�C���̐�
    u32         mFaceNum;       ///< �ʐ�

    lnfloat     mTileWidth;     ///< �^�C���ЂƂ��̕�
    lnfloat     mTileHeight;    ///< �^�C���ЂƂ��̍���

    lnfloat     mUVWidth;       ///< �}�b�v�`�b�v�ЂƂ��̃s�N�Z���T�C�Y���e�N�X�`�����W�n�ɕϊ������Ƃ��̕�
    lnfloat     mUVHeight;      ///< �}�b�v�`�b�v�ЂƂ��̃s�N�Z���T�C�Y���e�N�X�`�����W�n�ɕϊ������Ƃ��̕�

    int*        mMapData;

    u32         mSrcXNum;       ///< �^�C���Z�b�g�摜�̉������̃`�b�v�� (RGSS �̏ꍇ�� 8)
    
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