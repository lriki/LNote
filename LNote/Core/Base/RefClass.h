//=============================================================================
//�y RefClass �z
//-----------------------------------------------------------------------------
///**
//  @file       RefClass.h
//  @brief      RefClass
//  @author     Riki
//*/
//=============================================================================

#pragma once

/*
	�� ��`�����֐�

	// �f�t�H���g�R���X�g���N�^ ( �����Q�Ƃ��Ă��Ȃ���� )
	TYPE_();

	// �֐��̈����� nil ��ݒ�ł���悤�ɂ���d�|��
	TYPE_( lnNullPtr nil_ );

	// �R�s�[�R���X�g���N�^ ( ���ݎQ�Ƃ��Ă�����̂� release() ���ĂсAobj_ �� addRef() ���Ă�ŐV�����Q�Ƃ��� )
	TYPE_( const TYPE_& obj_ );

	// �f�X�g���N�^ ( �Q�ƃJ�E���g���ЂƂ��炷 ) 
	virtual ~TYPE_();

	// ����B�R�s�[�R���X�g���N�^�Ɠ���
	TYPE_& operator = ( const TYPE_& obj_ );

	// ��r
	bool operator == ( const TYPE_& obj_ ) const;
	bool operator != ( const TYPE_& obj_ ) const;

	// nil ���
	TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; };

	// nil ��r
	bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); }

	// �Q�ƃJ�E���g���ЂƂ��炵�ĎQ�Ƃ��O���B�����Q�Ƃ��Ă��Ȃ��ꍇ�͂Ȃɂ����Ȃ�
	void release();

	// �ȉ��͈ꉞ�p�ӂ��Ă��邯�ǎg�����ǂ����͖���

	// ( �����p ) �Q�ƃI�u�W�F�N�g��n���ăR���X�g���N�g ( �Q�ƃJ�E���g�ɕω��͖��� )
	TYPE_( IMPL_TYPE_* impl_ );

	// ( �����p ) �Q�ƃI�u�W�F�N�g���擾���� ( �Q�ƃJ�E���g�ɕω��͖��� )
	IMPL_TYPE_* getImpl() const;

*/


#ifdef LNOTE_PRIVATE_HIDE

	// �Q�ƃN���X�����ׂ������o
	#define LN_REFERENCE_TYPE_METHODS( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_(); \
			TYPE_( lnNullPtr nil_ ); \
			TYPE_( const TYPE_& obj_ ); \
			virtual ~TYPE_(); \
			TYPE_& operator = ( const TYPE_& obj_ ); \
			bool operator == ( const TYPE_& obj_ ) const; \
			bool operator != ( const TYPE_& obj_ ) const; \
			TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; }; \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
			void release();

	// ���J�p�Ƃ��Ă̓V���O���g���̎����ɂȂ��Ă���N���X�̃����o
	#define LN_SINGLETON_REFERENCE_METHODS( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_() : mImpl( 0 ) {} \
			TYPE_( const TYPE_& obj_ ) : mImpl( obj_.mImpl ) {} \
			virtual ~TYPE_() {}; \
			TYPE_& operator = ( const TYPE_& obj_ ) { mImpl = obj_.mImpl; } \
			bool operator == ( const TYPE_& obj_ ) const { return ( mImpl == obj_.mImpl ); } \
			bool operator != ( const TYPE_& obj_ ) const { return ( mImpl != obj_.mImpl ); } \
			TYPE_& operator = ( const lnNullPtr null_ ) { mImpl = NULL; } \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
		private: \
			IMPL_TYPE_* mImpl;


    // RefString �p�B��r���Z�q�J�b�g��
	#define LN_REFERENCE_TYPE_METHODS_NOT_CMP( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_(); \
			TYPE_( lnNullPtr nil_ ); \
			TYPE_( const TYPE_& obj_ ); \
			virtual ~TYPE_(); \
			TYPE_& operator = ( const TYPE_& obj_ ); \
			TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; }; \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
			void release();

#else

	// �_�~�[�N���X�����ׂ������o
	#define LN_REFERENCE_TYPE_METHODS( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_(); \
			TYPE_( lnNullPtr nil_ ); \
			TYPE_( const TYPE_& obj_ ); \
			virtual ~TYPE_(); \
			TYPE_& operator = ( const TYPE_& obj_ ); \
			bool operator == ( const TYPE_& obj_ ) const; \
			bool operator != ( const TYPE_& obj_ ) const; \
			TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; }; \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
			void release(); \
			TYPE_( IMPL_TYPE_* impl_ ); \
			IMPL_TYPE_* getImpl() const;

	// ���J�p�Ƃ��Ă̓V���O���g���̎����ɂȂ��Ă���N���X�̃����o
	#define LN_SINGLETON_REFERENCE_METHODS( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_() : mImpl( 0 ) {} \
			TYPE_( const TYPE_& obj_ ) : mImpl( obj_.mImpl ) {} \
			virtual ~TYPE_() {}; \
			TYPE_& operator = ( const TYPE_& obj_ ) { mImpl = obj_.mImpl; return *this; } \
			bool operator == ( const TYPE_& obj_ ) const { return ( mImpl == obj_.mImpl ); } \
			bool operator != ( const TYPE_& obj_ ) const { return ( mImpl != obj_.mImpl ); } \
			TYPE_& operator = ( const lnNullPtr null_ ) { mImpl = NULL; return *this; } \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
			TYPE_( IMPL_TYPE_* impl_ ) : mImpl( impl_ ) {} \
		private: \
			IMPL_TYPE_* mImpl;

			 //TYPE_& operator = ( IMPL_TYPE_* impl_ ) { mImpl = impl_; return *this; }


    // RefString �p�B��r���Z�q�J�b�g��
	#define LN_REFERENCE_TYPE_METHODS_NOT_CMP( TYPE_, IMPL_TYPE_ ) \
		public: \
			TYPE_(); \
			TYPE_( lnNullPtr nil_ ); \
			TYPE_( const TYPE_& obj_ ); \
			virtual ~TYPE_(); \
			TYPE_& operator = ( const TYPE_& obj_ ); \
			TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; }; \
			bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); } \
			bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); } \
			void release(); \
			TYPE_( IMPL_TYPE_* impl_ ); \
			IMPL_TYPE_* getImpl() const;
#endif




//=============================================================================
//								end of file
//=============================================================================