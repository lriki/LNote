//=============================================================================
//�y LFCommon �z
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

// Create �n�ȂǁA���s���� return 0 ����֐��p
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

// Create �n�ȂǁA���s���� return 0 ����֐��p
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

// Doxygen ����̃v���v���Z�b�T��` (���O��Ԃ̖�����)
#ifdef LNOTE_DISABLE_AT_DOXY_BUILD
    #define LNOTE_FUNC_NAMESPACE_BEGIN
    #define LNOTE_FUNC_NAMESPACE_END

         
#if 0
    /**
        @var typedef int LNRESULT
        @brief �֐��̏������ʂ�\���l�ł��B�ڍׂ� @ref modules_1_error_1_section_1 ���������������B
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