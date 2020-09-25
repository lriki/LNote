//=============================================================================
//�y GUIElement �z
//-----------------------------------------------------------------------------
///**
//  @file       GUIElement.h
//  @brief      GUIElement
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace GUI
{

//=============================================================================
// �� GUIElement
//-----------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              Rect �� Margin �ɂ��āc
//                  Rect �̓��[�J�����ł̍��W&�T�C�Y�B
//                  �e�R���g���[���̃T�C�Y��Margin �ɂ���ĕ`��̈悪
//                  Rect �T�C�Y�����������Ȃ��Ă��ARect �T�C�Y�͈ێ������B
//                  Margin �͐����A�����A���C���̂��߂̃I�t�Z�b�g�B
//*/
//=============================================================================
class GUIElement
    : public Base::ReferenceObject
    , public BindingTargetElement
{
public:

    struct Template;

public:

    GUIElement( Manager* manager_ );

    virtual ~GUIElement();

public:

    /// ������
    void initialize();
    Manager* getManager() const { return mManager; }

    void                setRect( const LRect& rect_ );
    const LRect&        getRect() const { return mElementRect; }

    void                setMargin( const LThickness& margin_ ) { mMargin = margin_; }
    const LThickness&   getMargin() const { return mMargin; }

    void                setVisible( bool flag_ ) { mVisible = flag_; }
    bool                isVisible() const { return mVisible; }

    void                setEnable( bool flag_ ) { mEnable = flag_; }
    bool                isEnable() const { return mEnable; }

    bool                isActive() const;   ///< ����K�w�̎q�v�f���ŃA�N�e�B�u���𔻒� (�^�u�I�[�_�[�������Ă��邩�A�܂��̓t�H�[�J�X�������Ă��邩)
    bool                hasFocus() const;

    void                focus();

    virtual void        setDataContext( INotifyPropertyChanged* object_ );
    INotifyPropertyChanged* getDataContext() const { return mDataContext; }

public:

    /// �e���v���[�g���琶��
    virtual void initialize( Template* data_ );

    /// �ʒu���̍X�V (parent_client_rect_ �̓O���[�o�����W)
    virtual void updateCoordinate( const LRect& parent_client_rect_ );

    /// �}�E�X�ʒu�Ƃ̓����蔻��Ɏg�p����R���C�_�̍X�V (�f�t�H���g�ł� mBoundingRect ���X�V)
    virtual void updateCollider();

    /// �}�E�X�ʒu�Əd�Ȃ��Ă��邩�𔻒肷�� (mouse_pos_ �̓O���[�o�����W)
    virtual bool checkMouseHover( const LPoint& mouse_pos );

    /// �}�E�X�ʒu�Əd�Ȃ��Ă��邩�𔻒肷�� (�q���܂߂āA�d�Ȃ��Ă���R���g���[����Ԃ�)
    ///     �����ł� checkMouseHover() �� true ��Ԃ��ꍇ�Athis ��Ԃ��Ă���B
    ///     ����� GUIControl �ł̍ċA�I�ȏ������V���v���ɏ������߂̑[�u�B
    virtual GUIElement* getMouseHoverElement( const LPoint& mouse_pos );

    /// �t�H�[�J�X�����Ă�R���g���[���̏ꍇ�� true ��Ԃ� (�T�u�N���X�ŃI�[�o�[���C�h)
    virtual bool canFocus() const { return false; }

    /// �C�x���g���� (�C�x���g�������������Ƃ������ɂ� true ��Ԃ�)
    virtual bool doEvent( const Event& e_ ) { return false; }

    /// �A�N�e�B�u��Ԃ̃`�F�b�N�J�n (Manager::setFocusControl() ����Ă΂��)
    virtual void beginCheckActivate();

    /// �A�N�e�B�u��Ԃ̃`�F�b�N�I�� (Manager::setFocusControl() ����Ă΂��)
    virtual void endCheckActivate( bool active_ );

    /// �`����̍쐬
    virtual void updateDrawContents() {}

    /// �`��
    virtual void draw() {}

public:

    virtual void onContextPropertyChanged( const lnChar* property_name_ ) {}

protected:

    Manager*                mManager;
    GUIControl*             mParent;
    INotifyPropertyChanged* mDataContext;
    LRect                   mElementRect;
    LThickness              mMargin;
    LPoint                  mGlobalPosition;    ///< �O���[�o�����W (updateCoordinate() �ō쐬�����)
    LRect                   mBoundingRect;      ///< �}�E�X�Ƃ̓����蔻��Ɏg����` (�O���[�o�����)
    Graphics::ITexture*     mSkinTexture;
    bool                    mVisible;
    bool                    mEnable;
    bool                    mTempActive;
    bool                    mIsInternalElement;

    friend class GUIControl;

public:

    /// �ǂ��������Ă����� style?
    struct Template
    {
        ElementType         Type;           ///< ��������N���X�̔���Ɏg�p����B�p����Őݒ肷�邱�ƁB

        LRect               ElementRect;
        LThickness          Margin;
        Graphics::ITexture* SkinTexture;    // ���O�̕�����������

        TemplateBindableValue   Width;
        TemplateBindableValue   Height;
        TemplateBindableValue   _Margin;
        TemplateBindableValue   _SkinTexture;

        Template()
        {
            Type = ELEMENTTYPE_UNKNOWN,
            ElementRect.set( 0, 0, 32, 32 );
            Margin.set( 0, 0, 0, 0 );
            SkinTexture = NULL;

            Width.initialize( 64 );
            Height.initialize( 32 );
            _Margin.initialize( LVector4( 0, 0, 0, 0 ) );
            _SkinTexture.initialize( _T("") );
        }


        virtual void serialize( const LRefTString& attr, const LRefTString& value )
        {
            
        }
    };

    typedef std::vector< Template* > TemplateArray;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace GUI
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================