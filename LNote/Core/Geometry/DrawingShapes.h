//=============================================================================
//【 DrawingShapes 】
//-----------------------------------------------------------------------------
///**
//  @file       DrawingShapes.h
//  @brief      DrawingShapes
//  @author     Riki
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
namespace Core
{
namespace Geometry
{

//=============================================================================
// ■ Point クラス
//-----------------------------------------------------------------------------
///**
//	@brief      点を表すクラス
//*/
//=============================================================================
class Point
{
public:

	int		x;		///< X 座標
	int		y;		///< Y 座標

public:

	/// デフォルトコンストラクタ
	Point() : x( 0 ), y( 0 )
	{}

	/// コンストラクタ
	Point( int x_, int y_ )
	{
		x = x_; y = y_;
	}

	/// 各要素をまとめて設定する
	void set( int x_, int y_ )
	{
		x = x_; y = y_;
	}

    
    Point operator + ( const Point& pt_ ) const
    {
        return Point( x + pt_.x, y + pt_.y );
    }

    //---------------------------------------------------------------------
	///**
	//	@brief		コンソールに出力する
	// 
	//	@param[in]	str_ : ヘッダ文字列
	// 
	//	@par
	//				要素の内容をコンソールに出力します。
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;
};

//=============================================================================
// ■ Size クラス
//-----------------------------------------------------------------------------
///**
//	@brief      2次元上での大きさを表すクラス
//*/
//=============================================================================
class Size
{
public:

	int		w;      ///< X 方向の大きさ
	int		h;      ///< Y 方向の大きさ

public:

	/// デフォルトコンストラクタ
	Size()
        : w( 0 ), h( 0 )
	{}

	/// コンストラクタ
	Size( int w_, int h_ )
        : w( w_ ), h( h_ )
	{}

	/// 各要素を設定する
	void set( int w_, int h_ )
	{
		w = w_; h = h_;
	}

    /// 要素がすべて 0 かを判定
    bool isZero() const { return ( w == 0 && h == 0 ); }

    /// いずれかの要素が 0 かを判定
    bool isAnyZero() const { return ( w == 0 || h == 0 ); }

    /// コンソールに出力する
    void dump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %d, %d )",
            ( str_ ) ? str_ : _T( "Size" ),
            w, h );
    }
};



//=============================================================================
// ■ Rect クラス
//-----------------------------------------------------------------------------
///**
//  @brief      矩形を表すクラス
//*/
//=============================================================================
template < typename T >
class RectT
{
public:

	T     x;			///< 左上隅のX座標
	T     y;			///< 左上隅のY座標
	T     w;		    ///< 矩形の幅
	T     h;		    ///< 矩形の高さ

public:

    static RectT<T> ZERO;  ///< LRect( 0, 0, 0, 0 )

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @par
    //              すべての要素を 0 で初期化します。
    //*/
    //---------------------------------------------------------------------
    RectT()
        : x( 0 ), y( 0 ), w( 0 ), h( 0 )
    {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @param[in]  x_      : 四角形の左上隅の X 座標
    //  @param[in]  x_      : 四角形の左上隅の Y 座標
    //  @param[in]  width_  : 四角形の幅
    //  @param[in]  height_ : 四角形の高さ
    //*/
    //---------------------------------------------------------------------
    RectT( T x_, T y_, T width_, T height_ )
    {
	    x = x_; y = y_; w = width_; h = height_;
    }

    /// 任意の RectT 型からの変換
    template<typename FROM_RECT_T>
    inline explicit RectT( const FROM_RECT_T& rect_ )
    {
        x = static_cast<T>( rect_.x );
        y = static_cast<T>( rect_.y );
        w = static_cast<T>( rect_.w );
        h = static_cast<T>( rect_.h );
    }

public:

    // 座標の取得
    const Point& getPoint() const { return (Point&)x; }

    // 大きさの取得
    const Size& getSize() const { return (Size&)w; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      各要素をまとめて設定する
    //
    //  @param[in]  x_      : 四角形の左上隅の X 座標
    //  @param[in]  x_      : 四角形の左上隅の Y 座標
    //  @param[in]  width_  : 四角形の幅
    //  @param[in]  height_ : 四角形の高さ
    //*/
    //---------------------------------------------------------------------
    void set( int x_, int y_, int width_, int height_ )
    {
        x = x_; y = y_; w = width_; h = height_;
    }

    /// 平行移動
    void offset( int x_, int y_ ) { x += x_; y += y; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      rect_ の矩形から padding_ 分の空白を開けた矩形を設定する
    //
    //  @par
    //              矩形の幅と高さが 0 より小さくなる場合は 0 に修正されます。
    //*/
    //---------------------------------------------------------------------
    void setPadding( const RectT<T>& rect_, int padding_ )
    {
	    x = rect_.x + padding_;
	    y = rect_.y + padding_;
	    w = rect_.w - padding_ * 2;
	    h = rect_.h - padding_ * 2;
        w = ( w  < 0 ) ? 0 : w;
        h = ( h < 0 ) ? 0 : h;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      点が矩形の中に入っているかを判定する
    //*/
    //---------------------------------------------------------------------
    bool contains( const Point& point_ ) const
	{
		return ( x <= point_.x && point_.x < x + w &&
				 y <= point_.y && point_.y < y + h );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された矩形を使ってクリッピングする
    //*/
    //---------------------------------------------------------------------
    void clip( const RectT<T>& area_ )
    {
        int l0 = x;
        int t0 = y;
        int r0 = x + w;
        int b0 = y + h;

        int l1 = area_.x;
        int t1 = area_.y;
        int r1 = area_.x + area_.w;
        int b1 = area_.y + area_.h;

        if ( l0 < l1 ) l0 = l1;
        if ( t0 < t1 ) t0 = t1;
        if ( r0 > r1 ) r0 = r1;
        if ( b0 > b1 ) b0 = b1;

        x = l0;
        y = t0;
        w = r0 - l0;
        h = b0 - t0;
    }

    //---------------------------------------------------------------------
    ///**
    //  @brief      矩形の大きさがマイナスになっているかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isMinus() const { return ( w < 0 || h < 0 ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンソールに出力する
    //*/
    //---------------------------------------------------------------------
    void dump( const char* str_ = NULL ) const
    {
        if ( str_ ) std::cout << str_;
        else        std::cout << "Rect";
        std::cout << "( " << x << ", " << y << ", " << w << ", " << h << " )" << std::endl;
    }

    /// 比較
    bool operator == ( const RectT<T>& rect_ ) const
    {
        return ( x == rect_.x && y == rect_.y && w == rect_.w && h == rect_.h );
    }
	bool operator != ( const RectT<T>& rect_ ) const
    {
        return ( x != rect_.x || y != rect_.y || w != rect_.w || h != rect_.h );
    }

};
#if 0
class Rect
{
public:

	int     x;			///< 左上隅のX座標
	int     y;			///< 左上隅のY座標
	int     w;		    ///< 矩形の幅
	int     h;		    ///< 矩形の高さ

public:

    static Rect ZERO;  ///< LRect( 0, 0, 0, 0 )

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      デフォルトコンストラクタ
    //
    //  @par
    //              すべての要素を 0 で初期化します。
    //*/
    //---------------------------------------------------------------------
    Rect();

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //
    //  @param[in]  x_      : 四角形の左上隅の X 座標
    //  @param[in]  x_      : 四角形の左上隅の Y 座標
    //  @param[in]  width_  : 四角形の幅
    //  @param[in]  height_ : 四角形の高さ
    //*/
    //---------------------------------------------------------------------
    Rect( int x_, int y_, int width_ = 0, int height_ = 0 );

public:

    // 座標の取得
    const Point& getPoint() const { return (Point&)x; }

    // 大きさの取得
    const Size& getSize() const { return (Size&)w; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      各要素をまとめて設定する
    //
    //  @param[in]  x_      : 四角形の左上隅の X 座標
    //  @param[in]  x_      : 四角形の左上隅の Y 座標
    //  @param[in]  width_  : 四角形の幅
    //  @param[in]  height_ : 四角形の高さ
    //*/
    //---------------------------------------------------------------------
    void set( int x_, int y_, int width_ = 0, int height_ = 0 );

    /// 平行移動
    void offset( int x_, int y_ ) { x += x_; y += y; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      rect_ の矩形から padding_ 分の空白を開けた矩形を設定する
    //
    //  @par
    //              矩形の幅と高さが 0 より小さくなる場合は 0 に修正されます。
    //*/
    //---------------------------------------------------------------------
    void setPadding( const Rect& rect_, int padding_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      点が矩形の中に入っているかを判定する
    //*/
    //---------------------------------------------------------------------
    bool contains( const Point& point_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      指定された矩形を使ってクリッピングする
    //*/
    //---------------------------------------------------------------------
    void clip( const Rect& area_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      矩形の大きさがマイナスになっているかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isMinus() const { return ( w < 0 || h < 0 ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンソールに出力する
    //*/
    //---------------------------------------------------------------------
    void dump( const char* str_ = NULL ) const;

    /// 比較
    bool operator == ( const Rect& rect_ ) const;
	bool operator != ( const Rect& rect_ ) const;

};

#endif

//=============================================================================
// ■ RectF クラス
//-----------------------------------------------------------------------------
///**
//  @brief      矩形を表すクラス
//*/
//=============================================================================
/*
class RectF
{
public:

	lnfloat     x;			///< 左上隅のX座標
	lnfloat     y;			///< 左上隅のY座標
	lnfloat     w;		///< 矩形の幅
	lnfloat     h;		///< 矩形の高さ

    RectF()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    explicit RectF( const Rect& rect_ )
    {
        x = static_cast< lnfloat >( rect_.x );
        y = static_cast< lnfloat >( rect_.y );
        w = static_cast< lnfloat >( rect_.w );
        h = static_cast< lnfloat >( rect_.h );
    }

    RectF( lnfloat x_, lnfloat y_, lnfloat width_, lnfloat height_ )
    {
        x = x_;
        y = y_;
        w = width_;
        h = height_;
    }

    void set( lnfloat x_, lnfloat y_, lnfloat width_, lnfloat height_ )
    {
        x = x_;
        y = y_;
        w = width_;
        h = height_;
    }
};
*/

//=============================================================================
// ■ Thickness クラス
//-----------------------------------------------------------------------------
///**
//  @brief      四角形の各辺の太さを表すクラス
//*/
//=============================================================================
class Thickness
{
public:

	lnfloat     left;			///< 左辺の太さ
	lnfloat     top;			///< 上辺の太さ
	lnfloat     right;		    ///< 右辺の太さ
	lnfloat     bottom;		    ///< 底辺の太さ

public:

    Thickness()
        : left( 0 ), top( 0 ), right( 0 ), bottom( 0 )
    {}

    Thickness( lnfloat left_, lnfloat top_, lnfloat right_, lnfloat bottom_ )
    {
        left = left_;
        top = top_;
        right = right_;
        bottom = bottom_;
    }

    void set( lnfloat left_, lnfloat top_, lnfloat right_, lnfloat bottom_ )
    {
        left = left_;
        top = top_;
        right = right_;
        bottom = bottom_;
    }
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

typedef RectT<int>      Rect;
typedef RectT<lnfloat>  RectF;

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::Point       LPoint;
typedef Core::Geometry::Size        LSize;
typedef Core::Geometry::Rect        LRect;
typedef Core::Geometry::RectF       LRectF;
typedef Core::Geometry::Thickness   LThickness;

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================