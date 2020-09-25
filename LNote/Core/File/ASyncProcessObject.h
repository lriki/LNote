//=============================================================================
//�y ASyncProcessObject �z
//-----------------------------------------------------------------------------
///**
//  @file       ASyncProcessObject.h
//  @brief      ASyncProcessObject
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Thread/EventFlag.h"
#include "Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace File
{

//=============================================================================
// �� ASyncProcessObject �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �񓯊��������s���֐��I�u�W�F�N�g�N���X�̊��
//*/
//=============================================================================
class ASyncProcessObject
{
protected:

	/// �R���X�g���N�^
	ASyncProcessObject( IManager* manager_ );

	/// �f�X�g���N�^
	virtual ~ASyncProcessObject();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      ���s����֐�
    //
    //  @par
    //              ���������s�����ꍇ�� false ��Ԃ��悤�ɂ��܂��B
    //*/		
    //---------------------------------------------------------------------
    virtual bool operator()() = 0;

    //---------------------------------------------------------------------
	///**
    //  @brief      ���s�J�n
    //*/		
    //---------------------------------------------------------------------
	void start();

    //---------------------------------------------------------------------
	///**
    //  @brief      �J�n�������N�G�X�g���s���̏ꍇ�A�I������܂ő҂�
    //*/		
    //---------------------------------------------------------------------
	void wait() { _m.Finished->wait(); }

    //---------------------------------------------------------------------
	///**
    //  @brief      �Ǘ��N���X�ւ̃|�C���^�̎擾
    //*/		
    //---------------------------------------------------------------------
    IManager* getManager() const { return _m.Manager; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��Ԃ𒲂ׂ�
    //
    //  @retval     true  : �ǂݍ��݂��������Ă���
    //  @retval     false : �ǂݍ��ݒ�
    //*/
    //---------------------------------------------------------------------
    LNASyncProcessState getProcessState() const { return _m.State; }

    //---------------------------------------------------------------------
	///**
    //  @brief      �G���[�������������𔻒肷��
    //
    //  @retval     true  : �G���[����������
    //  @retval     false : ����
    //*/		
    //---------------------------------------------------------------------
    bool isError() const { return _m.Failed->isTrue(); }

private:

    struct
    {
	    IManager*	                    Manager;    ///< �Ǘ��N���X
        volatile LNASyncProcessState    State;      ///< ���
        Thread::EventFlag*	            Finished;   ///< �񓯊��ǂݍ��݂������������ǂ����̃t���O (�J�n�� true)
        Thread::EventFlag*	            Failed;     ///< �G���[�����������ꍇ treu
        bool                            Requested;  ///< start() ���Ă񂾏ꍇ true
    } _m;

	friend class Manager;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
// �R���X�g���N�^
inline ASyncProcessObject::ASyncProcessObject( IManager* manager_ )
{
    _m.Manager   = manager_;
    _m.State     = LN_ASYNCSTATE_WAIT;
    _m.Finished  = LN_NEW Thread::EventFlag( true );
    _m.Failed    = LN_NEW Thread::EventFlag( false );
    _m.Requested = false;

    LN_SAFE_ADDREF( _m.Manager );
}

// �f�X�g���N�^
inline ASyncProcessObject::~ASyncProcessObject()
{
    if ( _m.Manager )
    {
        // �ǂݍ��݃��N�G�X�g����O��
        _m.Manager->removeASyncProcess( this );
    }

    if ( _m.Finished )
    {
	    // �������Ă��Ȃ��ꍇ�͏I����҂�
	    _m.Finished->wait();
    }

	LN_SAFE_RELEASE( _m.Finished );
    LN_SAFE_RELEASE( _m.Failed );
    LN_SAFE_RELEASE( _m.Manager );
}

// �񓯊����������N�G�X�g����
inline void ASyncProcessObject::start()
{
    if ( _m.State == LN_ASYNCSTATE_WAIT )
    {
        _m.Manager->requestASyncProcess( this );
    }
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================