//=============================================================================
//【 BindingValue 】
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
    /// (未使用) UI要素生成時に一度だけ、ソース・プロパティの値を読み出してターゲット・プロパティに与える
    BINDINGMODE_ONE_TIME = 0,

    /// ソース・プロパティが変更された際に、ターゲット・プロパティに変更を反映させる（逆は行わない）
    BINDINGMODE_ONE_WAY,

    /// ターゲット・プロパティが変更された際に、ソース・プロパティに変更を反映させる
    BINDINGMODE_ONE_WAY_TO_SOURCE,

    /// ソース・プロパティおよびターゲット・プロパティのいずれの変更も、他方に反映させる (デフォルト)
    BINDINGMODE_TWO_WAY,
};

enum BindingValueType
{
    BINDINGVALUETYPE_NONE = 0,      ///< 値の指定なし
    BINDINGVALUETYPE_SCALAR,        ///< スカラー値 (float)
    BINDINGVALUETYPE_VECTOR,        ///< ベクトル値 (Vector4)
    BINDINGVALUETYPE_STRING,        ///< 文字列
    BINDINGVALUETYPE_CONST,         ///< "Left" 等の定数 (必要？)
    BINDINGVALUETYPE_BINDING,       ///< "{Binding Text}" 等の Binding 指定
};

//=============================================================================
// ■ TemplateBindableValue
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              各 GUIElement の Template クラスがメンバとしてもつ値。
//              コンパイル…っていうほど大げさでもないけど、
//              XML の文字列解析の負荷を軽減するための処置。
//*/
//=============================================================================
class TemplateBindableValue
{
public:

    TemplateBindableValue();

    /// 初期値設定
    ///     serialize() はしない単純設定。
    ///     Template 継承先で改めて初期値を設定することもあるので、コンストラクタにはしない。
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
    LRefTString         mPath;          ///< mValueType が BINDINGVALUETYPE_BINDING の場合はプロパティ名。BINDINGVALUETYPE_STRING の場合は文字列
    LVector4            mVector;
};

//=============================================================================
// ■ BindableValue
//-----------------------------------------------------------------------------
///**
//  @brief      
//
//  @note
//              バインディングを行うメンバ変数(というより、XML の属性から設定可能なすべてのプロパティ)は
//              このクラスを使用する。 
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