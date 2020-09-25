//=============================================================================
//�y GLShader �z
//-----------------------------------------------------------------------------
///**
//  @file       GLShader.h
//  @brief      GLShader
//  @author     Riki
//*/
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
#include "GLUtil.h"
#include "GLShaderManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace OpenGL
{
class GraphicsDevice;
class TextureBase;
class ShaderManager;
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
    : public GLResource//public Base::ReferenceObject
    , public ShaderCache::CachedObject
    , public IShader
{
public:

    /// �R���X�g���N�^
    Shader( ShaderManager* manager_ );

    /// �f�X�g���N�^
    virtual ~Shader();

    LN_CACHED_REFOBJ_METHODS;

public:

    /// ������
    void initialize( const lnChar* filename_ );

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

    const LRefTString& getName() const { return mFilename; }

    ///// �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    //void onLostDevice();

    ///// �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    //void onResetDevice();

    /// �v���O�����I�u�W�F�N�g�̎擾
    GLuint getGLProgram() { return mProgram; }

    /// (0�X�^�[�g)
    int getTextureUnitIncr() { int i = mLastTextureUnit; ++mLastTextureUnit; return i; }

    /// GraphicsDevice �̎擾
    GraphicsDevice* getGraphicsDevice() { return mManager->getGraphicsDevice(); }

    /// �p�X�� begin()�Acommit() ����Ă΂��
    void onCommit();


    virtual void onCommitGLResource();
    virtual void onDisposeGLResource();
    void onLostDevice() { GLResource::onLostDevice(); }
    void onResetDevice() { GLResource::onResetDevice(); GLResource::commitChanges(); }

private:

    // �ȉ�3�͕`��^�X�N�Ɠ����X���b�h����ĂԁB
    void _createOnInitial();    // �������͂܂�����ȏ�Ԃ���쐬
    void _createOnRefresh();    // ShaderVariable ���͏o���オ���Ă���O��
    void _buildFromFile();      // ���̂Ƃ���A_buildFromFile()��_build()��_compileShader() �Ńr���h
    void _build( const char* code_ );
    bool _compileShader( GLuint shader_, int code_count_, const char** code_, GLint* size_ );

private:

    ShaderManager*          mManager;
    LRefTString             mFilename;

    GLuint                  mVertexShader;      // �����o�Ɏ��K�v�Ȃ������H
    GLuint                  mFragmentShader;
    GLuint                  mProgram;

    LNShaderCompileResult   mCompileResult;
    LRefTString             mCompileErrors;
    LNShaderDesc            mDesc;

    ShaderTechnique*        mShaderTechnique;   ///< �e�N�j�b�N�͂ЂƂ���
    //ShaderTechniqueArray    mShaderTechniqueArray;
    //ShaderTechniqueMap      mShaderTechniqueMap;
    ShaderVariableArray     mShaderVariableArray;
    ShaderVariableMap       mShaderVariableMap;

    std::string             mScript;

    int                     mLastTextureUnit;

private:

    // �`��X���b�h���g���ꍇ�ɂ̂ݎg�p
    class Initializer
    {
    public:

        Initializer(Shader* shader_)
            : mShader       (shader_)
            , mInitialized  (false)
        {}

        /// call by delegate
        void initialize()
        {
            mShader->_createOnInitial();
            mInitialized.setTrue();
        }

        /// call by Shader class
        void waitInitialized() { mInitialized.wait(); }

    private:

        Shader*     mShader;
        Thread::EventFlag   mInitialized;
    };

    friend class Initializer; 
};

/*
enum LNShaderVariableSourceType
{
    LN_SVST_BOOL = 0,
    LN_SVST_INT,
    LN_SVST_FLOAT,
    LN_SVST_VECTOR,
    LN_SVST_VECTOR_ARRAY,
    LN_SVST_MATRIX,
    LN_SVST_MATRIX_ARRAY,
    LN_SVST_TEXTURE,
    LN_SVST_STRING,
};

/// �A�m�e�[�V�����̐������
struct LNShaderAnnotationSource
{
    LNShaderVariableType    Type;
    LRefTString             AnnotationName;
    
    union
    {
        struct
        {
            bool        Value;
        } Bool;
        
        struct
        {
            int         Value;
        } Int;
        
        struct
        {
            lnfloat     Value;
        } Float;
        
        struct
        {
            LVector4    Value;
        } Vector;
        
        struct
        {
            std::vector< LVector4 > Values;
        } VectorArray;
        
        struct
        {
            LMatrix     Value;
        } Matrix;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } MatrixArray;
        
        //struct
        //{
        //} Texture;
        
        struct
        {
            LRefTString     Value;
        } String;
    };
};

typedef std::vector< LNShaderAnnotationSource > ShaderAnnotationSourceArray;

/// �V�F�[�_�ϐ��ɕt��������
struct LNShaderVariableAdditionalData
{
    LNShaderVariableType        Type;
    LRefTString                 VariableName;       ///< �Ώۂ̕ϐ��� (������Ȃ������ꍇ�̓G���[)
    LRefTString                 Semantics;          ///< �Z�}���e�B�N�X
    ShaderAnnotationSourceArray AnnotationArray;    ///< �A�m�e�[�V����
    
    union
    {
        struct
        {
        } Bool;
        
        struct
        {
            int         Value;
        } Int;
        
        struct
        {
            lnfloat     Value;
        } Float;
        
        struct
        {
            LVector4    Value;
        } Vector;
        
        struct
        {
            std::vector< LVector4 > Values;
        } VectorArray;
        
        struct
        {
            LMatrix     Value;
        } Matrix;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } MatrixArray;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } Texture;
        
        struct
        {
            std::vector< LMatrix >  Values;
        } String;
    };
};
*/

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
    ShaderVariable( Shader* shader_, int var_index_ );

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

    void onCreateGLResource();
    void onDisposeGLResource();

    /// �p�X�� begin()�Acommit() ����Ă΂��
    void onCommit();

    // �ȉ��A�ǉ��ݒ�p�B�X�N���v�g����ݒ肷�邱�ƑO��Ȃ̂� char

    /// (Shader �ŃZ�}���e�B�N�X������ϐ�����������@�\����������ꍇ��map�X�V����K�v������̂Œ���)
    void setSemanticName( const char* name_ ) { mSemanticName = name_; }

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

private:

    struct GLSamplerState
    {
        int             TextureUnitIndex;   ///< �T���v���^�̏ꍇ�A�ǂ̃e�N�X�`�����j�b�g���g���� (0�X�^�[�g)
        TextureBase*    Texture;           ///< �T���v���^�̏ꍇ�� setTexture() ���ꂽ�e�N�X�`��
        GLint           MagFilter;
        GLint           MinFilter;
        GLint           AddressU;
        GLint           AddressV;
    };

    enum UsableType
    {
        TYPE_BOOL = 0,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_VECTOR,
        TYPE_VECTORARRAY,
        TYPE_MATRIX,
        TYPE_MATRIXARRAY,
        //TYPE_TEXTURE,
    };

    struct
    {
        UsableType Type;    ///< �e�N�X�`���^�� TYPE_BOOL �ɂȂ�̂Œ��ӁB

        union
        {
            bool        Bool;
            int         Int;
            lnfloat     Float;
            LVector4*   Vector;         // new

            struct
            {
                LVector4*   Vectors;    // new
                lnU32       Count;
            } VectorArray;

            LMatrix*    Matrix;         // new

            struct
            {
                LMatrix*    Matrices;   // new
                lnU32       Count;
            } MatrixArray;
            
            //ITexture*   Texture;        // mSamplerState �ň����Ă���̂ł����ł̓i�V

            //String
        };   
    } mVariableBuffer;

private:

    Shader*                     mShader;
    int                         mVarIndex;
    GLint                       mLocation;
    LRefTString                 mName;
    LRefTString                 mSemanticName;
    ShaderAnnotationArray       mAnnotationArray;
    ShaderAnnotationMap         mAnnotationMap;
    LNShaderVariableTypeDesc    mVariableTypeDesc;

    GLSamplerState*             mSamplerState;      ///< �T���v���^�̏ꍇ�ɍ쐬�����


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

    ShaderAnnotation( Shader* shader_, const char* name_, bool v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, int v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, float v_ );
    ShaderAnnotation( Shader* shader_, const char* name_, float x_, float y_, float z_, float w_ );
    ShaderAnnotation( Shader* shader_, const char* name_, const char* v_ );

    LN_REFOBJ_METHODS;

public:

    /// �u�[���l�̎擾
    virtual LNRESULT getBool( bool* b_ ) { *b_ = mValue.Bool; return LN_OK; }

    /// �u�[���l�̐ݒ�
    virtual LNRESULT setBool( bool b_ ) { return LN_OK; }

    /// �����l�̎擾
	virtual LNRESULT getInt( int* value_ ) { *value_ = mValue.Int; return LN_OK; }

    /// �����l�̐ݒ�
	virtual LNRESULT setInt( int value_ ) { return LN_OK; }

    /// �����l�̎擾
    virtual LNRESULT getFloat( lnfloat* value_ ) { *value_ = mValue.Float; return LN_OK; }

    /// �����l�̐ݒ�
    virtual LNRESULT setFloat( lnfloat value_ ) { return LN_OK; }

    /// �x�N�g���̎擾
    virtual LNRESULT getVector( LVector4* vec_ ) { vec_->set( mValue.Vector.x, mValue.Vector.y, mValue.Vector.z, mValue.Vector.w ); return LN_OK; }

    /// �x�N�g���̐ݒ�
	virtual LNRESULT setVector( const LVector4& vec_ ) { return LN_OK; }

    /// �x�N�g���z��̎擾
    virtual LNRESULT getVectorArray( LVector4* vec_, u32 count_ ) { return LN_OK; }
    
    /// �x�N�g���z��̐ݒ�
	virtual LNRESULT setVectorArray( const LVector4* vec_, u32 count_ ) { return LN_OK; }

    /// �s��̎擾
    virtual LNRESULT getMatrix( LMatrix* matrix_ ) { return LN_OK; }

    /// �s��̐ݒ�
    virtual LNRESULT setMatrix( const LMatrix& matrix_ ) { return LN_OK; }

    /// �s��̔z��̎擾
    virtual LNRESULT getMatrixArray( LMatrix* matrices_, lnU32 count_ ) { return LN_OK; }

    /// �s��̔z��̐ݒ�
	virtual LNRESULT setMatrixArray( const LMatrix* matrices_, u32 count_ ) { return LN_OK; }

    /// �e�N�X�`���̎擾
    //virtual LNRESULT getTexture( ITexture** texture_ ) {}

    /// �e�N�X�`���̐ݒ�
    virtual LNRESULT setTexture( ITexture* texture_ ) { return LN_OK; }

    /// �e�N�X�`���̐ݒ�
	virtual LNRESULT setTexture( ITexture* texture_, const LSamplerState& state_ ) { return LN_OK; }

    /// ������̎擾
    virtual LNRESULT getString( const lnChar** str_ ) { *str_ = mStringValue.c_str(); return LN_OK; }

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

public:

    

private:

    Shader*                     mShader;
    LRefTString                 mName;
    LNShaderVariableTypeDesc    mVariableTypeDesc;

    union
    {
        bool        Bool;
        int         Int;
        lnfloat     Float;
        struct
        {
            float x, y, z, w;
        } Vector;
              
    } mValue;

    LRefTString mStringValue;   ///< �R�s�[�R���X�g���N�^�]�X�ŃG���[�ɂȂ�̂� union ����͂���
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
    ShaderTechnique( Shader* shader_ );

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
	virtual u32 getPasses() const { return 1; }

    /// ���O�ɂ���ăp�X���擾����
    virtual IShaderPass* getPassByName( const lnChar* name_ );

    /// �C���f�b�N�X�ɂ���ăp�X���擾����
	virtual IShaderPass* getPassByIndex( lnU32 index_ );

public:

    void onCreateGLResource();
    void onDisposeGLResource();

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

private:

    Shader*                 mShader;

    u32                     mPasses;
    LRefTString             mName;
   
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;

    ShaderPass*             mShaderPass;

    //ShaderPassArray         mShaderPassArray;
    //ShaderPassMap           mShaderPassMap;
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
    ShaderPass( Shader* shader_ );

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

public:

    void onCreateGLResource();
    void onDisposeGLResource();

    void addAnnotationBool( const char* name_, bool v_ );
    void addAnnotationInt( const char* name_, int v_ );
    void addAnnotationFloat( const char* name_, float v_ );
    void addAnnotationVector( const char* name_, float x_, float y_, float z_, float w_ );
    void addAnnotationString( const char* name_, const char* v_ );

    GLint getUsageEntryLocation( LNVertexDeclUsage usage_ ) { return mUsageEntryLocation[usage_]; }

private:

   /* struct UsageEntry
    {
        LNVertexDeclUsage   Usage;
        
    };*/

private:

    Shader*                 mShader;
    LRefTString             mName;
    ShaderAnnotationArray   mAnnotationArray;
    ShaderAnnotationMap     mAnnotationMap;
    GLint                   mUsageEntryLocation[LN_VERTEXDECLUSAGE_MAX];
    //UsageEntry              mUsageEntrys[LN_VERTEXDECLUSAGE_MAX];
    bool                    mBegan;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace OpenGL
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================