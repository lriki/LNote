//=============================================================================
//�y AudioSource �z
//-----------------------------------------------------------------------------
///**
//  @file       AudioSource.h
//  @brief      AudioSource
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list> // Midi �Ŏg���Ă�̂Łc
#include "../../../Dependencies/libogg/include/ogg/ogg.h"
#include "../../../Dependencies/libvorbis/include/vorbis/vorbisfile.h"
#include "../../File/Interface.h"
#include "../Interface.h"
#include "AudioResourceManager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Audio
{

//=============================================================================
// �� AudioSourceBase �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �����f�[�^�������N���X�̊��
//*/
//=============================================================================
class AudioSourceBase
    : public IAudioSource
    , public Base::ReferenceObject
    , public AudioSourceCache::CachedObject
{
public:
    
    LN_CACHED_REFOBJ_METHODS;

    /// �R���X�g���N�^
    AudioSourceBase( ResourceManager* manager_ );

    /// �f�X�g���N�^
    virtual ~AudioSourceBase();

    /// fillBufferAndReleaseStream() �X���b�h�Z�[�t
    virtual void fillBufferSafe()
    {
        Thread::ScopedLock lock( mMutex );
        this->fillBufferAndReleaseStream();
    }

    /// seek + reed + �X���b�h�Z�[�t
    virtual void readSafe( void* buffer_, u32 buffer_size_, u32 offset_, u32* read_size_, u32* write_size_ )
    {
        Thread::ScopedLock lock( mMutex );
        this->seek( offset_ );
        this->read( buffer_, buffer_size_, read_size_, write_size_ );
    }


protected:

    ResourceManager*    mManager;
    Thread::Mutex       mMutex;
};

//=============================================================================
// �� Wave �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      Wave �f�[�^�������N���X
//*/
//=============================================================================
class Wave
    : public AudioSourceBase
{
public:

	/// �R���X�g���N�^
	Wave( ResourceManager* manager_ );

	/// �f�X�g���N�^
	virtual ~Wave();

public:

	/// �t�@�C���t�H�[�}�b�g�̎擾
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_WAVE; }

	/// ���f�[�^�̃T�C�Y�̎擾 ( �X�g���[�~���O�Đ��ł̏I�����蓙�Ŏg�� )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// PCM �t�H�[�}�b�g�̎擾
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// �S�̂̍Đ����Ԃ̎擾 ( �~���b )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// �S�̂̃T���v�����̎擾
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// �I���������Đ��p�̃o�b�t�@�̐擪�A�h���X�擾
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// �I���������Đ����̑S�o�b�t�@�T�C�Y�̎擾
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 �b������̃o�C�g���擾
	virtual u32 getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }

    /// ���[�v�J�n�ʒu�ƏI���ʒu�̎擾
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = 0; *length_ = 0; }

	/// �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
	virtual LNRESULT fillBufferAndReleaseStream();

	/// �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// �t�@�C���|�C���^�ړ�
	virtual LNRESULT seek( u32 offset_ );

	/// �f�R�[�h��Ԃ̃��Z�b�g
	virtual void reset() {}

private:

	/// wave �t�@�C���̃w�b�_
	struct WaveFileHeader
	{
		u32	RIFF;
		u32	Size;
		u32	WaveHeader;
		u32	FmtChunk;
		u32	FmtSize;
	};

private:

    File::IInStream*        mInFile;				///< ���̓X�g���[��
	LNWaveFormat	        mWaveFormat;			///< PCM �p�t�H�[�}�b�g
	u32				        mSourceDataSize;		///< �����f�[�^�����̃T�C�Y
	u32				        mDataOffset;			///< �X�g���[������ PCM �f�[�^�̐擪�܂ł̃I�t�Z�b�g�o�C�g��
	u32				        mPCMDataSize;			///< �X�g���[������ PCM �f�[�^�̃T�C�Y
	u32				        mTotalTime;				///< �S�̂̍Đ����� ( �~���b )
	u8*				        mOnmemoryPCMBuffer;		///< �I���������Đ��p�̃f�[�^��ǂݍ��ރo�b�t�@
	u32				        mOnmemoryPCMBufferSize;	///< mOnmemoryPCMBuffer �̃T�C�Y ( ���̂Ƃ��� mDataOffset �Ɠ��� )
    u32                     mTotalSamples;          ///< �S�̂̍Đ��T���v����
};

/*
	�� WaveFileHeader �\���̂ɂ��Ă߂�

	��������

		struct RiffHeader
		{
			u32 riff;	// 'RIFF'
			s32 size;	// �t�@�C���̑S�̃T�C�Y - 8 �̒l
			u32 type;	// �����f�[�^�̏ꍇ�� 'WAVE'
		};

	�Ƃ����w�b�_�̌�ɁA

		struct FormatChunk
		{
			u32 id;			// 'fmt '
			s32 size;		// id �� size ������fmt�`�����N�T�C�Y
			s16 format_tag;	// �g�`�f�[�^�̃t�H�[�}�b�g ( ��{�� WAVE_FORMAT_PCM�B����ȊO�̏ꍇ�� extended ������炵�� )
			u16 channels;
			u32 samplerate;
			u32 bytepersec;
			u16 blockalign;
			u16 bitswidth;
		};
		u16 extended_size;
		u8  extended [];

	�c�Ƒ����Ă����B
	���̂��ƂɎ��ۂ̃f�[�^�`�����N������B

		struct DataChunk
		{
			u32 id;		// 'data'
			s32 size;
			u8  waveformData [];
		};

	
	���̃��C�u�����Ŏg���Ă遪�� WaveFileHeader �\���̂́A

	RiffHeader �S����FormatChunk �� size �܂ł�
	�S������������(�t�@�C���S�̂̃T�C�Y�͂���Ȃ���)�t�@�C���t�H�[�}�b�g�̃`�F�b�N�ɂ����g��Ȃ���ˁI
	���Ă��ƂłЂƂɂ܂Ƃ߂Ă܂��B

*/

#if defined(LNOTE_MSVC)
//=============================================================================
// �� MP3 �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      MP3 �f�[�^�������N���X
//
//  @note
//              �X�g���[�~���O�Đ����̃R���o�[�g���͂��̃N���X�̓�����
//              �����Ă��邽�߁A�����̃v���C���[�N���X�ł��̃\�[�X�����L����
//              �X�g���[�~���O�Đ����邱�Ƃ͂ł��Ȃ��B( ���ۂ͂ł��邯�ǁA
//              �����ςȊ����ɔ�� )
//              ���̂��߁A���̃N���X�͋��L���Ȃ��悤�ɂ��邱�ƁB
//              ( �X�g���[�~���O�Đ����K�v�Ȃقǒ��������𕡐��Đ����邱�Ƃ�
//              ���܂�Ȃ����낤���c�B )
//              <br>
//              <br>
//              ���Ȃ݂ɂ������������ɂ́A�I�[�f�B�I�\�[�X����X�g���[�~���O�Đ��̏��
//              ���擾���Aread() ���ĂԂƂ��ɂ����n���ĉ����f�[�^���擾����悤�ɂ���B
//              ( ����͎��o�[�W�����ł̎����ɂȂ邩�Ȃ��c )
//*/
//=============================================================================
class MP3
    : public AudioSourceBase
{
public:

	/// �R���X�g���N�^
	MP3( ResourceManager* manager_ );

	/// �f�X�g���N�^
	virtual ~MP3();

public:

	/// �t�@�C���t�H�[�}�b�g�̎擾
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_MP3; }

	/// PCM �t�H�[�}�b�g�̎擾
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// ���f�[�^�̃T�C�Y�̎擾 ( �X�g���[�~���O�Đ��ł̏I�����蓙�Ŏg�� )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// �S�̂̍Đ����Ԃ̎擾 ( �~���b )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// �S�̂̃T���v�����̎擾
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// �I���������Đ��p�̃o�b�t�@�̐擪�A�h���X�擾
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// �I���������Đ����̑S�o�b�t�@�T�C�Y�̎擾
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 �b������̃o�C�g���擾
	virtual u32 getBytesPerSec() const { return mStreamingPCMBufferSize; }

    /// ���[�v�J�n�ʒu�ƏI���ʒu�̎擾
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = 0; *length_ = 0; }

	/// �I�[�f�B�I�t�@�C���Ƃ��Ĉ����t�@�C����ݒ肷��
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
	virtual LNRESULT fillBufferAndReleaseStream();

	/// �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// �t�@�C���|�C���^�ړ�
	virtual LNRESULT seek( u32 offset_ );

	/// �f�R�[�h��Ԃ̃��Z�b�g
	virtual void reset() { mResetFlag = true; }

private:

	/// �ǂ̃t�H�[�}�b�g�� mp3 �����ׂāA�f�[�^�܂ł̃I�t�Z�b�g��^�O�t�B�[���h�̃T�C�Y�������o�Ɋi�[
	LNRESULT _checkId3v();

	/// mp3 �� PCM �t�H�[�}�b�g�𒲂ׂă����o�Ɋi�[����
	LNRESULT _getPCMFormat();

private:

	/// ID3v2 �`���̃w�b�_���
	struct ID3v2Header
	{
		u8	ID[ 3 ];
		u8	Version[ 2 ];
		u8	Flag;
		u8	Size[ 4 ];
	};

private:

    File::IInStream*        mInFile;					///< ���̓X�g���[��
	LNWaveFormat			mWaveFormat;				///< PCM �p�t�H�[�}�b�g
    //WAVEFORMATEX            mWaveFormatEX;
	MPEGLAYER3WAVEFORMAT*	mMP3WaveFormat;				///< MPEGLAYER3WAVEFORMAT �\���̂ւ̃|�C���^
	HACMSTREAM*				mACMStreamHandle;			///< HACMSTREAM �\���̂ւ̃|�C���^
	u32						mSourceDataSize;			///< �\�[�X�f�[�^�̃T�C�Y ( ���܂̂Ƃ��� mInFile.size() �Ɠ��� )
	u32						mDataOffset;				///< �X�g���[������ PCM �f�[�^�̐擪�܂ł̃I�t�Z�b�g�o�C�g��
	u32						mID3vTagFieldSize;			///< Id3v2 �`���̏ꍇ�̃^�O�t�B�[���h(�w�b�_���)�����̃T�C�Y
	u8*						mOnmemoryPCMBuffer;			///< �I���������Đ��p�̃f�[�^��ǂݍ��ރo�b�t�@
	u32						mOnmemoryPCMBufferSize;		///< ( fillBufferAndReleaseStream() �Œl���ݒ肳��� )
	u32						mStreamingPCMBufferSize;	///< 1 �b���� mp3 �f�[�^��ϊ��������́A�œK�ȓ]���� PCM �o�b�t�@�T�C�Y
	u8*						mMP3SourceBufferParSec;		///< �f�R�[�h���Ƀt�@�C������ǂ� 1 �b���� mp3 �f�[�^���ꎞ�I�Ɋi�[����o�b�t�@
	u32						mMP3SourceBufferSizeParSec;	///< mMP3SourceBufferParSec �̃T�C�Y
	u32						mTotalTime;					///< �S�̂̍Đ����� ( �~���b )
    u32                     mTotalSamples;              ///< �S�̂̃T���v����
	bool					mResetFlag;					///< �f�R�[�h��Ԃ̃��Z�b�g��v������t���O ( read() �ł̃f�R�[�h���̃t���O�w��Ɏg�� )
};

/*
	id3v1 �c MP3�f�[�^�̓t�@�C���̐擪����i�[����Ă���
	id3v2 �c �^�O�t�B�[���h�̌��MP3�f�[�^���i�[����Ă���
*/
#endif // LNOTE_MSVC

//=============================================================================
// �� Ogg �N���X
//-----------------------------------------------------------------------------
/**
//  @brief      Ogg Vorbis �����f�[�^�������N���X
//
//  @note
//              �C�ɂȂ�_�c<br>
//              1�T���v��������̃r�b�g����16�r�b�g�ŌŒ肵�Ă���B���̂Ƃ�����Ȃ����ǁc�H
//*/
//=============================================================================
class Ogg
    : public AudioSourceBase
{
public:

	/// �R���X�g���N�^
	Ogg( ResourceManager* manager_ );

	/// �f�X�g���N�^
	virtual ~Ogg();

public:

	/// �t�@�C���t�H�[�}�b�g�̎擾
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_OGG; }

	/// PCM �t�H�[�}�b�g�̎擾
	virtual const LNWaveFormat* getWaveFormat() const { return &mWaveFormat; }

	/// ���f�[�^�̃T�C�Y�̎擾 ( �X�g���[�~���O�Đ��ł̏I�����蓙�Ŏg�� )
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

	/// �S�̂̍Đ����Ԃ̎擾 ( �~���b )
	virtual u32 getTotalTime() const { return mTotalTime; }

    /// �S�̂̃T���v�����̎擾
    virtual u32 getTotalUnits() const { return mTotalSamples; }

	/// �I���������Đ��p�̃o�b�t�@�̐擪�A�h���X�擾
	virtual u8* getOnmemoryPCMBuffer() const { return mOnmemoryPCMBuffer; }

	/// �I���������Đ����̑S�o�b�t�@�T�C�Y�̎擾
	virtual u32 getOnmemoryPCMBufferSize() const { return mOnmemoryPCMBufferSize; }

	/// 1 �b������̃o�C�g���擾
	virtual u32 getBytesPerSec() const { return mWaveFormat.AvgBytesPerSec; }

    /// ���[�v�J�n�ʒu�ƏI���ʒu�̎擾
    virtual void getLoopState( u32* begin_, u32* length_ ) const { *begin_ = mLoopStart; *length_ = mLoopLength; }

	/// �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	virtual LNRESULT setStream( File::IInStream* stream_ );

	/// �I���������Đ��p�ɑS�Ẵf�[�^��ǂݍ���
	virtual LNRESULT fillBufferAndReleaseStream();

	/// �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
	virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ );

	/// �t�@�C���|�C���^�ړ�
	virtual LNRESULT seek( u32 offset_ );

	/// �f�R�[�h��Ԃ̃��Z�b�g
	virtual void reset() {}

public:	// �ȉ��� ogg API ����̃R�[���o�b�N�Ƃ��ēo�^����֐��B���̂��� public

	/// ogg �p read �R�[���o�b�N
	static size_t readOggCallback( void* buffer_, size_t element_size_, size_t count_, void* stream_ );

	/// ogg �p seek �R�[���o�b�N
	static int seekOggCallback( void* stream_, ogg_int64_t offset_, int whence_ );

	/// ogg �p close �R�[���o�b�N
	static int closeOggCallback( void* stream_ );

	/// ogg �p tell �R�[���o�b�N
	static long tellOggCallback( void* stream_ );

private:

	static const int WORD_SIZE = sizeof( lnU16 );
	static const int WORD_BITS = WORD_SIZE * 8;

    File::IInStream*        mInFile;				///< ���̓X�g���[��
	OggVorbis_File	        mOggVorbisFile;			///< �I�[�v���ς݂� Ogg �t�@�C��
	LNWaveFormat	        mWaveFormat;			///< PCM �p�t�H�[�}�b�g
	u32				        mSourceDataSize;		///< ���f�[�^�̃T�C�Y ( �G���R�[�h���ꂽ���� )
	u8*				        mOnmemoryPCMBuffer;	    ///< �I���������Đ��p�̃f�[�^��ǂݍ��ރo�b�t�@
	u32				        mOnmemoryPCMBufferSize;	///< ( fillBufferAndReleaseStream() �Œl���ݒ肳��� )
	u32				        mTotalTime;				///< �S�̂̍Đ����� ( �~���b )
    u32                     mTotalSamples;          ///< �S�̂̃T���v����
    u32                     mLoopStart;             ///< ���[�v�̈�̐擪�T���v����
    u32                     mLoopLength;            ///< ���[�v�̈�̒��� ( �T���v���� )
};

    
#if defined(LNOTE_MSVC)
//=============================================================================
// �� Midi �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      Midi �f�[�^�������N���X
//*/
//=============================================================================
class Midi
    : public AudioSourceBase
{
public:

	/// �R���X�g���N�^
	Midi( ResourceManager* manager_ );

	/// �f�X�g���N�^
	virtual ~Midi();

public:

	/// �t�@�C���t�H�[�}�b�g�̎擾
	virtual LNAudioSourceFormat getSourceFormat() const { return LN_AUDIOSOURCE_MIDI; }

    /// ���f�[�^�̎擾 ( Midi �f�[�^�{�� )
	virtual void* getSourceData() const { return mSourceData; }

	/// ���f�[�^�̃T�C�Y�̎擾
	virtual u32 getSourceDataSize() const { return mSourceDataSize; }

    /// �S�̂̍Đ����Ԃ̎擾 ( �~���[�W�b�N�^�C�� )
	virtual u32 getTotalTime() const { return 0; }

    /// �S�̂̃T���v�����̎擾 ( Midi �̏ꍇ�̓~���[�W�b�N�^�C���P�� )
    virtual u32 getTotalUnits() const { printf( "Midi::getTotalUnits() ����`" ); return 0; }

    /// CC111 �ʒu�ƃx�[�X�^�C���̎擾
    virtual void getLoopState( u32* cc111time_, u32* base_time_ ) const { *cc111time_ = mCC111Time; *base_time_ = mBaseTime; }

    /// �I�[�f�B�I�t�@�C���Ƃ��Ĉ����X�g���[����ݒ肷��
	virtual LNRESULT setStream( File::IInStream* stream_ );

    /// �����Ƀo�b�t�@���m�ۂ��AMidi �f�[�^��S�ēǂݍ��� ( �A�N�Z�X�� getSourceData() ���� )
	virtual LNRESULT fillBufferAndReleaseStream();

    /// ���ʂ𐳋K�����ēǂݍ��ނ��̐ݒ� ( fillBufferAndReleaseStream() �̑O�ɌĂԂ��� )
    void setEnableVolumeNormalize( bool flag_ ) { mVolumeNormalize = flag_; }


protected:

	/// PCM �t�H�[�}�b�g�̎擾
	virtual const LNWaveFormat* getWaveFormat() const { return NULL; }

	/// �I���������Đ��p�̃o�b�t�@�̐擪�A�h���X�擾
	virtual u8* getOnmemoryPCMBuffer() const { return NULL; }

	/// �I���������Đ����̑S�o�b�t�@�T�C�Y�̎擾
	virtual u32 getOnmemoryPCMBufferSize() const { return 0; }

	/// 1 �b������̃o�C�g���擾
	virtual u32 getBytesPerSec() const { return 0; }

	/// �f�[�^���f�R�[�h���Abuffer_ �ɏ�������
    virtual LNRESULT read( void* buffer_, u32 buffer_size_, u32* read_size_, u32* write_size_ ) { printf( "non\n" ); return LN_OK; }

	/// �t�@�C���|�C���^�ړ�
	virtual LNRESULT seek( u32 offset_ ) { printf( "non\n" ); return LN_OK; }

	/// �f�R�[�h��Ԃ̃��Z�b�g
	virtual void reset() {}

private:

    /// midi �f�[�^�̒��̕K�v�ȃf�[�^���`�F�b�N����
	void _searchData();

	/// �f���^�^�C���̓ǂݍ���
	u32 _readDelta();

	/// �g���b�N���� CC111 ����
	bool _searchTrack( u32* cc111_time_ );

private:

    /// Midi �t�@�C���̃w�b�_
    struct MidiHeader
    {
	    u8      mChunktype[ 4 ];	///< �`�����N�^�C�v ( MThd )
	    u32     mLength;			///< �f�[�^��
	    u16     mFormat;			///< �t�H�[�}�b�g�^�C�v
	    u16     mNumtrack;			///< �g���b�N��
	    u16     mTimebase;			///< �^�C���x�[�X
    };

    /// �{�����[���`�F���W�̈ʒu�ƒl ( ���K���Ɏg�� )
    struct VolumeEntry
	{
		u32     mPosition;	        ///< �{�����[���`�F���W�̒l�̈ʒu [ 00 B0 07 46 ] �� 07
		u32     mVolume;            ///< �{�����[���l
	};

private:

    File::IInStream*            mInFile;				///< ���̓X�g���[��
    char*                       mSourceData;            ///< ���f�[�^ ( Midi �f�[�^ )
	u32				            mSourceDataSize;		///< ���f�[�^�̃T�C�Y
    u32                         mCC111Time;
	u32                         mBaseTime;
    std::list< VolumeEntry >    mVolumeEntryList;
    bool                        mVolumeNormalize;
};

#endif // LNOTE_MSVC

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Audio
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================