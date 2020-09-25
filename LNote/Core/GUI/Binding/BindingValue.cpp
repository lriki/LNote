//=============================================================================
//【 BindingValue 】
//=============================================================================
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Base/StringUtil.h"
#include "BindingValue.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

//=============================================================================
// ■ TemplateBindableValue
//=============================================================================

    TemplateBindableValue::TemplateBindableValue()
        : mBindingMode  ( BINDINGMODE_TWO_WAY )
        , mValueType    ( BINDINGVALUETYPE_NONE )
    {
    }

    void TemplateBindableValue::initialize( lnfloat value )
    {
        mValueType = BINDINGVALUETYPE_SCALAR;
        mVector.x = value;
    }

    void TemplateBindableValue::initialize( const LVector4& value )
    {
        mValueType = BINDINGVALUETYPE_VECTOR;
        mVector = value;
    }

    void TemplateBindableValue::initialize( const lnChar* value )
    {
        mValueType = BINDINGVALUETYPE_STRING;
        mPath = value;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void TemplateBindableValue::serialize( BindingValueType request_type, const lnChar* path )
    {
        mValueType = BINDINGVALUETYPE_NONE;

        //-----------------------------------------------------
        // {Binding ...} 解析

        ln_std_tstring str;
        if ( Base::StringUtil::trimRange( &str, path, _T('{'), _T('}') ) )
        {
            // 前後のスペースを消す
            Base::StringUtil::trimHFSpace( &str, str );

            // 連続するスペースを 1 つにする
            Base::StringUtil::replaceSpace( &str, str.c_str(), _T(" ") ); 

            // スペースで分割
            std::vector< ln_std_tstring > units = Base::StringUtil::split( str, _T(" ") );

            if ( units.size() >= 2 && units[0] == _T("Binding") )
            {
                mPath = units[1].c_str();
                mBindingMode = BINDINGMODE_TWO_WAY;
                mValueType = BINDINGVALUETYPE_BINDING;

                if ( units.size() >= 3 )
                {
                    std::vector< ln_std_tstring > pair;
                    Base::StringUtil::split( &pair, &units[2].c_str()[8], _T("=") ); // 8 は "Binding "

                    if ( pair[1] == _T("OneTime") )
                    {
                        mBindingMode = BINDINGMODE_ONE_TIME;
                    }
                    else if ( pair[1] == _T("OneWayToSource") )
                    {
                        mBindingMode = BINDINGMODE_ONE_WAY_TO_SOURCE;
                    }
                }
            }
        } 
        
        //-----------------------------------------------------
        // Binding でなければ値化

        if ( mValueType != BINDINGVALUETYPE_BINDING )
        {
            ln_std_tstring tmp_str = path;
            Base::StringUtil::deleteSpace( &tmp_str );
            std::vector< ln_std_tstring > units = Base::StringUtil::split( tmp_str, _T(" ") );

            TCHAR* endptr = NULL;
            lnfloat vec[4] = { 0 };
            int i = 0;
            for ( ; i < units.size() && i < 4; ++i )
            {
                double d = _tcstod( units[i].c_str(), &endptr );
                if ( endptr ) break;
            }

            if ( units.size() >= 2 )
            {
                mValueType = BINDINGVALUETYPE_VECTOR;
                if ( endptr )
                {
                    mVector = 0.0f;
                }
                else
                {
                    mVector.set( vec[0], vec[1], vec[2], vec[3] );
                }
            }
            else if ( units.size() == 1 )
            {
                if ( endptr )
                {
                    mValueType = BINDINGVALUETYPE_STRING;
                    mPath = units[i].c_str();
                }
                else
                {
                    mValueType = BINDINGVALUETYPE_SCALAR;
                    mVector.x = vec[0];
                }
            }
        }

        //-----------------------------------------------------
        // 要求された型チェック

        // CONST は STRING からで OK
        if ( request_type == BINDINGVALUETYPE_CONST && mValueType == BINDINGVALUETYPE_STRING )
        {
            mValueType = request_type;
        }
        // エラー
        else if ( mValueType != request_type )
        {
            LN_THROW_InvalidCall( 0 );
        }
    }

//=============================================================================
// ■ BindingValue
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void BindableValue::setFormat( const lnChar* path_ )
    {
        mIsBinding = false;

        ln_std_tstring str;
        if ( Base::StringUtil::trimRange( &str, path_, _T('{'), _T('}') ) )
        {
            // 前後のスペースを消す
            Base::StringUtil::trimHFSpace( &str, str );

            // 連続するスペースを 1 つにする
            Base::StringUtil::replaceSpace( &str, str.c_str(), _T(" ") ); 

            // スペースで分割
            std::vector< ln_std_tstring > units = Base::StringUtil::split( str, _T(" ") );

            if ( units.size() >= 2 && units[0] == _T("Binding") )
            {
                mPath = units[1].c_str();
                mMode = BINDINGMODE_TWO_WAY;
                mIsBinding = true;

                if ( units.size() >= 3 )
                {
                    std::vector< ln_std_tstring > pair;
                    Base::StringUtil::split( &pair, &units[2].c_str()[8], _T("=") ); // 8 は "Binding "

                    if ( pair[1] == _T("OneTime") )
                    {
                        mMode = BINDINGMODE_ONE_TIME;
                    }
                    else if ( pair[1] == _T("OneWayToSource") )
                    {
                        mMode = BINDINGMODE_ONE_WAY_TO_SOURCE;
                    }
                }
            }
        } 
        
        if ( !mIsBinding )
        {
            mPath = path_;
        }
    }


} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================