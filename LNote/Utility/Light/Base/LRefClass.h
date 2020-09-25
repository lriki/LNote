//=============================================================================
//�y LRefClass �z
//-----------------------------------------------------------------------------
///**
//  @file       LRefClass.h
//  @brief      LRefClass
//  @version    1.0
//  @date       2010/10/06
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LRefClass �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
template < class TYPE_, class IMPL_TYPE_ > class LRefClass
{
public:

    LRefClass()
        : _impl( NULL )
    {}

	LRefClass( lnNullPtr nil_ )
        : _impl( NULL )
    {}

	LRefClass( const LRefClass& obj_ )
        : _impl( NULL )
    {
        _impl = obj_._impl;

	    if ( _impl )
	    {
		    _impl->addRef();
	    }
    }

	virtual ~LRefClass()
    {
        release();
    }

	LRefClass& operator = ( const LRefClass& obj_ )
    {
	    // �R�s�[���Ɠ����ꍇ�͉������Ȃ�
	    if ( _impl == obj_._impl ) { return *this; }

	    // �܂��̓J�E���^���Z
	    release();

	    _impl = obj_._impl;

	    // �����Ɩ{�̂�����ꍇ�̓J�E���^�𑝂₷
	    if ( _impl )
	    {
		    _impl->addRef();
	    }

	    return *this;
    }

    bool operator == ( const LRefClass& obj_ ) const
    {
	    return ( _impl == obj_._impl );
    }

	bool operator != ( const LRefClass& obj_ ) const
    {
	    return ( _impl != obj_._impl );
    }

	TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; };
	bool operator == ( const lnNullPtr null_ ) const { return ( _impl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( _impl != NULL ); }

	void release()
    {
        LN_SAFE_RELEASE( _impl );
    }


    TYPE_* operator -> () { return (TYPE_*)this; }

    /// ���b�v���Ă���C���^�[�t�F�C�X�ւ̃|�C���^���擾����
    IMPL_TYPE_* getInterface() { return _impl; }

    //// ���󂯎�����I�u�W�F�N�g�̎Q�ƃJ�E���g���ЂƂ��₷
    //IMPL_TYPE_* _setImpl( IMPL_TYPE_* impl_ )
    //{
    //    _impl = impl_;
    //    LN_SAFE_ADDREF( _impl );
    //}


    // ���󂯎�����I�u�W�F�N�g�̎Q�ƃJ�E���g���ЂƂ��₷
    LRefClass( IMPL_TYPE_* impl_ )
        : _impl( impl_ )
    {
        LN_SAFE_ADDREF( _impl );
    }

protected:

    IMPL_TYPE_* _impl;
};

#define LN_UTIL_REFCLASS_METHODS( type_, impl_type_ ) \
    public: \
        type_(){} \
        type_( impl_type_* impl_ ) : LRefClass< type_, impl_type_ >( impl_ ) {} \
        type_& operator = ( const lnNullPtr null_ ) { release(); return *this; };



//=============================================================================
// �� LSingletonRefClass �N���X
//-----------------------------------------------------------------------------
///**
//  @brief     
//*/
//=============================================================================
template < class TYPE_, class IMPL_TYPE_ > class LSingletonRefClass
{
public:

    /// �R���X�g���N�^
    LSingletonRefClass()
        : _impl( NULL )
    {}

    /// ���b�v���Ă���C���^�[�t�F�C�X�ւ̃|�C���^���擾����
    IMPL_TYPE_* getInterface() { return _impl; }

    /// -> �A�N�Z�X
    TYPE_* operator -> () { return (TYPE_*)this; }

	bool operator == ( const lnNullPtr null_ ) const { return ( _impl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( _impl != NULL ); }

protected:

    IMPL_TYPE_* _impl;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================