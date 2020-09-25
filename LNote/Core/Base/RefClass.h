//=============================================================================
//【 RefClass 】
//-----------------------------------------------------------------------------
///**
//  @file       RefClass.h
//  @brief      RefClass
//  @author     Riki
//*/
//=============================================================================

#pragma once

/*
	▼ 定義される関数

	// デフォルトコンストラクタ ( 何も参照していない状態 )
	TYPE_();

	// 関数の引数に nil を設定できるようにする仕掛け
	TYPE_( lnNullPtr nil_ );

	// コピーコンストラクタ ( 現在参照しているものに release() を呼び、obj_ の addRef() を呼んで新しく参照する )
	TYPE_( const TYPE_& obj_ );

	// デストラクタ ( 参照カウントをひとつ減らす ) 
	virtual ~TYPE_();

	// 代入。コピーコンストラクタと同じ
	TYPE_& operator = ( const TYPE_& obj_ );

	// 比較
	bool operator == ( const TYPE_& obj_ ) const;
	bool operator != ( const TYPE_& obj_ ) const;

	// nil 代入
	TYPE_& operator = ( const lnNullPtr null_ ) { release(); return *this; };

	// nil 比較
	bool operator == ( const lnNullPtr null_ ) const { return ( mImpl == NULL ); }
	bool operator != ( const lnNullPtr null_ ) const { return ( mImpl != NULL ); }

	// 参照カウントをひとつ減らして参照を外す。何も参照していない場合はなにもしない
	void release();

	// 以下は一応用意してあるけど使うかどうかは未定

	// ( 内部用 ) 参照オブジェクトを渡してコンストラクト ( 参照カウントに変化は無い )
	TYPE_( IMPL_TYPE_* impl_ );

	// ( 内部用 ) 参照オブジェクトを取得する ( 参照カウントに変化は無い )
	IMPL_TYPE_* getImpl() const;

*/


#ifdef LNOTE_PRIVATE_HIDE

	// 参照クラスが持つべきメンバ
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

	// 公開用としてはシングルトンの実装になっているクラスのメンバ
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


    // RefString 用。比較演算子カット版
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

	// ダミークラスが持つべきメンバ
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

	// 公開用としてはシングルトンの実装になっているクラスのメンバ
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


    // RefString 用。比較演算子カット版
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