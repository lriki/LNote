//=============================================================================
//�y Shader �z
//-----------------------------------------------------------------------------
///**
//  @file       Shader.h
//  @brief      Shader
//  @author     Riki
//*/
//-----------------------------------------------------------------------------
/*
    �ׂ��������c
        �A�m�e�[�V�����ɂ͍s��A�z��͎g�����AShaderVariable �Ƃ��Ĉ����Ă邯��
        �l�̃Z�b�g�͂ł��Ȃ��B

        double �� float �Ƌ�ʂ���Ă���BMME �V�F�[�_�� double4 �Ƃ����Ă��F�����Ȃ��B

        �x�N�g���� Rows �� 1�AColumns ������
*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <map>
#include "../../Base/Cache.h"
#include "../../Base/RefString.h"
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
{
class GraphicsDevice;
class Shader;
class ShaderVariable;
class ShaderAnnotation;
class ShaderTechnique;
class ShaderPass;

typedef Base::CacheObjectManager< Shader >    ShaderCache;

typedef std::vector< ShaderVariable* >                  ShaderVariableArray;
typedef std::map< LRefTString, ShaderVariable* >        ShaderVariableMap;
typedef std::pair< LRefTString, ShaderVariable* >       ShaderVariablePair;

typedef std::vector< ShaderAnnotation* >                ShaderAnnotationArray;
typedef std::map< LRefTString, ShaderAnnotation* >      ShaderAnnotationMap;
typedef std::pair< LRefTString, ShaderAnnotation* >     ShaderAnnotationPair;

typedef std::vector< ShaderTechnique* >                 ShaderTechniqueArray;
typedef std::map< LRefTString, ShaderTechnique* >       ShaderTechniqueMap;
typedef std::pair< LRefTString, ShaderTechnique* >      ShaderTechniquePair;

typedef std::vector< ShaderPass* >                      ShaderPassArray;
typedef std::map< LRefTString, ShaderPass* >            ShaderPassMap;
typedef std::pair< LRefTString, ShaderPass* >           ShaderPassPair;

//=============================================================================
// �� Shader �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�����������N���X
//*/
//=============================================================================
class Shader
    : public Base::ReferenceObject
    , public ShaderCache::CachedObject
    , public IShader
{
public:

    /// �R���X�g���N�^
    Shader( GraphicsDevice* manager_ );

    /// �f�X�g���N�^
    virtual ~Shader();

    LN_CACHED_REFOBJ_METHODS;

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      ������
    //
    //  @param[in]  code_   : �f�[�^�̂���o�b�t�@�ւ̃|�C���^ (NULL �I�[������)
    //*/
    //---------------------------------------------------------------------
    LNRESULT initialize( const char* code_ );

public:

    /// �u�[���l�̐ݒ�
	virtual LNRESULT setBool( const lnChar* name_, bool b_ );

    /// �����l�̐ݒ�
	virtual LNRESULT setInt( const lnChar* name_, int value_ );

    /// �����l�̐ݒ�
	virtual LNRESULT setFloat( const lnChar* name_, lnfloat value_ );

    /// �x�N�g���̐ݒ�
	virtual LNRESULT setVector( const lnChar* name_, const LVector4& vec_ );

    /// �x�N�g���z��̐ݒ�
	virtual LNRESULT setVectorArray( const lnChar* name_, const LVector4* vec_, lnU32 count_ );

    /// �s��̐ݒ�
	virtual LNRESULT setMatrix( const lnChar* name_, const LMatrix& matrix_ );

    /// �s��̔z��̐ݒ�
	virtual LNRESULT setMatrixArray( const lnChar* name_, const LMatrix* matrices_, lnU32 count_ );

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( const lnChar* name_, ITexture* texture_ );

    /// ���O�ɂ���ĕϐ����擾����
	virtual IShaderVariable* getVariableByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ĕϐ����擾����
	virtual IShaderVariable* getVariableByIndex( u32 index_ );

    /// �Z�}���e�B�N�X�ɂ���ĕϐ����擾����
	//virtual IShaderVariable* getVariableBySemantic( const char* name_ );
    
    /// ���O�ɂ���ăe�N�j�b�N���擾����
	virtual IShaderTechnique* getTechniqueByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ăe�N�j�b�N���擾����
	virtual IShaderTechnique* getTechniqueByIndex( u32 index_ );

    /// �V�F�[�_�̏����擾����
    virtual const LNShaderDesc& getDesc() { return mDesc; }
    
    /// �R���p�C�����ʂ��擾����
    virtual LNShaderCompileResult getCompileResult() { return mCompileResult; }

    /// �R���p�C���G���[�E�x���̕������Ԃ�
    virtual const lnChar* getCompileErrors() { return mCompileErrors.c_str(); }

public:

    GraphicsDevice* getGraphicsDevice() { return mManager; }

    /// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    LNRESULT onLostDevice();

    /// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    LNRESULT onResetDevice();

    /// �e�N�X�`���^�ꗗ
    ShaderVariableArray& getTextureVariableArray() { return mTextureVariableArray; }

private:

    GraphicsDevice*         mManager;
    ID3DXEffect*            mDxEffect;
    LNShaderCompileResult   mCompileResult;
    LRefTString             mCompileErrors;
    LNShaderDesc            mDesc;
    ShaderTechniqueArray    mShaderTechniqueArray;
    ShaderTechniqueMap      mShaderTechniqueMap;
    ShaderVariableArray     mShaderVariableArray;
    ShaderVariableMap       mShaderVariableMap;
    ShaderVariableArray     mTextureVariableArray;  ///< (�T���v���X�e�[�g�����̂��߂̌����Ώ�)
};

//=============================================================================
// �� ShaderVariable �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̕ϐ��������N���X
//*/
//=============================================================================
class ShaderVariable
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

    /// �R���X�g���N�^
    ShaderVariable( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ );

    /// �f�X�g���N�^
    virtual ~ShaderVariable();

    LN_REFOBJ_METHODS;

public:

    /// �u�[���l�̎擾
	virtual LNRESULT getBool( bool* b_ );

    /// �u�[���l�̐ݒ�
	virtual LNRESULT setBool( bool b_ );

    /// �����l�̎擾
	virtual LNRESULT getInt( int* value_ );

    /// �����l�̐ݒ�
	virtual LNRESULT setInt( int value_ );

    /// �����l�̎擾
    virtual LNRESULT getFloat( lnfloat* value_ );

    /// �����l�̐ݒ�
    virtual LNRESULT setFloat( lnfloat value_ );

    /// �x�N�g���̎擾
	virtual LNRESULT getVector( LVector4* vec_ );

    /// �x�N�g���̐ݒ�
	virtual LNRESULT setVector( const LVector4& vec_ );

    /// �x�N�g���z��̎擾
	virtual LNRESULT getVectorArray( LVector4* vec_, u32 count_ );
    
    /// �x�N�g���z��̐ݒ�
	virtual LNRESULT setVectorArray( const LVector4* vec_, u32 count_ );

    /// �s��̎擾
    virtual LNRESULT getMatrix( LMatrix* matrix_ );

    /// �s��̐ݒ�
    virtual LNRESULT setMatrix( const LMatrix& matrix_ );

    /// �s��̔z��̎擾
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ );

    /// �s��̔z��̐ݒ�
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ );

    /// �e�N�X�`���̎擾
	//virtual LNRESULT getTexture( ITexture** texture_ );

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( ITexture* texture_ );

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ );

    /// ������̎擾
	virtual LNRESULT getString( const char** str_ );

    /// ������̐ݒ�
	virtual LNRESULT setString( const char* str_ );

    /// �ϐ����̎擾
	virtual const char* getName() const;

    /// �Z�}���e�B�N�X���̎擾
	virtual const char* getSemanticName() const;

    /// �^���̎擾
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const;

    /// �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// ���O�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByName( const char* name_ );

public:

    struct TextureParam
    {
        ITexture*       Texture;
        LSamplerState   SamplerState;
    };

public:

    /// �e�N�X�`���^�ł��邩�𔻒� (LN_SVT_TEXTURE[*D])
    bool isTextureType() const { return (mTextureParam != NULL); }

    /// setTexture() �Őݒ肳�ꂽ���
    const TextureParam& getTextureParam() const { return *mTextureParam; }

private:

    Shader*                     mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
    LRefTString                 mName;
    LRefTString                 mSemanticName;
    ShaderAnnotationArray       mAnnotationArray;
    ShaderAnnotationMap         mAnnotationMap;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
    TextureParam*               mTextureParam;      ///< (�e�N�X�`���^�̏ꍇ�̂ݍ쐬)
};

//=============================================================================
// �� ShaderAnnotation �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�����̃A�m�e�[�V�����̃N���X
//*/
//=============================================================================
class ShaderAnnotation
    : public Base::ReferenceObject
    , public IShaderVariable
{
public:

    /// �R���X�g���N�^
    ShaderAnnotation( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ );

    /// �f�X�g���N�^
    virtual ~ShaderAnnotation() {}

    LN_REFOBJ_METHODS;

public:

    /// �u�[���l�̎擾
	virtual LNRESULT getBool( bool* b_ );

    /// �u�[���l�̐ݒ�
    virtual LNRESULT setBool( bool b_ ) { return LN_OK; }

    /// �����l�̎擾
	virtual LNRESULT getInt( int* value_ );

    /// �����l�̐ݒ�
	virtual LNRESULT setInt( int value_ ) { return LN_OK; }

    /// �����l�̎擾
    virtual LNRESULT getFloat( lnfloat* value_ );

    /// �����l�̐ݒ�
    virtual LNRESULT setFloat( lnfloat value_ ) { return LN_OK; }

    /// �x�N�g���̎擾
	virtual LNRESULT getVector( LVector4* vec_ );

    /// �x�N�g���̐ݒ�
	virtual LNRESULT setVector( const LVector4& vec_ ) { return LN_OK; }

    /// �x�N�g���z��̎擾
	virtual LNRESULT getVectorArray( LVector4* vec_, u32 count_ );
    
    /// �x�N�g���z��̐ݒ�
	virtual LNRESULT setVectorArray( const LVector4* vec_, u32 count_ ) { return LN_OK; }

    /// �s��̎擾
    virtual LNRESULT getMatrix( LMatrix* matrix_ );

    /// �s��̐ݒ�
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) { return LN_OK; }

    /// �s��̔z��̎擾
	virtual LNRESULT getMatrixArray( LMatrix* matrices_, u32 count_ );

    /// �s��̔z��̐ݒ�
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ ) { return LN_OK; }

    /// �e�N�X�`���̎擾
    //virtual LNRESULT getTexture( ITexture** texture_ ) {}

    /// �e�N�X�`���̐ݒ�
    virtual LNRESULT setTexture( ITexture* texture_ ) { return LN_OK; }

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) { return LN_OK; }

    /// ������̎擾
    virtual LNRESULT getString( const lnChar** str_ );

    /// ������̐ݒ�
	virtual LNRESULT setString( const lnChar* str_ ) { return LN_OK; }

    /// �ϐ����̎擾
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// �Z�}���e�B�N�X���̎擾
    virtual const lnChar* getSemanticName() const { return NULL; }

    /// �^���̎擾
    virtual const LNShaderVariableTypeDesc& getTypeDesc() const { return mVariableTypeDesc; }

    /// �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
    virtual IShaderVariable* getAnnotationByIndex( u32 index_ ) { return NULL; }

    /// ���O�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByName( const char* name_ ) { return NULL; }

private:

    Shader*                     mShader;
    ID3DXEffect*                mDxEffect;
    D3DXHANDLE                  mHandle;
    LRefTString                 mName;
    LNShaderVariableTypeDesc    mVariableTypeDesc;
    LRefTString                 mString;            ///< String �^�̃A�m�e�[�V�����̏ꍇ�̕�����l (wchar_t �擾���̍������p)
};

//=============================================================================
// �� ShaderTechnique �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̃e�N�j�b�N�������N���X
//*/
//=============================================================================
class ShaderTechnique
    : public Base::ReferenceObject
    , public IShaderTechnique
{
public:

    /// �R���X�g���N�^
    ShaderTechnique( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE tech_ );

    /// �f�X�g���N�^
    virtual ~ShaderTechnique();

    LN_REFOBJ_METHODS;

public:

    /// ���O�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// �e�N�j�b�N�̖��O���擾����
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// �e�N�j�b�N���̃p�X�̐����擾����
	virtual u32 getPasses() const { return mShaderPassArray.size(); }

    /// ���O�ɂ���ăp�X���擾����
    virtual IShaderPass* getPassByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ăp�X���擾����
	virtual IShaderPass* getPassByIndex( lnU32 index_ );

private:

    Shader*                 mShader;
    ID3DXEffect*            mDxEffect;
    D3DXHANDLE              mTechnique;
    u32                     mPasses;
    LRefTString             mName;
   
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;

    ShaderPassArray         mShaderPassArray;
    ShaderPassMap           mShaderPassMap;
};

//=============================================================================
// �� ShaderPass �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�������̃p�X�������N���X
//*/
//=============================================================================
class ShaderPass
    : public Base::ReferenceObject
    , public IShaderPass
{
public:

    /// �R���X�g���N�^
    ShaderPass( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_, u32 pass_index_, D3DXHANDLE tech_ );

    /// �f�X�g���N�^
    virtual ~ShaderPass();

    LN_REFOBJ_METHODS;

public:

    /// �p�X�̖��O���擾����
    virtual const lnChar* getName() const { return mName.c_str(); }

    /// ���O�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ăA�m�e�[�V�������擾����
	virtual IShaderVariable* getAnnotationByIndex( u32 index_ );

    /// �p�X�̓K�p���J�n����
	virtual LNRESULT begin();

    /// �p�X�̓K�p���I������
	virtual LNRESULT end();

    /// �p�X�̎��s���ɕω������X�e�[�g���f�o�C�X�ɑ��M����
	virtual void commit();

private:

    Shader*                 mShader;
    ID3DXEffect*            mDxEffect;
    D3DXHANDLE              mHandle;
    D3DXHANDLE              mTechnique;
    u32                     mPassIndex;
    LRefTString             mName;
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================