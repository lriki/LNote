//=============================================================================
//�y BindingValue �z
//-----------------------------------------------------------------------------
///**
//  @file       BindingValue.h
//  @brief      BindingValue
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Geometry/Vector4.h"
#include "INotifyPropertyChanged.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

class BindingSourceValue
{
public:

    enum Type
    {
        UNKNOWN = 0,
        STRING,
        INT,
        FLOAT,
        BOOL
    };

public:

    BindingSourceValue( const lnChar* value_ )
    {
        mType = STRING;
        mString = value_;
    }

    BindingSourceValue( const LRefTString& value_ )
    {
        mType = STRING;
        mString = value_;
    }

    BindingSourceValue( int value_ )
    {
        mType = STRING;
        mInt = value_;
    }

    BindingSourceValue( float value_ )
    {
        mType = STRING;
        mFloat = value_;
    }

    BindingSourceValue( bool value_ )
    {
        mType = STRING;
        mBool = value_;
    }

public:

    Type getType() const
    {
        return mType;
    }

    const lnChar* getString() const
    {
        return (mType == STRING) ? mString.c_str() : NULL;
    }

    int getInt() const
    {
        return (mType == INT) ? mInt : 0;
    }

    float getFloat() const
    {
        return (mType == FLOAT) ? mFloat : 0;
    }

    int getBool() const
    {
        return (mType == BOOL) ? mBool : 0;
    }

private:

    Type    mType;

    LRefTString   mString;
    int                 mInt;
    float               mFloat;
    bool                mBool;

};

enum BindingMode
{
    /// (���g�p) UI�v�f�������Ɉ�x�����A�\�[�X�E�v���p�e�B�̒l��ǂݏo���ă^�[�Q�b�g�E�v���p�e�B�ɗ^����
    BINDINGMODE_ONE_TIME = 0,

    /// �\�[�X�E�v���p�e�B���ύX���ꂽ�ۂɁA�^�[�Q�b�g�E�v���p�e�B�ɕύX�𔽉f������i�t�͍s��Ȃ��j
    BINDINGMODE_ONE_WAY,

    /// �^�[�Q�b�g�E�v���p�e�B���ύX���ꂽ�ۂɁA�\�[�X�E�v���p�e�B�ɕύX�𔽉f������
    BINDINGMODE_ONE_WAY_TO_SOURCE,

    /// �\�[�X�E�v���p�e�B����у^�[�Q�b�g�E�v���p�e�B�̂�����̕ύX���A�����ɔ��f������ (�f�t�H���g)
    BINDINGMODE_TWO_WAY,
};

enum BindingValueType
{
    BINDINGVALUETYPE_NONE = 0,      ///< �l�̎w��Ȃ�
    BINDINGVALUETYPE_SCALAR,        ///< �X�J���[�l (float)
    BINDINGVALUETYPE_VECTOR,        ///< �x�N�g���l (Vector4)
    BINDINGVALUETYPE_STRING,        ///< ������
    BINDINGVALUETYPE_CONST,         ///< "Left" ���̒萔 (�K�v�H)
    BINDINGVALUETYPE_BINDING,       ///< "{Binding Text}" ���� Binding �w��
};

//=============================================================================
// �� TemplateBindableValue
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              �e GUIElement �� Template �N���X�������o�Ƃ��Ă��l�B
//              �R���p�C���c���Ă����قǑ傰���ł��Ȃ����ǁA
//              XML �̕������͂̕��ׂ��y�����邽�߂̏��u�B
//*/
//=============================================================================
class TemplateBindableValue
{
public:

    TemplateBindableValue();

    /// �����l�ݒ�
    ///     serialize() �͂��Ȃ��P���ݒ�B
    ///     Template �p����ŉ��߂ď����l��ݒ肷�邱�Ƃ�����̂ŁA�R���X�g���N�^�ɂ͂��Ȃ��B
    void initialize( lnfloat value );
    void initialize( const LVector4& value );
    void initialize( const lnChar* value );

    void serialize( BindingValueType request_type, const lnChar* path );

    BindingMode         getBindingMode() const { return mBindingMode; }
    BindingValueType    getValueType() const { return mValueType; }
    float               getFloat() const { return mVector.x; }
    const LVector4&     getVector() const { return mVector; }
    const LRefTString&  getString() const { return mPath; }

private:

    BindingMode         mBindingMode;
    BindingValueType    mValueType;
    LRefTString         mPath;          ///< mValueType �� BINDINGVALUETYPE_BINDING �̏ꍇ�̓v���p�e�B���BBINDINGVALUETYPE_STRING �̏ꍇ�͕�����
    LVector4            mVector;
};

//=============================================================================
// �� BindableValue
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              �o�C���f�B���O���s�������o�ϐ�(�Ƃ������AXML �̑�������ݒ�\�Ȃ��ׂẴv���p�e�B)��
//              ���̃N���X���g�p����B 
//*/
//=============================================================================
class BindableValue
{
public:

    BindableValue( BindingTargetElement* parent_element, BindingValueType type )
        : mParentElement    ( parent_element )
        , mValueType        ( type )
        , mIsBinding        ( false )
    {}

    void setFormat( const lnChar* path_ );

    bool checkBindingSource( const lnChar* property_name_ ) const
    { 
        return ( mIsBinding && _tcscmp( property_name_, mPath.c_str() ) == 0 );
    };

public:

    const lnChar* getString( INotifyPropertyChanged* context_ ) const
    {
        return (mIsBinding && (mMode == BINDINGMODE_ONE_WAY || mMode == BINDINGMODE_TWO_WAY)) ?
            context_->getPropertyValue( mPath ) :
            mPath.c_str();
    }

    void sendString( INotifyPropertyChanged* context_, const lnChar* str_ )
    {
        if ( mIsBinding && 
            (mMode == BINDINGMODE_ONE_WAY_TO_SOURCE || mMode == BINDINGMODE_TWO_WAY) &&
            mParentElement->isLockedBindingToSource() )
        {
            context_->onPropertyApplied( mPath, BindingSourceValue( str_ ) );
        };
    }

private:

    BindingTargetElement*   mParentElement;
    BindingValueType        mValueType;
    LRefTString  mPath;
    BindingMode     mMode;
    bool            mIsBinding;

};



} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================