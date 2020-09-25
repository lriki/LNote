//=============================================================================
//【 Shader 】
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Geometry/Vector4.h"
#include "../../Geometry/Matrix.h"
#include "../Common/GraphicsUtil.h"
#include "GraphicsDevice.h"
#include "DX9Module.h"
#include "Texture.h"
#include "Shader.h"

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

//---------------------------------------------------------------------
// ● 変数クラスと変数型の変換
//---------------------------------------------------------------------
void D3DXParamDescToLNParamDesc(
    const D3DXPARAMETER_DESC&   dx_desc_,
    LNShaderVariableClass*      ln_class_,
    LNShaderVariableType*       ln_type_ )
{
    // 変数クラス (LN_SVC_SAMPLER は Type がテクスチャまたはサンプラ型のときこの次の switch でセットする)
    switch ( dx_desc_.Class )
    {
        case D3DXPC_SCALAR:         
        	*ln_class_ = LN_SVC_SCALAR; break;
        case D3DXPC_VECTOR:         
        	*ln_class_ = LN_SVC_VECTOR; break;
        case D3DXPC_MATRIX_ROWS:    
        	*ln_class_ = LN_SVC_MATRIX; break;
        case D3DXPC_MATRIX_COLUMNS: 
        	*ln_class_ = LN_SVC_MATRIX; break;
        case D3DXPC_OBJECT:         
        	*ln_class_ = LN_SVC_OBJECT; break;
        case D3DXPC_STRUCT:         
        	*ln_class_ = LN_SVC_STRUCT; break;
        //                                       LN_SVC_ARRAY;
        //                                       LN_SVC_SAMPLER;
        //                                       LN_SVC_BUFFER;
        default:                    
        	*ln_class_ = LN_SVC_UNKNOWN; break;
    }

    // 変数型
    switch ( dx_desc_.Type )
    {
        case D3DXPT_VOID:           
        	*ln_type_ = LN_SVT_VOID; break;
        case D3DXPT_BOOL:           
        	*ln_type_ = LN_SVT_BOOL; break;
        case D3DXPT_INT:            
        	*ln_type_ = LN_SVT_INT; break;
        case D3DXPT_FLOAT:          
        	*ln_type_ = LN_SVT_FLOAT; break;
        case D3DXPT_STRING:         
        	*ln_type_ = LN_SVT_STRING; break;
        case D3DXPT_TEXTURE:        
        	*ln_type_ = LN_SVT_TEXTURE; break;
        case D3DXPT_TEXTURE1D:      
        	*ln_type_ = LN_SVT_TEXTURE1D; break;
        case D3DXPT_TEXTURE2D:      
        	*ln_type_ = LN_SVT_TEXTURE2D; break;
        case D3DXPT_TEXTURE3D:      
        	*ln_type_ = LN_SVT_TEXTURE3D; break;
        case D3DXPT_TEXTURECUBE:    
        	*ln_type_ = LN_SVT_TEXTURECUBE; break;
        case D3DXPT_SAMPLER:        
        	*ln_type_ = LN_SVT_SAMPLER; break;
        case D3DXPT_SAMPLER1D:      
        	*ln_type_ = LN_SVT_SAMPLER1D; break;
        case D3DXPT_SAMPLER2D:      
        	*ln_type_ = LN_SVT_SAMPLER2D; break;
        case D3DXPT_SAMPLER3D:      
        	*ln_type_ = LN_SVT_SAMPLER3D; break;
        case D3DXPT_SAMPLERCUBE:    
        	*ln_type_ = LN_SVT_SAMPLERCUBE; break;
        case D3DXPT_PIXELSHADER:    
        	*ln_type_ = LN_SVT_PIXELSHADER; break;
        case D3DXPT_VERTEXSHADER:   
        	*ln_type_ = LN_SVT_VERTEXSHADER; break;
        case D3DXPT_PIXELFRAGMENT:  
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        case D3DXPT_VERTEXFRAGMENT: 
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        case D3DXPT_UNSUPPORTED:    
        	*ln_type_ = LN_SVT_UNKNOWN; break;
        default:                    
        	*ln_type_ = LN_SVT_UNKNOWN; break;
    }
}

//=============================================================================
// ■ Shader クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Shader::Shader( GraphicsDevice* manager_ )
        : mManager          ( manager_ )
        , mDxEffect         ( NULL )
        , mCompileResult    ( LN_SHADERRESULT_ERROR )
    {
        LN_SAFE_ADDREF( mManager );
    } 

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Shader::~Shader()
    {
        mManager->removeShader( this );

        // ShaderTechnique 解放
        for ( u32 i = 0; i < mShaderTechniqueArray.size(); ++i )
        {
            SAFE_DELETE( mShaderTechniqueArray[ i ] );
        }

        // ShaderVariable 解放
        for ( u32 i = 0; i < mShaderVariableArray.size(); ++i )
        {
            SAFE_DELETE( mShaderVariableArray[ i ] );
        }

        SAFE_RELEASE( mDxEffect );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Shader::initialize( const char* code_ )
    {
        // 最初に成功にしておく
        mCompileResult = LN_SHADERRESULT_SUCCESS;

        // ソース分割
        LNFXShaderSource source;
        source.initialize(code_, LNFXShaderSource::SHADER_HLSL);

        // 定義マクロ
        D3DXMACRO deflist[] =
        {
            { LNFXShaderSource::HLSL_MACRO,	"1" },
	        { NULL, NULL },
        };

        // シェーダプログラムのコンパイル
		//ShaderInclude dx_include;
        ID3DXBuffer* buffer = NULL;
        HRESULT hr = DX9Module::getInstance()->D3DXCreateEffect(
            mManager->getDX9Device(),
            source.getSourceData(),
            source.getSourceDataSize(),
            deflist,
            NULL,
            0,
            NULL,
            &mDxEffect,
            &buffer );
        // 失敗
        if ( FAILED( hr ) && buffer )
		{
            mCompileResult = LN_SHADERRESULT_ERROR;
			char* c = static_cast< char* >( buffer->GetBufferPointer() );
            mCompileErrors = c;
            SAFE_RELEASE( buffer );
			LN_SETDXERR_R_S( D3DERR_INVALIDCALL, "< LShaderImpl::create >\nD3DXCreateEffect\n\n【シェーダプログラムのコンパイルエラー】\n\n%s", mCompileErrors.c_str() );
		}
        // 警告がある
		else if ( buffer )
		{
            mCompileResult = LN_SHADERRESULT_WARNING;
            char* c = static_cast< char* >( buffer->GetBufferPointer() );
            mCompileErrors = c;
            SAFE_RELEASE( buffer );
		}

        //---------------------------------------------------------
        // すべてのテクニックを列挙
        D3DXHANDLE tech = NULL;
        D3DXHANDLE next = NULL;
        do
        {
            mDxEffect->FindNextValidTechnique( tech, &next );
            if ( next )
            {
                ShaderTechnique* t = LN_NEW ShaderTechnique( this, mDxEffect, next );
                mShaderTechniqueMap.insert( ShaderTechniquePair( t->getName(), t ) );
                mShaderTechniqueArray.push_back( t );
            }
            tech = next;
        } while ( tech );

        //---------------------------------------------------------
        // すべての変数を列挙
        D3DXHANDLE handle;
        UINT idx = 0;
        while ( true )
        {
            handle = mDxEffect->GetParameter( NULL, idx );
            if ( !handle ) break;

            ShaderVariable* v = LN_NEW ShaderVariable( this, mDxEffect, handle );
            mShaderVariableMap.insert( ShaderVariablePair( v->getName(), v ) );
            mShaderVariableArray.push_back( v );
            ++idx;

            if ( v->isTextureType() )
            {
                mTextureVariableArray.push_back( v );
            }
        }

        //---------------------------------------------------------
        // シェーダ情報
        D3DXEFFECT_DESC desc;
        mDxEffect->GetDesc( &desc );
        mDesc.ParameterNum = desc.Parameters;
        mDesc.TechniqueNum = desc.Techniques;

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ブール値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setBool( const lnChar* name_, bool b_ )
    {
        return getVariableByName( name_ )->setBool( b_ );
    }

    //---------------------------------------------------------------------
    // ● 整数値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setInt( const lnChar* name_, int value_ )
    {
        return getVariableByName( name_ )->setInt( value_ );
    }

    //---------------------------------------------------------------------
    // ● 実数値の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setFloat( const lnChar* name_, lnfloat value_ )
    {
        return getVariableByName( name_ )->setFloat( value_ );
    }

    //---------------------------------------------------------------------
    // ● ベクトルの設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setVector( const lnChar* name_, const LVector4& vec_ )
    { 
        return getVariableByName( name_ )->setVector( vec_ );
    }

    //---------------------------------------------------------------------
    // ● ベクトル配列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setVectorArray( const lnChar* name_, const LVector4* vec_, u32 count_ )
    {
        return getVariableByName( name_ )->setVectorArray( vec_, count_ );
    }

    //---------------------------------------------------------------------
    // ● 行列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrix( const lnChar* name_, const LMatrix& matrix_ )
    {
        return getVariableByName( name_ )->setMatrix( matrix_ );
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setMatrixArray( const lnChar* name_, const LMatrix* matrices_, u32 count_ )
    {
        return getVariableByName( name_ )->setMatrixArray( matrices_, count_ );
    }
   
    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
	LNRESULT Shader::setTexture( const lnChar* name_, ITexture* texture_ )
    {
        return getVariableByName( name_ )->setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    // ● 名前によって変数を取得する
    //---------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByName( const lnChar* name_ )
    {
        ShaderVariableMap::iterator it = mShaderVariableMap.find( name_ );
        if ( it != mShaderVariableMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによって変数を取得する
    //---------------------------------------------------------------------
    IShaderVariable* Shader::getVariableByIndex( u32 index_ )
    {
        if ( index_ >= mShaderVariableArray.size() ) return NULL;
        return mShaderVariableArray[ index_ ];
    }

    //// セマンティクスによって変数を取得する
    //IShaderVariable* Shader::getVariableBySemantic( const char* name_ )
    //{
    //    return NULL;
    //}
    
    //---------------------------------------------------------------------
    // ● 名前によってテクニックを取得する
    //---------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByName( const lnChar* name_ )
    {
        ShaderTechniqueMap::iterator it = mShaderTechniqueMap.find( name_ );
        if ( it != mShaderTechniqueMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってテクニックを取得する
    //---------------------------------------------------------------------
    IShaderTechnique* Shader::getTechniqueByIndex( u32 index_ )
    {
        if ( index_ >= mShaderTechniqueArray.size() ) return NULL;
        return mShaderTechniqueArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● デバイスリセット直前 GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT Shader::onLostDevice()
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->OnLostDevice() );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ●  デバイスリセット後に GraphicsDevice から呼ばれる
    //---------------------------------------------------------------------
    LNRESULT Shader::onResetDevice()
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->OnResetDevice() );
        return LN_OK;
    }

//=============================================================================
// ■ ShaderVariable クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderVariable::ShaderVariable( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ )
        : mShader       ( shader_ )
        , mDxEffect     ( dx_effect_ )
        , mHandle       ( handle_ )
        , mTextureParam ( NULL )
    {
        // 変数名とセマンティクス名
        D3DXPARAMETER_DESC desc;
        mDxEffect->GetParameterDesc( mHandle, &desc );
        mName         = LRefTString( desc.Name );
        mSemanticName = LRefTString( desc.Semantic );

        // 変数情報
        D3DXParamDescToLNParamDesc(
            desc, 
            &mVariableTypeDesc.Class,
            &mVariableTypeDesc.Type );
        mVariableTypeDesc.Rows      = desc.Rows;
        mVariableTypeDesc.Columns   = desc.Columns;
        mVariableTypeDesc.Elements  = desc.Elements;
        mVariableTypeDesc.Shared    = ( desc.Flags & D3DX_PARAMETER_SHARED );

        // すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( u32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mHandle, i );
            if ( anno ) // 念のため
            {
                ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, mDxEffect, anno );
                mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }

        // テクスチャ型の場合はテクスチャとサンプラステートを記憶する構造体を作る
        if ( LN_SVT_TEXTURE_BEGIN <= mVariableTypeDesc.Type && mVariableTypeDesc.Type <= LN_SVT_TEXTURE_END )
        {
            mTextureParam = LN_NEW TextureParam;
            mTextureParam->Texture = NULL;
        }

    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderVariable::~ShaderVariable()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }

        SAFE_DELETE( mTextureParam );
    }

    //---------------------------------------------------------------------
    // ● ブール値の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getBool( bool* b_ )
    {
        HRESULT hr;
        BOOL b;
        LN_DXCALL_R( mDxEffect->GetBool( mHandle, &b ) );
        *b_ = ( b != FALSE );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ブール値の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setBool( bool b_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetBool( mHandle, b_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 整数値の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getInt( int* value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetInt( mHandle, value_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 整数値の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setInt( int value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetInt( mHandle, value_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実数値の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getFloat( lnfloat* value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetFloat( mHandle, value_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実数値の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setFloat( lnfloat value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetFloat( mHandle, value_ ) );
        return LN_OK;;
    }

    //---------------------------------------------------------------------
    // ● ベクトルの取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVector( LVector4* vec_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetVector( mHandle, (D3DXVECTOR4*)vec_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトルの設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setVector( const LVector4& vec_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetVector( mHandle, (D3DXVECTOR4*)&vec_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトル配列の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getVectorArray( LVector4* vec_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetVectorArray( mHandle, (D3DXVECTOR4*)vec_, count_ ) );
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● ベクトル配列の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setVectorArray( const LVector4* vec_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetVectorArray( mHandle, (const D3DXVECTOR4*)vec_, count_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getMatrix( LMatrix* matrix_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetMatrix( mHandle, (D3DXMATRIX*)matrix_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setMatrix( const LMatrix& matrix_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetMatrix( mHandle, (const D3DXMATRIX*)&matrix_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の取得
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::getMatrixArray( LMatrix* matrices_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetMatrixArray( mHandle, (D3DXMATRIX*)matrices_, count_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setMatrixArray( const LMatrix* matrices_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetMatrixArray( mHandle, (const D3DXMATRIX*)matrices_, count_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
	LNRESULT ShaderVariable::setTexture( ITexture* texture_ )
    {
        HRESULT hr;
        if ( texture_ )
        {
            // getDxTexture() が NULL でもエラーにはならない
            LN_DXCALL_R( mDxEffect->SetTexture( mHandle, ( (Texture*)texture_ )->getDxTexture() ) );
        }
        else
        {
            LN_DXCALL_R( mDxEffect->SetTexture( mHandle, NULL ) );
        }

        mTextureParam->Texture = texture_;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setTexture( ITexture* texture_, const LSamplerState& state_ )
    {
        mTextureParam->SamplerState = state_;
        return setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    // ● 文字列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::getString( const char** str_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetString( mHandle, str_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 文字列の設定
    //---------------------------------------------------------------------
    LNRESULT ShaderVariable::setString( const char* str_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetString( mHandle, str_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 変数名の取得
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getName() const
    {
        return mName.c_str();
    }

    //---------------------------------------------------------------------
    // ● セマンティクス名の取得
    //---------------------------------------------------------------------
    const lnChar* ShaderVariable::getSemanticName() const
    {
        return mSemanticName.c_str();
    }

    //---------------------------------------------------------------------
    // ● 型情報の取得
    //---------------------------------------------------------------------
    const LNShaderVariableTypeDesc& ShaderVariable::getTypeDesc() const
    {
        return mVariableTypeDesc;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) return NULL;
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderVariable::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

//=============================================================================
// ■ ShaderAnnotation クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderAnnotation::ShaderAnnotation( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_ )
        : mShader       ( shader_ )
        , mDxEffect     ( dx_effect_ )
        , mHandle       ( handle_ )
    {
        // 変数名とセマンティクス名
        D3DXPARAMETER_DESC desc;
        mDxEffect->GetParameterDesc( mHandle, &desc );
        mName         = LRefTString( desc.Name );

        // 変数情報
        D3DXParamDescToLNParamDesc(
            desc, 
            &mVariableTypeDesc.Class,
            &mVariableTypeDesc.Type );
        mVariableTypeDesc.Rows      = desc.Rows;
        mVariableTypeDesc.Columns   = desc.Columns;
        mVariableTypeDesc.Elements  = desc.Elements;

        // String 型の場合は wchar_t 取得時高速化のため、メンバに持っておく
        if ( mVariableTypeDesc.Type == LN_SVT_STRING )
        {
            const char* str = NULL;
            mDxEffect->GetString( mHandle, &str );
            mString = LRefTString( str );
        }
    }

    //---------------------------------------------------------------------
    // ● ブール値の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getBool( bool* b_ )
    {
        HRESULT hr;
        BOOL b;
        LN_DXCALL_R( mDxEffect->GetBool( mHandle, &b ) );
        *b_ = ( b != FALSE );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 整数値の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getInt( int* value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetInt( mHandle, value_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 実数値の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getFloat( lnfloat* value_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetFloat( mHandle, value_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトルの取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getVector( LVector4* vec_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetVector( mHandle, (D3DXVECTOR4*)vec_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ベクトル配列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getVectorArray( LVector4* vec_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetVectorArray( mHandle, (D3DXVECTOR4*)vec_, count_ ) );
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● 行列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getMatrix( LMatrix* matrix_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetMatrix( mHandle, (D3DXMATRIX*)matrix_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 行列の配列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getMatrixArray( LMatrix* matrices_, u32 count_ )
    {
        HRESULT hr;
        LN_DXCALL_R( mDxEffect->GetMatrixArray( mHandle, (D3DXMATRIX*)matrices_, count_ ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 文字列の取得
    //---------------------------------------------------------------------
    LNRESULT ShaderAnnotation::getString( const lnChar** str_ )
    {
        *str_ = mString.c_str();
        return LN_OK;
    }

//=============================================================================
// ■ ShaderTechnique クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderTechnique::ShaderTechnique( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE tech_ )
        : mShader       ( shader_ )
        , mDxEffect     ( dx_effect_ )
        , mTechnique    ( tech_ )
    {
        D3DXTECHNIQUE_DESC desc;
        mDxEffect->GetTechniqueDesc( mTechnique, &desc );
        mName = desc.Name;

        mPasses = desc.Passes;

        // すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( u32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mTechnique, i );
            if ( anno ) // 念のため
            {
                ShaderAnnotation* v = LN_NEW ShaderAnnotation( mShader, mDxEffect, anno );
                mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }

        // パス
        D3DXHANDLE pass;
        for ( u32 i = 0; i < mPasses; ++i )
        {
            pass = mDxEffect->GetPass( mTechnique, i );
            if ( pass ) // 念のため
            {
                ShaderPass* ps = LN_NEW ShaderPass( mShader, mDxEffect, pass, i, mTechnique );
                mShaderPassMap.insert( ShaderPassPair( ps->getName(), ps ) );
                mShaderPassArray.push_back( ps );
            }
        }
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderTechnique::~ShaderTechnique()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }

        for ( u32 i = 0; i < mShaderPassArray.size(); ++i )
        {
            SAFE_DELETE( mShaderPassArray[ i ] );
        }
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByName( const char* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderTechnique::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) { 
	return NULL; }
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● 名前によってパスを取得する
    //---------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByName( const lnChar* name_ )
    {
        ShaderPassMap::iterator it = mShaderPassMap.find( name_ );
        if ( it != mShaderPassMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってパスを取得する
    //---------------------------------------------------------------------
    IShaderPass* ShaderTechnique::getPassByIndex( u32 index_ )
    {
        if ( index_ >= mShaderPassArray.size() ) { 
	return NULL; }
        return mShaderPassArray[ index_ ];
    }

    /*
    //---------------------------------------------------------------------
    // ● テクニックの適用を開始する
    //---------------------------------------------------------------------
    LNRESULT ShaderTechnique::begin()
    {
        //printf( "ShaderTechnique this:%p mDxEffect:%p mTechnique:%p\n", this, mDxEffect );
        //printf( "%p:%p ,", mTechnique, mDxEffect->GetTechnique( 0 ) );

        //printf("< Begin(%p)", this);

        // テクニック適用
		HRESULT hr;
        LN_DXCALL_R( mDxEffect->SetTechnique( mTechnique ), "< ShaderTechnique::begin >" );

		// エフェクト開始
		UINT pass_num;
        LN_DXCALL_R( mDxEffect->Begin( &pass_num, 0 ), "< ShaderTechnique::begin >" );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● テクニックの適用を終了する
    //---------------------------------------------------------------------
    LNRESULT ShaderTechnique::end()
    {
        //printf(" End(%p) >\n", this);

        mDxEffect->End();
        return LN_OK;
    }
    */


//=============================================================================
// ■ ShaderPass クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    ShaderPass::ShaderPass( Shader* shader_, ID3DXEffect* dx_effect_, D3DXHANDLE handle_, u32 pass_index_, D3DXHANDLE tech_ )
        : mShader       ( shader_ )
        , mDxEffect     ( dx_effect_ )
        , mHandle       ( handle_ )
        , mTechnique    ( tech_ )
        , mPassIndex    ( pass_index_ )
    {
        D3DXPASS_DESC desc;
        mDxEffect->GetPassDesc( mHandle, &desc );
        mName = LRefTString( desc.Name );

        //LPCSTR sapmler_names[16];
        //UINT count = 0;
        //D3DXGetShaderSamplers(desc.pPixelShaderFunction, sapmler_names, &count);

        // すべてのアノテーションを配列と map に格納
        D3DXHANDLE anno;
        for ( u32 i = 0; i < desc.Annotations; ++i )
        {
            anno = mDxEffect->GetAnnotation( mHandle, i );
            if ( anno ) // 念のため
            {
                ShaderAnnotation* v = LN_NEW ShaderAnnotation( shader_, mDxEffect, anno );
                mAnnotationMap.insert( ShaderAnnotationPair( v->getName(), v ) );
                mAnnotationArray.push_back( v );
            }
        }
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    ShaderPass::~ShaderPass()
    {
        for ( u32 i = 0; i < mAnnotationArray.size(); ++i )
        {
            SAFE_DELETE( mAnnotationArray[ i ] );
        }
    }

    //---------------------------------------------------------------------
    // ● 名前によってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByName( const lnChar* name_ )
    {
        ShaderAnnotationMap::iterator it = mAnnotationMap.find( name_ );
        if ( it != mAnnotationMap.end() )
        {
            return it->second;
        }
        return NULL;
    }

    //---------------------------------------------------------------------
    // ● インデックスによってアノテーションを取得する
    //---------------------------------------------------------------------
    IShaderVariable* ShaderPass::getAnnotationByIndex( u32 index_ )
    {
        if ( index_ >= mAnnotationArray.size() ) return NULL;
        return mAnnotationArray[ index_ ];
    }

    //---------------------------------------------------------------------
    // ● パスの適用を開始する
    //---------------------------------------------------------------------
    LNRESULT ShaderPass::begin()
    {
        //-----------------------------------------------------
        // テクスチャ型の ShaderVariable が持つサンプラステートを設定
        
        ShaderVariableArray& var_ary = mShader->getTextureVariableArray(); 
        IDirect3DDevice9* device;
        mDxEffect->GetDevice( &device );
        for ( int i = 0; i < LN_MAX_SAMPLER_STAGE_INDEX; ++i )
        {
            IDirect3DBaseTexture9* tex;
            device->GetTexture( i, &tex );

            if ( tex )
            {
                ln_foreach( ShaderVariable* v, var_ary )
                {
                    if ( tex == v->getTextureParam().Texture->getIDirect3DTexture9() )
                    {
                        mShader->getGraphicsDevice()->setSamplerState( i, v->getTextureParam().SamplerState );
                    }
                }

                SAFE_RELEASE( tex );
            }
        }
        SAFE_RELEASE( device );

        //-----------------------------------------------------
        // 適用

        HRESULT hr;
        UINT pass_num;


        hr = mDxEffect->SetTechnique( mTechnique );
        hr = mDxEffect->Begin( &pass_num, 0 );

        hr = mDxEffect->BeginPass( mPassIndex );


        


        
#if 0   // サンプラステートインデックス探し


#pragma comment(lib, "d3dx9.lib")

        IDirect3DDevice9* dx_device;
        mDxEffect->GetDevice( &dx_device );

        //IDirect3DVertexShader9* vs;
        //dx_device->GetVertexShader( &vs );


        IDirect3DPixelShader9* ps;
        dx_device->GetPixelShader( &ps );


        UINT size;
        lnByte buf[2048] = {0};
        //vs->GetFunction( buf, &size );
        ps->GetFunction( buf, &size );
        _p(size);


        ID3DXConstantTable* ct;
        D3DXGetShaderConstantTable( (const DWORD*)buf, &ct );

        _p(ct);

        D3DXHANDLE param;
        D3DXCONSTANT_DESC desc;
        for (int i = 0; (param = ct->GetConstant(NULL, i)) != NULL; i++)
        {
            UINT count;
            ct->GetConstantDesc(param, &desc,&count);
            _p(count);
            _p(desc.Name);
            
        }


        D3DXHANDLE sampler = ct->GetConstantByName(NULL, "gTexSampler");
        

        _p((void*)sampler);

        _p(ct->Release());
        _p(ps->Release());
        //_p(vs->Release());
        _p(dx_device->Release());


        //if (1)
        //{
        //    D3DXHANDLE pass = mDxEffect->GetPass( mTechnique, mPassIndex );
        //    _p((void*)pass);
        //    {
        //        D3DXHANDLE parameterHandle;
        //        D3DXPARAMETER_DESC parameterDesc;

        //        for (int i = 0; (parameterHandle = mDxEffect->GetParameter (mTechnique, i)) != NULL; i++)
        //        {
        //            if (mDxEffect->GetParameterDesc (parameterHandle, &parameterDesc) != D3D_OK)
        //            break;

        //            printf (" Parameter %d: Name = '%s' Type = %d\n", i, parameterDesc.Name, parameterDesc.Type);
        //        }
        //    }
        //    //IDirect3DVertexShader9* vs;
        //    //mDxEffect->GetVertexShader( pass, &vs );

        //    //_p(vs);
        //    //_p(vs->Release());
        //    //D3DXHANDLE vs = mDxEffect->GetParameterByName( pass, "VertexShader" );
        //    
        //    //mDxEffect->GetVertexShader( pass, 
        //    //D3DXGetShaderConstantTable

        //        /*
        //        D3DXCONSTANT_DESC

        //    if (mName == "gTexSampler")
        //    {
        //        //D3DXHANDLE sampler = mDxEffect->GetParameterByName(NULL, "gTexSampler");
        //        D3DXHANDLE texture = mDxEffect->GetParameterByName(mHandle, "MINFILTER");

        //        _p(desc.Name);
        //        _p((void*)mHandle);
        //        _p((void*)texture);
        //        _p(desc.Type);
        //        _p(D3DXPT_SAMPLER);
        //        _p(desc.Rows);
        //        _p(desc.Columns);
        //        _p(desc.Elements);
        //        _p(desc.Annotations);
        //        _p(desc.StructMembers);
        //        _p(desc.Bytes);

        //        
        //    }
        //    */
        //}

#endif


        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● パスの適用を終了する
    //---------------------------------------------------------------------
    LNRESULT ShaderPass::end()
    {
        mDxEffect->EndPass();
        mDxEffect->End();
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● パスの実行中に変化したステートをデバイスに送信する
    //---------------------------------------------------------------------
    void ShaderPass::commit()
    {
        mDxEffect->CommitChanges();
    }

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