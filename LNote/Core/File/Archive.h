//=============================================================================
//�y Archive �z
//-----------------------------------------------------------------------------
///**
//  @file       Archive.h
//  @brief      Archive
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include <string>
#include "../Base/RefString.h"
#include "../Thread/Mutex.h"
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
class ArchiveInStream;

//=============================================================================
// �� Archive �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �A�[�J�C�u�t�@�C���������N���X
//*/
//=============================================================================
class Archive
    : public Base::ReferenceObject
{
public:

	/// initialize() ���Ԃ��G���[�R�[�h
	enum ErrorCode
	{
		SUCCESS = 0,	///< ����
		ERR_NOT_OPEN,	///< �A�[�J�C�u�t�@�C�����J���Ȃ�����
		ERR_FORMAT,		///< �A�[�J�C�u�t�@�C���ł͂Ȃ�
		ERR_KEY			///< �L�[���s��v
	};

public:

	/// �R���X�g���N�^
	Archive();

	/// �f�X�g���N�^
	virtual ~Archive();

public:

    //---------------------------------------------------------------------
    /**
    //  @brief      �A�[�J�C�u�t�@�C�����J���ăA�N�Z�X�̏���������
    //
    //  @param[in]  filename_ : �A�[�J�C�u�t�@�C���̃t�@�C����
    //  @param[in]  key_      : �����Ɏg���L�[ ( \0 �I�[������ �܂��� NULL )
    //
    //  @par
    //              filename_ �� NULL ��n���ƁA�_�~�[�A�[�J�C�u�Ƃ��ď��������܂��B
    //              �_�~�[�A�[�J�C�u�́A���ڃf�B���N�g����̃t�@�C���������܂��B
    //
    //              filename_ �Ƀf�B���N�g���p�X���܂܂�Ă���ꍇ�A
    //              �A�[�J�C�u�Ɋ܂܂�Ă���t�@�C���͂��ׂĂ��̃f�B���N�g���p�X
    //              �ȉ��̃t�@�C���Ƃ��Ĉ����܂��B
@code
// �A�[�J�C�u�t�@�C�����ɁuChara.png�v�uMap/Ground.png�v�Ƃ����t�@�C��������ꍇ�c

// �� 1 )
// ���s�t�@�C���Ɠ����t�H���_�ɂ���A�[�J�C�u�t�@�C�� "Image.lna" ���g��
archive.initialize( "Image.lna" );

file.open( "Image/Chara.png" );
file.open( "Image/Map/Ground.png" );


// �� 2 )
// ���s�t�@�C���Ɠ����t�H���_�ɂ���uData�v�t�H���_���̃A�[�J�C�u�t�@�C�� "Image.lna" ���g��
archive.initialize( "Data/Image.lna" );

file.open( "Data/Image/Chara.png" );
file.open( "Data/Image/Map/Ground.png" );
@endcode
    //*/
    //---------------------------------------------------------------------
    ErrorCode initialize( const lnChar* directory_, const lnChar* filename_, const char* key_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �t�@�C���X�g���[���ƃf�[�^�ʒu�܂ł̃I�t�Z�b�g�擾 �y ���S���̂��߁A�g�p�񐄏��BcreateStream() ���g���Ă������� �z
    //
    //  @param[in]  filename_    : �t�@�C���� (/ �� \\ �ɕϊ��ς݂ł��邱��)
    //  @param[out] out_stream_  : �t�@�C���X�g���[���ւ̃|�C���^���i�[����ϐ��ւ̃|�C���^
    //  @param[out] data_offset_ : �f�[�^������ꏊ�܂ł̃I�t�Z�b�g ( �o�C�g�P�� ) ���i�[����ϐ��ւ̃|�C���^
    //  @param[out] data_size_   : �t�@�C���T�C�Y���i�[����ϐ��ւ̃|�C���^
    //
    //  @par
    //              �t�@�C����������Ȃ������ꍇ�� out_stream_ �� NULL ���������܂��B   <br>
    //                                                                                      <br>
    //              �_�~�[�A�[�J�C�u�̏ꍇ�Aout_stream_ �͏�� NULL �ł��B          
    //
    //  @attention 
    //              ���̊֐��Ŏ擾�����t�@�C���X�g���[���͕K�����̃N���X�� close() �֐���
    //              ���Ă��������Bfclose() �ŕ���ƃA�N�Z�X�ᔽ���������܂��B
    //*/
    //---------------------------------------------------------------------
    void open( const lnChar* filename_, FILE** out_stream_, int* data_offset_, int* data_size_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �A�[�J�C�u���̃t�@�C���ɃA�N�Z�X���邽�߂̃X�g���[���̍쐬
    //
    //  @param[out] stream_   : �쐬���ꂽ�I�u�W�F�N�g���w���|�C���^�ϐ��ւ̃A�h���X
    //  @param[in]  filename_ : �t�@�C���� (/ �� \\ �ɕϊ��ς݂ł��邱��)
    //
    //  @par
    //              �t�@�C����������Ȃ������ꍇ�� stream_ �� NULL ���������܂��B   <br>
    //              <br>
    //              �_�~�[�A�[�J�C�u�ł��A�t�@�C����������� ArchiveInStream �͍쐬����܂��B<br>
    //              ArchiveInStream �͒ʏ�̃A�[�J�C�u�A�_�~�[�A�[�J�C�u�̋�ʂȂ�
    //              �����悤�Ɏg�����Ƃ��ł��܂��B
    //
    //  @note
    //              ���̊֐��̌Ăяo������ ( File::Manager ) �ł́A������
    //              �A�[�J�C�u���`�F�b�N���āA�S���Ȃ�������G���[�ɂ���悤�ɂ������B
    //              ���̂��߁A���̊֐��̒��ŃG���[�ɂ͂������Ȃ��B
    //              ����Ȃ킯�ŁA���̊֐��̖߂�l�� void�B
    //*/
    //---------------------------------------------------------------------
    void createStream( IInStream** stream_, const lnChar* filename_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �A�[�J�C�u�̒��Ƀt�@�C�������邩�ǂ����m�F����
    //
    //  @par
    //              �_�~�[�A�[�J�C�u�̏ꍇ�̓f�B���N�g���Ƀt�@�C�������邩�ǂ������m�F���܂��B<br>
    //                                                                                        <br>
    //              �t�@�C���̑��݂��m�F���āA�����ɂ��̃t�@�C�����J���ꍇ��
    //              open() �ŕԂ����t�@�C���X�g���[���� NULL ���ǂ�����
    //              �`�F�b�N������������ɓ��삵�܂��B
    //*/
    //---------------------------------------------------------------------
    bool isExist( const char* filename_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief		�_�~�[�A�[�J�C�u���𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool isDummy() const { return mIsDummy; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�[�^��ǂݍ���
    //
    //  @param[out] buffer_ : �f�[�^���������ރo�b�t�@�ւ̃|�C���^
    //  @param[in]  count_	: �������ރf�[�^�T�C�Y ( �o�C�g�� )
    //  @param[in]  stream_ : �t�@�C���X�g���[��
    //
    //  @return     �ǂݍ��񂾃f�[�^�T�C�Y
    //
    //  @par
    //              �f�[�^�̈Í����������Ȃ���ǂݍ��݂��s���܂��B
    //              fread() �̑���Ɏg���܂��B
    //              fseek() �Ő������ʒu���w���Ă���X�g���[����n���Ă��������B<br>
    //              �_�~�[�A�[�J�C�u�̏ꍇ�̓���� fread() �Ɠ����ł�
    //*/
    //---------------------------------------------------------------------
    u32 read( void* buffer_, u32 count_, FILE* stream_, LNFileOpenMode mode_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      open() �Ŏ擾�����t�@�C���X�g���[�������
    //*/
    //---------------------------------------------------------------------
    void close( FILE* stream_ );

private:

    // ���l�� 16 �ɂ��낦�邽�߂ɉ��Z���鐔�l�u( 16 - ( v_ % 16 ) ) % 16�v�̍œK�� ( 5 �� 11�A27 �� 5 �� )
    int _padding16( int v_ ) const { return ( v_ != 0 ) ? ( 16 - ( v_ & 0x0000000f ) ) & 0x0000000f : 16; }
    u32 _padding16( u32 v_ ) const { return ( v_ != 0 ) ? ( 16 - ( v_ & 0x0000000f ) ) & 0x0000000f : 16; }

    /// �ꎞ�o�b�t�@�̃T�C�Y�`�F�b�N
    void _checkTempBuffer( u32 request_size_ );

    /// �p�f�B���O���l�����Đ�����ǂݍ���
	u32 _readU32Padding16();

    /// �p�f�B���O���l�����Đ�����ǂݍ��� (�t�@�C���������A�t�@�C���T�C�Y�p)
	void _readU32Padding16( u32* v0_, u32* v1_ );

    /// �p�f�B���O���l�����ăf�[�^��ǂݍ���
	int _readPadding16( void* buffer_, int count_ );



	/// 4 �o�C�g�ǂݍ���
	//u32 _readU32();

	/// �f�R�[�h�t�� read
	//u32 _readWithDecord( const void* buffer_, u32 count_ );

    /// �f�t�H���g�̃f�R�[�_
	//static void _decoder( DecodeArgs& args_ );

private:

	/// �t�@�C���ЂƂ��̏��
	struct Entry
	{
		u32		mOffset;		///< �X�g���[���擪����t�@�C���̈ʒu�܂ł̃I�t�Z�b�g
		u32		mSize;			///< �t�@�C���T�C�Y
	};

	typedef std::map< Base::RefTString, Entry > EntriesMap;

private:

    static const int KEY_SIZE  = 128;

    Base::RefTString    mArchiveDirectory;  ///< �A�[�J�C�u�t�@�C�����f�B���N�g���Ɍ����Ă����́A�����܂̃p�X ( [�p�X]/[�g���q���������A�[�J�C�u�t�@�C����] )
	EntriesMap	        mEntriesMap;	    ///< �t�@�C�����ɑΉ�����t�@�C�������i�[���� map
	FILE*		        mStream;		    ///< �A�[�J�C�u�t�@�C���̃X�g���[��
	int			        mFileNum;		    ///< �A�[�J�C�u�t�@�C�����̃t�@�C����
    Base::RefTString    mKey;			    ///< �����L�[ (char)
	bool		        mIsDummy;		    ///< �_�~�[�A�[�J�C�u�̏ꍇ true ( �f�B���N�g�����璼�ړǂ� )
    lnByte              mKeyTable[ 256 ];
    lnByte*             mTempBuffer;
    u32                 mTempBufferSize;
    Thread::Mutex*      mLock;
};

//=============================================================================
// �� ArchiveInStream �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �A�[�J�C�u����t�@�C����ǂނ��߂̃X�g���[���N���X
//
//  @note
//              �R���X�g���N�^�� private �ł��B
//              Archive �N���X����쐬���Ă��������B
//*/
//=============================================================================
class ArchiveInStream
    : public IInStream
    , public Base::ReferenceObject
{
private:

    /// �R���X�g���N�^
	ArchiveInStream( Archive* archive_, FILE* stream_, u32 data_offset_, u32 data_size_, LNFileOpenMode mode_, bool is_dir_ );

	/// �f�X�g���N�^
	virtual ~ArchiveInStream();

    LN_REFOBJ_METHODS;

public:

	/// �t�@�C�� ( �f�[�^ ) �T�C�Y�̎擾
    virtual int getSize() { return mDataSize; }

	/// �t�@�C���|�C���^�̈ʒu�̎擾
	virtual int getPosition() { return mSeekPoint; }

	/// �f�[�^�̓ǂݍ���
	virtual int read( void* buffer_, int buffer_size_, int read_size_ = -1 );

	/// �t�@�C���|�C���^�̐ݒ�
	virtual void seek( int offset_, int origin_ = SEEK_SET );

private:

    Archive*        mArchive;           ///< ���̃N���X���쐬�����A�[�J�C�u�N���X
    FILE*           mStream;            ///< �t�@�C���X�g���[��
    u32             mDataOffset;        ///< �t�@�C���̐擪����f�[�^�̐擪�ʒu�܂ł̃I�t�Z�b�g
    u32             mDataSize;          ///< �f�[�^�T�C�Y
    u32             mSeekPoint;         ///< �V�[�N�ʒu
    LNFileOpenMode  mOpenMode;
    bool            mIsReadDirectory;   ///< �f�B���N�g������Ă�ł���ꍇ true

    friend class Archive;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace File
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================