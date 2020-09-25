//=============================================================================
//【 LFCommon 】
//-----------------------------------------------------------------------------
///**
//  @file       LFCommon.h
//  @brief      LFCommon
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-----------------------------------------------------------------------------

#define LN_FUNC_CALL_THROW(x) \
    try \
    { \
        x; \
    } \
    catch (LNote::Core::Base::Exception& e) \
    { \
        return LNote::Core::Function::Manager::procException( e ); \
    } \
    return LN_OK;

// Create 系など、失敗時に return 0 する関数用
#define LN_FUNC_CALL_RETURN_0(x) \
    try \
    { \
        x; \
    } \
    catch (LNote::Core::Base::Exception& e) \
    { \
        LNote::Core::Function::Manager::procException( e ); \
        return 0; \
    } \

// Create 系など、失敗時に return 0 する関数用
#define LN_FUNC_CALL_RETURN_ENUM(x, default_val) \
    try \
    { \
        x; \
    } \
    catch (LNote::Core::Base::Exception& e) \
    { \
        LNote::Core::Function::Manager::procException( e ); \
        return default_val; \
    } \

// Doxygen からのプリプロセッサ定義 (名前空間の無効化)
#ifdef LNOTE_DISABLE_AT_DOXY_BUILD
    #define LNOTE_FUNC_NAMESPACE_BEGIN
    #define LNOTE_FUNC_NAMESPACE_END

         
#if 0
    /**
        @var typedef int LNRESULT
        @brief 関数の処理結果を表す値です。詳細は @ref modules_1_error_1_section_1 をご覧ください。
    */
    typedef int                 LNRESULT; //
#endif

#else  // LNOTE_DISABLE_AT_DOXY_BUILD

    #define LNOTE_FUNC_NAMESPACE_BEGIN \
    namespace LNote \
    { \
	namespace Core \
    { \
    namespace Function \
    {

    #define LNOTE_FUNC_NAMESPACE_END \
    } \
	} \
    }

#endif // LNOTE_DISABLE_AT_DOXY_BUILD


//-----------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_BEGIN

enum ObjectType
{
    OBJECT_UNKNOWN = 0,

    OBJECT_SOUND,
};

LNOTE_FUNC_NAMESPACE_END

//=============================================================================
//								end of file
//=============================================================================