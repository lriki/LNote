//=============================================================================
//【 DebugText 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "DebugText.h"
#include "SpriteRenderer.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// ■ DebugText クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    DebugText::DebugText()
        : mGraphicsDevice   ( NULL )
        , mRenderer         ( NULL )
        , mFontTexture      ( NULL )
        , mColor            ( LColor::WHITE )
    {
        memset( mTempBuffer, 0, sizeof( mTempBuffer ) );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    DebugText::~DebugText()
    {
        LN_SAFE_RELEASE( mFontTexture );
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void DebugText::initialize( IGraphicsDevice* device_, SpriteRenderer* renderer_ )
    {
        mGraphicsDevice = device_;
        mRenderer = renderer_;
        mDrawArea.set( 0, 0, 0, 0 );

        mGraphicsDevice->createTexture(
            &mFontTexture, _T("LNResource\\Image\\DebugFont.png") );
    }

    //---------------------------------------------------------------------
	// ● 表示位置の設定	
	//---------------------------------------------------------------------
	void DebugText::setPosition( int x_, int y_ )
	{
		mDrawArea.x = x_;
		mDrawArea.y = y_;
	}

    //---------------------------------------------------------------------
	// ● 文字色の設定	
	//---------------------------------------------------------------------
    void DebugText::setColor( const LColor& color_ )
	{
        mColor = color_;
	}

    //---------------------------------------------------------------------
	// ● 文字列を出力する	
	//---------------------------------------------------------------------
	LNRESULT DebugText::printf( const char* format_, ... )
    {
        va_list args;
		va_start( args, format_ );
		_vstprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, format_, args );
		va_end( args );
		mText += mTempBuffer;
		return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● operator <<
	//---------------------------------------------------------------------
	DebugText& DebugText::operator << ( const char* c_ )
	{
		mText += c_;
		return *this;
	}

	DebugText& DebugText::operator << ( unsigned char v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%c", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( short v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%d", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator <<( unsigned short v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%u", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( int v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%d", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( unsigned int v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%u", v_ );
		mText += mTempBuffer;
		return *this;
	}

    DebugText& DebugText::operator << ( long v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%ld", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( unsigned long v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%lu", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( float v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%.3f", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( double v_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%.3lf", v_ );
		mText += mTempBuffer;
		return *this;
	}

	DebugText& DebugText::operator << ( bool v_ )
	{
		mText += ( v_ ) ? "true" : "false";
		return *this;
	}

	DebugText& DebugText::operator << ( void* p_ )
	{
		_stprintf_s( mTempBuffer, TEMP_BUFFER_SIZE, "%08x", p_ );
		mText += mTempBuffer;
		return *this;
	}

    //---------------------------------------------------------------------
	// ● 文字列を描画する	
	//---------------------------------------------------------------------
	void DebugText::draw()
    {
        if ( !mDrawingText.empty() )
		{
            LRect src_rect(0, 0, 12, 22);
            LVector3 pos((lnfloat)mDrawArea.x, (lnfloat)mDrawArea.y, 0);
            lnfloat ox = 12;
            lnfloat oy = 22;
            lnU32 color = mColor.to32Bit(mGraphicsDevice->getGraphicsAPI());

            Graphics::IRenderer* r = mGraphicsDevice->getRenderer();
            r->setRenderState(LRenderState::SMOOTH_ALPHA);

            const LVector2& size = r->getRenderTarget(0)->getSize();
            LMatrix proj;
            LMatrix::perspective2DLH(&proj, size.x, size.y, 0, 10);

            mRenderer->setTransform(LMatrix::IDENTITY);
            mRenderer->setViewProjMatrix(LMatrix::IDENTITY, proj);
            mRenderer->setSortMode(LN_SPRITESORT_NONE, false);
            int n = mDrawingText.size();
            for (int i = 0; i < n; ++i)
            {
                lnChar c = mDrawingText[i];
                if (c == _T('\n'))
                {
                    pos.x = (lnfloat)mDrawArea.x;
                    pos.y += oy;
                }
                else
                {
                    src_rect.x = (c % 12) * src_rect.w;
                    src_rect.y = (c / 12) * src_rect.h;
                    mRenderer->drawRequest2D(pos, LVector3::ZERO, mFontTexture, src_rect, color);
                    pos.x += ox;
                }
            }
            mRenderer->flash();

            mDrawingText = _T( "" );
		}
        
    }

  



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================