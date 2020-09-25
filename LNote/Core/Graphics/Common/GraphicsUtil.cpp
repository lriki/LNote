//=============================================================================
//�y GraphicsUtil �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsUtil.h"

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
// �� Util �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� ���_�t�H�[�}�b�g���璸�_�̃T�C�Y�����߂�
    //---------------------------------------------------------------------
    u32 Util::getVertexSize( const LNVertexElemenst* element_, u32 stream_index_ )
    {
        u32 size = 0;
        u32 i = 0;

        while ( true )
        {
            if ( element_[ i ].StreamIndex != stream_index_ || element_[ i ].Usage == LN_USAGE_UNKNOWN )
            {
                break;
            }

            size += getVertexDeclTypeSize( element_[ i ].Type );

            ++i;
        }

        return size;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    lnU32 Util::getVertexDeclTypeSize( LNDeclType type_ )
    {
        switch ( type_ )
	    {
		    case LN_DECL_FLOAT1: 
		    	return sizeof( float );
		    case LN_DECL_FLOAT2: 
		    	return sizeof( float ) * 2;
		    case LN_DECL_FLOAT3: 
		    	return sizeof( float ) * 3;
		    case LN_DECL_FLOAT4: 
		    	return sizeof( float ) * 4;
		    case LN_DECL_UBYTE4: 
		    	return sizeof( unsigned char ) * 4;
		    case LN_DECL_COLOR4: 
		    	return sizeof( unsigned char ) * 4;
		    case LN_DECL_SHORT2: 
		    	return sizeof( short ) * 2;
		    case LN_DECL_SHORT4: 
		    	return sizeof( short ) * 4;
	    }
        return 0;
    }

    //---------------------------------------------------------------------
    // �� �r�b�g�}�b�v�]���̍ۂ̕K�v�ȃ��[�v�����v�Z���� (���[�v���� 0 �ȉ��̏ꍇ false ��Ԃ�)
    //---------------------------------------------------------------------
    bool calcBitBltLoopNum(
        int* loop_x_, int* loop_y_,
        int* x_, int* y_, int r_, int b_,
        int dest_w_, int dest_h_,
        int sx_, int sy_,
        int sw_, int sh_, int  src_w_, int src_h_ )
    {
        // �]����͈̔͂ɓ����Ă��邩�`�F�b�N
		if ( *x_ + sw_ < 0 ||
			 *y_ + sh_ < 0 ||
			 *x_ >= dest_w_ ||
			 *y_ >= dest_h_ )
		{
			return false;
		}
		// �]�����͈̔͂ɓ����Ă��邩�`�F�b�N
		if ( sx_ + sw_ < 0 ||
			 sy_ + sh_ < 0 ||
			 sx_ >= src_w_ ||
			 sy_ >= src_h_ )
		{
			return false;
		}
		// ���[�v�񐔂𒲂ׂĂ���
		*loop_x_ = sw_;
		*loop_y_ = sh_;
		if ( *x_ < 0 )
		{
			*loop_x_ += *x_;
			*x_ = 0;
		}
        if ( *x_ + *loop_x_ >= r_ )
        {
            *loop_x_ -= ( *x_ + *loop_x_ ) - r_;
        }
		else if ( *x_ + *loop_x_ >= dest_w_ )
		{
			*loop_x_ -= ( *x_ + *loop_x_ ) - dest_w_;
		}
        
		if ( *y_ < 0 )
		{
			*loop_y_ += *y_;
			*y_ = 0;
		}
        if ( *y_ + *loop_y_ >= b_ )
        {
            *loop_y_ -= ( *y_ + *loop_y_ ) - b_;
        }
		else if ( *y_ + *loop_y_ >= dest_h_ )
		{
			*loop_y_ -= ( *y_ + *loop_y_ ) - dest_h_;
		}
        return true;
    }

    //---------------------------------------------------------------------
    // �� �r�b�g�}�b�v�̓]�����s��
    //---------------------------------------------------------------------
    void Util::bitblt(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ )
    {
        bitblt( 
            x_,  y_ , dest_w_ - x_, dest_h_ - y_, dest_, dest_w_, dest_h_,
	        sx_, sy_, sw_, sh_, src_ , src_w_ , src_h_, alpha_ );
    }

    //---------------------------------------------------------------------
    // �� �r�b�g�}�b�v�̓]�����s�� (�E���N���b�s���O�t��)
    //---------------------------------------------------------------------
    void Util::bitblt(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u32* src_ , int src_w_ , int src_h_,
        bool alpha_ )
    {
        int loop_x, loop_y;
        if ( !calcBitBltLoopNum( &loop_x, &loop_y, &x_, &y_, r_, b_, dest_w_, dest_h_, sx_, sy_, sw_, sh_, src_w_, src_h_ ) ) { 
	return; }
		
        // �A���t�@�u�����h���s���ꍇ
        if ( alpha_ )
        {
            u32* write_pos;     // �������ރs�N�Z��
            u32  color;         // �������ސF
            u32  dest_color;
            u32  dest_alpha;    // �]����A���t�@�l (0�`255)
            u32  src_color;
            u32  src_alpha;     // �]�����A���t�@�l (0�`255)
            u32  a, r, g, b;

            for ( int y = 0; y < loop_y; ++y )
		    {
			    for ( int x = 0; x < loop_x; ++x )
			    {
                    

                    // �������݈ʒu
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    
                    // �]����A�]�����̃A���t�@�l
                    dest_color = *write_pos;
                    dest_alpha = ( *write_pos >> 24 ) & 0xff;
                    src_color  = *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) );
                    src_alpha  = ( src_color & 0xff000000 ) >> 24;


                    
                    if ( dest_alpha == 0 )
                    {
                        a = 0xff;
                    }
                    else if ( src_alpha == 0 )
                    {
                        a = 0;
                    }
                    else
                    {
                        a = src_alpha;// * ( ( color_ & 0xff000000 ) >> 24 );
                        //a >>= 8;
                    }
                  

                    r = ( ( ( dest_color & 0x00ff0000 ) >> 16 ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x00ff0000 ) >> 16 ) * a );
                    r >>= 8;
                    r = ( r > 255 ) ? 255 : r;

                    g = ( ( ( dest_color & 0x0000ff00 ) >> 8 ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x0000ff00 ) >> 8 ) * a );
                    g >>= 8;
                    g = ( g > 255 ) ? 255 : g;

                    b = ( ( ( dest_color & 0x000000ff ) ) * ( 255 - a ) ) +
                        ( ( ( src_color  & 0x000000ff ) ) * a );
                    b >>= 8;
                    b = ( b > 255 ) ? 255 : b;

                    // �A���t�@�l���������ݗp�̃A���t�@�l�Ƃ��Čv�Z������
                    a = ( dest_alpha + src_alpha );
                    a = ( a > 255 ) ? 255 : a;

                    color = ( a << 24 ) |
                            ( r << 16 ) |
                            ( g << 8 ) |
                            ( b );
                
                    // ��������
                    *write_pos = color;
			    }
            }
        }
        // �A���t�@�u�����h���s��Ȃ��ꍇ
        else
        {
		    for ( int y = 0; y < loop_y; ++y )
		    {
			    //for ( int x = 0; x < loop_x; ++x )
			    //{
			    //	dest_[ ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) ] = src_[ ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ];
			    //}
                // �������܂�R�s
                memcpy( dest_ + ( x_ + ( ( y_ + y ) * dest_w_ ) ), src_ + ( sx_ + ( ( sy_ + y ) * src_w_ ) ), sizeof( u32 ) * loop_x );
		    }
        }
    }

    //---------------------------------------------------------------------
    // �� �r�b�g�}�b�v�̓]�����s�� (�]�����r�b�g�}�b�v�� u8 �̃A���t�@�}�X�N�Ƃ���)
    //---------------------------------------------------------------------
    void Util::bitbltAlphaByte(
        int x_,  int y_ , u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ )
    {
        bitbltAlphaByte(
            x_, y_, dest_w_ - x_, dest_h_ - y_, dest_, dest_w_, dest_h_, 
            sx_, sy_, sw_, sh_, src_, a_level_, color_, src_w_, src_h_, alpha_ );
    }

    //---------------------------------------------------------------------
    // �� �r�b�g�}�b�v�̓]�����s�� (�]�����r�b�g�}�b�v�� u8 �̃A���t�@�}�X�N�Ƃ���)
    //---------------------------------------------------------------------
    void Util::bitbltAlphaByte(
        int x_,  int y_ , int  r_, int  b_, u32* dest_, int dest_w_, int dest_h_,
	    int sx_, int sy_, int sw_, int sh_, u8* src_, u8 a_level_, u32 color_, int src_w_ , int src_h_,
        bool alpha_ )
    {
        int loop_x, loop_y;
        if ( !calcBitBltLoopNum( &loop_x, &loop_y, &x_, &y_, r_, b_, dest_w_, dest_h_, sx_, sy_, sw_, sh_, src_w_, src_h_ ) ) { 
	return; }

        // �A���t�@�u�����h���s���ꍇ
        if ( alpha_ )
        {
            const u8 level = 255 / a_level_;    // src_ ����擾�����l�ɂ�����|����� 0�`255 �ɕϊ������l�ɂȂ�
            u32* write_pos;     // �������ރs�N�Z��
            u32  color;         // �������ސF
            u32  dest_color;
            u32  dest_alpha;    // �]����A���t�@�l (0�`255)
            u32  src_alpha;     // �]�����A���t�@�l (0�`255)
            u32  a, r, g, b;

            for ( int y = 0; y < loop_y; ++y )
		    {
			    for ( int x = 0; x < loop_x; ++x )
			    {
                    

                    // �������݈ʒu
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    
                    // �]����A�]�����̃A���t�@�l
                    dest_color = *write_pos;
                    dest_alpha = ( *write_pos >> 24 ) & 0xff;
                    src_alpha  = *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) ) * level;

                    //// �]�����̒l���u�����h�p�̃A���t�@�l�ɂ���
                    //a = src_alpha * ( ( color_ & 0xff000000 ) >> 24 );
                    //a >>= 8;
                    // �����ƁAdest �� 0x00000000 �������ꍇ�� src �̃A���t�@�l�� 0xff �ł͂Ȃ�������
                    // ���ƃu�����h����č�����ł��܂��B

                    
                    if ( dest_alpha == 0 )
                    {
                        a = 0xff;
                    }
                    else if ( src_alpha == 0 )
                    {
                        a = 0;
                    }
                    else
                    {
                        a = src_alpha * ( ( color_ & 0xff000000 ) >> 24 );
                        a >>= 8;
                    }
                  

                    r = ( ( ( dest_color & 0x00ff0000 ) >> 16 ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x00ff0000 ) >> 16 ) * a );
                    r >>= 8;
                    r = ( r > 255 ) ? 255 : r;

                    g = ( ( ( dest_color & 0x0000ff00 ) >> 8 ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x0000ff00 ) >> 8 ) * a );
                    g >>= 8;
                    g = ( g > 255 ) ? 255 : g;

                    b = ( ( ( dest_color & 0x000000ff ) ) * ( 255 - a ) ) +
                        ( ( ( color_     & 0x000000ff ) ) * a );
                    b >>= 8;
                    b = ( b > 255 ) ? 255 : b;

                    // �A���t�@�l���������ݗp�̃A���t�@�l�Ƃ��Čv�Z������
                    a = ( dest_alpha + ((src_alpha * ( ( color_ & 0xff000000 ) >> 24 ))>>8) );
                    a = ( a > 255 ) ? 255 : a;

                    color = ( a << 24 ) |
                            ( r << 16 ) |
                            ( g << 8 ) |
                            ( b );
                
                    // ��������
                    *write_pos = color;
			    }
            }
        }
        // �A���t�@�u�����h���s��Ȃ��ꍇ
        else
        {            
            const u8 level = 255 / a_level_;    // src_ ����擾�����l�ɂ�����|����� 0�`255 �ɕϊ������l�ɂȂ�
            u32* write_pos;                     // �������ރs�N�Z��
            u32  color;                         // �������ސF

		    for ( int y = 0; y < loop_y; ++y )
		    {
                for ( int x = 0; x < loop_x; ++x )
			    {
                    // �������݈ʒu�ƁA�]������A���t�@�l���擾���Ă���
                    write_pos = dest_ + ( ( x_ + x ) + ( ( y_ + y ) * dest_w_ ) );
                    color = ( *( src_ + ( ( sx_ + x ) + ( ( sy_ + y ) * src_w_ ) ) ) );

                    color *= level;                                     // ������ 0�`255 �ɕϊ�
                    color *= ( ( color_ & 0xff000000 ) >> 24 );         // color_ �̃A���t�@�l�Ə�Z
                    color >>= 8;                                        // ��Z���ʂ� 0�`255 �ɕϊ�
                    color = ( color << 24 ) | ( color_ & 0x00ffffff );  // ���̐F�Ƃ������ď������ސF�̊���

                    // ������������ƍœK���ł��邯�ǂ���܂茩�h���Ȃ��Ă��A���Ȃ̂łƂ肠���������܂�
                    
                    // ��������
                    *write_pos = color;
                }
		    }
        }
    }

    //---------------------------------------------------------------------
    // �� �e�N�X�`���t�H�[�}�b�g������� LNSurfaceFormat �ɕϊ�����
    //---------------------------------------------------------------------
    LNSurfaceFormat Util::convertFMTStringToLNFMT( const lnChar* fmt_ )
    {
#define LN_STRCMP( a_, b_ )   ( _tcsicmp( a_, _T( b_ ) ) == 0 )

        ln_std_tstring tfmt = fmt_;

        // D3DFMT_�AFMT_ ������΍폜
        Base::StringUtil::replaceString( &tfmt, _T( "D3DFMT_" ), _T( "" ) );
        Base::StringUtil::replaceString( &tfmt, _T( "FMT_" ), _T( "" ) );

        const lnChar* fmt = tfmt.c_str();

        // �悭�g�����̂��ɔ���
        if LN_STRCMP( fmt, "A8R8G8B8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8R8G8B8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A16B16G16R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "A32B32G32R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "D24S8" ) { 
        	return LN_FMT_D24S8; }


        // �o�b�N�o�b�t�@�[ �t�H�[�}�b�g�܂��̓f�B�X�v���C �t�H�[�}�b�g
        if LN_STRCMP( fmt, "A2R10G10B10" ) { 
        	return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "A8R8G8B8" ) { return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "X8R8G8B8" ) { return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R5G6B5" ) { 
        	return LN_FMT_A8R8G8B8; }

        // �o�b�t�@�[ �t�H�[�}�b�g
        if LN_STRCMP( fmt, "D16_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D15S1" ) { 
        	return LN_FMT_D24S8; }
        //if LN_STRCMP( fmt, "D24S8" ) { return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24X8" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24X4S4" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32F_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D24FS8" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D32_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "S8_LOCKABLE" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "D16" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "VERTEXDATA" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "INDEX16" ) { 
        	return LN_FMT_D24S8; }
        if LN_STRCMP( fmt, "INDEX32" ) { 
        	return LN_FMT_D24S8; }
        
        // DXTn ���k�e�N�X�`���[ �t�H�[�}�b�g
        if LN_STRCMP( fmt, "DXT1" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT2" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT3" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "DXT5" ) { 
        	return LN_FMT_A8R8G8B8; }

        // ���������_�t�H�[�}�b�g
        if LN_STRCMP( fmt, "R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "G16R16F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        //if LN_STRCMP( fmt, "A16B16G16R16F" ) { return LN_FMT_A16B16G16R16F; }

        // FOURCC �t�H�[�}�b�g
        if LN_STRCMP( fmt, "MULTI2_ARGB8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "G8R8_G8B8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R8G8_B8G8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "UYVY" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "YUY2" ) { 
        	return LN_FMT_A8R8G8B8; }

        // IEEE �t�H�[�}�b�g
        if LN_STRCMP( fmt, "R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        if LN_STRCMP( fmt, "G32R32F" ) { 
        	return LN_FMT_A16B16G16R16F; }
        //if LN_STRCMP( fmt, "A32B32G32R32F" ) { return LN_FMT_A16B16G16R16F; }

        // �~�b�N�X �t�H�[�}�b�g
        if LN_STRCMP( fmt, "L6V5U5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8L8V8U8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A2W10V10U10" ) { 
        	return LN_FMT_A8R8G8B8; }

        // �����t���t�H�[�}�b�g
        if LN_STRCMP( fmt, "V8U8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "Q8W8V8U8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "V16U16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "Q16W16V16U16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "CxV8U8" ) { 
        	return LN_FMT_X8R8G8B8; }

        // �����Ȃ��t�H�[�}�b�g
        if LN_STRCMP( fmt, "R8G8B8" ) { 
        	return LN_FMT_X8R8G8B8; }
        //if LN_STRCMP( fmt, "A8R8G8B8" ) { return LN_FMT_A8R8G8B8; }
        //if LN_STRCMP( fmt, "X8R8G8B8" ) { return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "R5G6B5" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "X1R5G5B5" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A1R5G5B5" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A4R4G4B4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "R3G3B2" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8R3G3B2" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X4R4G4B4" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A2B10G10R10" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8B8G8R8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "X8B8G8R8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "G16R16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A2R10G10B10" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A16B16G16R16" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A8P8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "P8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "L8" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "L16" ) { 
        	return LN_FMT_X8R8G8B8; }
        if LN_STRCMP( fmt, "A8L8" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A4L4" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "A1" ) { 
        	return LN_FMT_A8R8G8B8; }
        if LN_STRCMP( fmt, "BINARYBUFFER" ) { 
        	return LN_FMT_X8R8G8B8; }

        return LN_FMT_X8R8G8B8;

#undef LN_STRCMP
    }

   /*








�����t���t�H�[�}�b�g�̃f�[�^�́A���ł����ł����܂��܂���B�����t���t�H�[�}�b�g�ł́A(U)�A(V)�A(W)�A����� (Q) �̃f�[�^��g�ݍ��킹�Ďg�p���܂��B

�����t���t�H�[�}�b�g �t���O	�l	�t�H�[�}�b�g


�����Ȃ��t�H�[�}�b�g�̃f�[�^�́A���łȂ���΂Ȃ�܂���B�����Ȃ��t�H�[�}�b�g�ł́A�� (R)�A�� (G)�A�� (B)�A�A���t�@ (A)�A�P�x (L)�A�p���b�g (P) �̃f�[�^��g�ݍ��킹�Ďg�p���܂��B�p���b�g �f�[�^�́A�J���[ �p���b�g�̃C���f�b�N�X�쐬�Ɏg�p�����̂ŁA�J���[ �C���f�b�N�X �f�[�^�Ƃ��Ă΂�܂��B

�����t���t�H�[�}�b�g �t���O	�l	�t�H�[�}�b�g
���̑�

���̃t���O�́A����`�̃t�H�[�}�b�g�Ɏg�p����܂��B

���̑��̃t���O	�l	�t�H�[�}�b�g
D3DFMT_UNKNOWN
*/

    //---------------------------------------------------------------------
    // �� �e�N�X�`���̍쐬
    //---------------------------------------------------------------------
    LNRESULT Util::createTexture( ITexture** texture_, IGraphicsDevice* device_, u32 width_, u32 height_ )
    {
        return device_->createTexture(
            texture_, width_, height_, 1, LN_FMT_A8R8G8B8 );
    }


  
//=============================================================================
// �� LNFXShaderSource
//=============================================================================

struct LNFXTag
{
	const char* TagName;
	const char*	DefineStr;
};

static LNFXTag gLNFXTag[] =
{
	{ "@hlsl",		    "#ifdef LN_HLSL" },
	{ "@endhlsl",	    "#endif" },
	{ "@glslvert",	    "#ifdef LN_GLSL_VERT" },
	{ "@endglslvert",   "#endif" },
	{ "@glslflag",	    "#ifdef LN_GLSL_FLAG" },
	{ "@endglslflag",   "#endif" },
	{ "@glslmeta",		"#ifdef LN_GLSL_META" },
	{ "@endglslmeta",   "#endif" }
};

static const char* GLSL_META_DATA_BEGIN = gLNFXTag[6].DefineStr;
static const char* GLSL_META_DATA_END   = gLNFXTag[7].DefineStr;


const char* LNFXShaderSource::HLSL_MACRO      = "LN_HLSL";
const char* LNFXShaderSource::GLSL_VERT_MACRO = "LN_GLSL_VERT";
const char* LNFXShaderSource::GLSL_FLAG_MACRO = "LN_GLSL_FLAG";
const char* LNFXShaderSource::GLSL_META_MACRO = "LN_GLSL_META";


    void LNFXShaderSource::initialize(const char* buf_, ShaderSystem system_)
    {
	    mSource = buf_;

	    for (int i = 0; i < 8; ++i)
	    {
            Base::StringUtil::replaceString(&mSource, gLNFXTag[i].TagName, gLNFXTag[i].DefineStr);
	    }

	    //printf("---------------------------\n");
        //printf("%s\n", mSource.c_str());

        _splitMetaData();

		if (system_ == SHADER_GLSL)
		{
			Base::StringUtil::replaceString(&mSource, "\"", " ");
		}
	    
    /*
	    int begin_str_len = strlen(GLSL_META_DATA_BEGIN);
	    mGLSLMetaDataSource = std::string(
		    mSource.c_str(),
		    meta_begin_idx + begin_str_len,
		    meta_end_idx - meta_begin_idx - begin_str_len );

        // �b��CRLF�΍�
        if (mGLSLMetaDataSource[0] == 13 && mGLSLMetaDataSource[1] == 10)
        {
            mGLSLMetaDataSource = "";
        }
    */

       /* printf("---------------------------\n");
	    printf("%s\n", mGLSLMetaDataSource.c_str());

        printf("---------------------------\n");*/
    }

    void LNFXShaderSource::_splitMetaData()
    {
        // "#if LN_GLSL_META" ��T��
        std::string::size_type meta_begin_idx = mSource.find( GLSL_META_DATA_BEGIN );
        if ( meta_begin_idx == std::string::npos )
        {
            return;
        }

	    // "#endif" ��T��
        std::string::size_type meta_end_idx = mSource.find( GLSL_META_DATA_END, meta_begin_idx );
        if ( meta_end_idx == std::string::npos )
        {
            return;
        }

        mGLSLMetaDataSource = std::string(mSource.c_str(), meta_begin_idx, meta_end_idx - meta_begin_idx);
        Base::StringUtil::replaceString(&mGLSLMetaDataSource, gLNFXTag[6].DefineStr, "\n");

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