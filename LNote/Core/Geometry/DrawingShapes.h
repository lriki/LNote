//=============================================================================
//�y DrawingShapes �z
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
// �� Point �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �_��\���N���X
//*/
//=============================================================================
class Point
{
public:

	int		x;		///< X ���W
	int		y;		///< Y ���W

public:

	/// �f�t�H���g�R���X�g���N�^
	Point() : x( 0 ), y( 0 )
	{}

	/// �R���X�g���N�^
	Point( int x_, int y_ )
	{
		x = x_; y = y_;
	}

	/// �e�v�f���܂Ƃ߂Đݒ肷��
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
	//	@brief		�R���\�[���ɏo�͂���
	// 
	//	@param[in]	str_ : �w�b�_������
	// 
	//	@par
	//				�v�f�̓��e���R���\�[���ɏo�͂��܂��B
	//*/
	//---------------------------------------------------------------------
	void dump( const char* str_ = NULL ) const;
};

//=============================================================================
// �� Size �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      2������ł̑傫����\���N���X
//*/
//=============================================================================
class Size
{
public:

	int		w;      ///< X �����̑傫��
	int		h;      ///< Y �����̑傫��

public:

	/// �f�t�H���g�R���X�g���N�^
	Size()
        : w( 0 ), h( 0 )
	{}

	/// �R���X�g���N�^
	Size( int w_, int h_ )
        : w( w_ ), h( h_ )
	{}

	/// �e�v�f��ݒ肷��
	void set( int w_, int h_ )
	{
		w = w_; h = h_;
	}

    /// �v�f�����ׂ� 0 ���𔻒�
    bool isZero() const { return ( w == 0 && h == 0 ); }

    /// �����ꂩ�̗v�f�� 0 ���𔻒�
    bool isAnyZero() const { return ( w == 0 || h == 0 ); }

    /// �R���\�[���ɏo�͂���
    void dump( const lnChar* str_ = NULL ) const
    {
        _tprintf( "%s ( %d, %d )",
            ( str_ ) ? str_ : _T( "Size" ),
            w, h );
    }
};



//=============================================================================
// �� Rect �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ��`��\���N���X
//*/
//=============================================================================
template < typename T >
class RectT
{
public:

	T     x;			///< �������X���W
	T     y;			///< �������Y���W
	T     w;		    ///< ��`�̕�
	T     h;		    ///< ��`�̍���

public:

    static RectT<T> ZERO;  ///< LRect( 0, 0, 0, 0 )

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @par
    //              ���ׂĂ̗v�f�� 0 �ŏ��������܂��B
    //*/
    //---------------------------------------------------------------------
    RectT()
        : x( 0 ), y( 0 ), w( 0 ), h( 0 )
    {}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  x_      : �l�p�`�̍������ X ���W
    //  @param[in]  x_      : �l�p�`�̍������ Y ���W
    //  @param[in]  width_  : �l�p�`�̕�
    //  @param[in]  height_ : �l�p�`�̍���
    //*/
    //---------------------------------------------------------------------
    RectT( T x_, T y_, T width_, T height_ )
    {
	    x = x_; y = y_; w = width_; h = height_;
    }

    /// �C�ӂ� RectT �^����̕ϊ�
    template<typename FROM_RECT_T>
    inline explicit RectT( const FROM_RECT_T& rect_ )
    {
        x = static_cast<T>( rect_.x );
        y = static_cast<T>( rect_.y );
        w = static_cast<T>( rect_.w );
        h = static_cast<T>( rect_.h );
    }

public:

    // ���W�̎擾
    const Point& getPoint() const { return (Point&)x; }

    // �傫���̎擾
    const Size& getSize() const { return (Size&)w; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�v�f���܂Ƃ߂Đݒ肷��
    //
    //  @param[in]  x_      : �l�p�`�̍������ X ���W
    //  @param[in]  x_      : �l�p�`�̍������ Y ���W
    //  @param[in]  width_  : �l�p�`�̕�
    //  @param[in]  height_ : �l�p�`�̍���
    //*/
    //---------------------------------------------------------------------
    void set( int x_, int y_, int width_, int height_ )
    {
        x = x_; y = y_; w = width_; h = height_;
    }

    /// ���s�ړ�
    void offset( int x_, int y_ ) { x += x_; y += y; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      rect_ �̋�`���� padding_ ���̋󔒂��J������`��ݒ肷��
    //
    //  @par
    //              ��`�̕��ƍ����� 0 ��菬�����Ȃ�ꍇ�� 0 �ɏC������܂��B
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
    //  @brief      �_����`�̒��ɓ����Ă��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool contains( const Point& point_ ) const
	{
		return ( x <= point_.x && point_.x < x + w &&
				 y <= point_.y && point_.y < y + h );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ��`���g���ăN���b�s���O����
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
    //  @brief      ��`�̑傫�����}�C�i�X�ɂȂ��Ă��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool isMinus() const { return ( w < 0 || h < 0 ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���\�[���ɏo�͂���
    //*/
    //---------------------------------------------------------------------
    void dump( const char* str_ = NULL ) const
    {
        if ( str_ ) std::cout << str_;
        else        std::cout << "Rect";
        std::cout << "( " << x << ", " << y << ", " << w << ", " << h << " )" << std::endl;
    }

    /// ��r
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

	int     x;			///< �������X���W
	int     y;			///< �������Y���W
	int     w;		    ///< ��`�̕�
	int     h;		    ///< ��`�̍���

public:

    static Rect ZERO;  ///< LRect( 0, 0, 0, 0 )

public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �f�t�H���g�R���X�g���N�^
    //
    //  @par
    //              ���ׂĂ̗v�f�� 0 �ŏ��������܂��B
    //*/
    //---------------------------------------------------------------------
    Rect();

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���X�g���N�^
    //
    //  @param[in]  x_      : �l�p�`�̍������ X ���W
    //  @param[in]  x_      : �l�p�`�̍������ Y ���W
    //  @param[in]  width_  : �l�p�`�̕�
    //  @param[in]  height_ : �l�p�`�̍���
    //*/
    //---------------------------------------------------------------------
    Rect( int x_, int y_, int width_ = 0, int height_ = 0 );

public:

    // ���W�̎擾
    const Point& getPoint() const { return (Point&)x; }

    // �傫���̎擾
    const Size& getSize() const { return (Size&)w; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �e�v�f���܂Ƃ߂Đݒ肷��
    //
    //  @param[in]  x_      : �l�p�`�̍������ X ���W
    //  @param[in]  x_      : �l�p�`�̍������ Y ���W
    //  @param[in]  width_  : �l�p�`�̕�
    //  @param[in]  height_ : �l�p�`�̍���
    //*/
    //---------------------------------------------------------------------
    void set( int x_, int y_, int width_ = 0, int height_ = 0 );

    /// ���s�ړ�
    void offset( int x_, int y_ ) { x += x_; y += y; }

    //---------------------------------------------------------------------
    ///**
    //  @brief      rect_ �̋�`���� padding_ ���̋󔒂��J������`��ݒ肷��
    //
    //  @par
    //              ��`�̕��ƍ����� 0 ��菬�����Ȃ�ꍇ�� 0 �ɏC������܂��B
    //*/
    //---------------------------------------------------------------------
    void setPadding( const Rect& rect_, int padding_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      �_����`�̒��ɓ����Ă��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool contains( const Point& point_ ) const;

    //---------------------------------------------------------------------
    ///**
    //  @brief      �w�肳�ꂽ��`���g���ăN���b�s���O����
    //*/
    //---------------------------------------------------------------------
    void clip( const Rect& area_ );

    //---------------------------------------------------------------------
    ///**
    //  @brief      ��`�̑傫�����}�C�i�X�ɂȂ��Ă��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool isMinus() const { return ( w < 0 || h < 0 ); }

    //---------------------------------------------------------------------
    ///**
    //  @brief      �R���\�[���ɏo�͂���
    //*/
    //---------------------------------------------------------------------
    void dump( const char* str_ = NULL ) const;

    /// ��r
    bool operator == ( const Rect& rect_ ) const;
	bool operator != ( const Rect& rect_ ) const;

};

#endif

//=============================================================================
// �� RectF �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      ��`��\���N���X
//*/
//=============================================================================
/*
class RectF
{
public:

	lnfloat     x;			///< �������X���W
	lnfloat     y;			///< �������Y���W
	lnfloat     w;		///< ��`�̕�
	lnfloat     h;		///< ��`�̍���

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
// �� Thickness �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �l�p�`�̊e�ӂ̑�����\���N���X
//*/
//=============================================================================
class Thickness
{
public:

	lnfloat     left;			///< ���ӂ̑���
	lnfloat     top;			///< ��ӂ̑���
	lnfloat     right;		    ///< �E�ӂ̑���
	lnfloat     bottom;		    ///< ��ӂ̑���

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