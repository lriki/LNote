//=============================================================================
//�y HierarchyNode �z
//-----------------------------------------------------------------------------
///**
//  @file       HierarchyNode.h
//  @brief      HierarchyNode
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <algorithm>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Base
{
enum MoveDirection
{
    MOVE_DIR_Front = 0,
    MOVE_DIR_Back,
    MOVE_DIR_Parent,
    MOVE_DIR_Child
};

//=============================================================================
// �� HierarchyNode
//-----------------------------------------------------------------------------
///**
//  @brief		�K�w�\���m�[�h
//
//  @note
//              addChild()�AremoveChild() �ŎQ�ƃJ�E���g�𑀍삵�Ă���_�ɒ��ӁB
//              �Q�Ƃ͐e���q�B
//*/
//=============================================================================
template< class SUB_CLASS_T >
class HierarchyNode
    : public ReferenceObject
{
public:

    typedef SUB_CLASS_T                     NodeType;
    //typedef HierarchyNode< SUB_CLASS_T >    DerivedType;
    typedef std::vector< NodeType* >        NodeList;

public:

    /// �R���X�g���N�^
    HierarchyNode()
        : mParent   ( NULL )
    {
    }

    /// �f�X�g���N�^
    virtual ~HierarchyNode()
    {
        // �폜���X�g�̍쐬
	    //  �C�e���[�g���̉�������ɂȂ�̂ŁA���ʂɂ��Ǝq�m�[�h�̃f�X�g���N�^����
        //  removeChild() ���Ă΂�A�C�e���[�^�������B
	    //  ���̂��߁A�폜����ׂ��R���g���[���̃|�C���^����x�ʂ̔z��ɓ���Ă��珈��������B
	    NodeList delete_list;
	    delete_list.reserve( mChildren.size() );
        ln_foreach( NodeType* n, mChildren )
        {
            delete_list.push_back( n );
        }

        // ���ۂ̍폜
        ln_foreach( NodeType* n, delete_list )
        {
            n->release();
        }

        // ���̃m�[�h���e����O��
        if ( mParent )
        {
            mParent->_removeChild( (NodeType*)this );
        }
    }

public:

    /// �q�m�[�h��ǉ�����
    virtual void addChild( NodeType* child_ )
    {
        LN_SAFE_ADDREF( child_ );

	    if ( child_->mParent )
	    {
		    child_->mParent->removeChild( child_ );
	    }

        mChildren.push_back( child_ );
        child_->mParent = (NodeType*)this;
    }

    /// �q�m�[�h���O��
    virtual void removeChild( NodeType* child_ )
    {
        if ( child_->mParent == this )
        {
            mChildren.erase(std::remove( mChildren.begin(), mChildren.end(), child_ ));
            child_->mParent = NULL;
            LN_SAFE_RELEASE( child_ );
        }
    }

    /// �e�v�f�̎擾
    NodeType* getParent() { return mParent; }

    /// �q�z��̎擾
    NodeList& getChildren()
    {
        return mChildren;
    }

public:

    /// <summary>
    /// node �����̃m�[�h�̂ЂƂO�ɒǉ����� (�e�m�[�h�K�{)
    /// </summary>
    void insertNode( NodeType* node_ )
    {
        if (node_->mParent)
        {
            node_->mParent->removeChild(node_);
        }

        mParent->mChildren.insert(
            std::find( mParent->mChildren.begin(), mParent->mChildren.end(), this ),
            node_ );
        LN_SAFE_ADDREF( node_ );

        node_->mParent = mParent;
    }

    /// <summary>
    /// node �����̃m�[�h�̂ЂƂ��ɒǉ�����
    /// </summary>
    void addNode(NodeType* node_)
    {
        if (node_->mParent)
        {
            node_->mParent->removeChild(node_);
        }

        mParent->mChildren.insert(
            (std::find( mParent->mChildren.begin(), mParent->mChildren.end(), this ) + 1),
            node_ );
        LN_SAFE_ADDREF( node_ );

        node_->mParent = mParent;
    }

    /// <summary>
    /// node �����̃m�[�h�̎q�Ƃ��Ēǉ�����
    /// </summary>
    //public virtual void AddChildNode(LNTreeNode node)
    //{
    //    LNTreeNode src_parent = node.Parent;
    //    if (src_parent != null)
    //    {
    //        src_parent.Children.Remove(node);
    //    }

    //    this.Children.Add(node);
    //    node.Parent = this;
    //}

    /// <summary>
    /// �m�[�h���ړ�
    /// </summary>
    void moveNode(MoveDirection dir)
    {
        if (!canMoveNode(dir)) { return; }
        switch (dir)
        {
            case MOVE_DIR_Front:
            {
                // �ړ��\���� canMoveNode() �Ń`�F�b�N����Ă���̂Ŕ͈̓`�F�b�N�͂��Ȃ���OK
                typename NodeList::iterator itr = std::find( mParent->mChildren.begin(), mParent->mChildren.end(), this ) - 1;
                (*itr)->insertNode( (NodeType*)this );
                break;
            }
            case MOVE_DIR_Back:
            {
                typename NodeList::iterator itr = std::find( mParent->mChildren.begin(), mParent->mChildren.end(), this ) + 1;
                (*itr)->insertNode( (NodeType*)this );
                break;
            }
            case MOVE_DIR_Parent:
            {
                mParent->addNode( (NodeType*)this );
                break;
            }
            case MOVE_DIR_Child:
            {
                typename NodeList::iterator itr = std::find( mParent->mChildren.begin(), mParent->mChildren.end(), this ) - 1;
                (*itr)->addChild( (NodeType*)this );
                break;
            }
        }
    }

    /// <summary>
    /// �m�[�h���ړ��\�����`�F�b�N����
    /// </summary>
    bool canMoveNode( MoveDirection dir_ )
    {
        if (!mParent) return false;

        switch (dir_)
        {
            case MOVE_DIR_Front:
                if (this != *(mParent->mChildren.begin()))
                {
                    return true;
                }
                break;
            case MOVE_DIR_Back:
                if (this != *(mParent->mChildren.rbegin()))
                {
                    return true;
                }
                break;
            case MOVE_DIR_Parent:
                if (mParent->mParent)
                {
                    return true;
                }
                break;
            case MOVE_DIR_Child:
                if (this != *(mParent->mChildren.begin()))
                {
                    return true;
                }
                break;
        }
        return false;
    }

    /// <summary>
    /// node ���O�� (node�ȉ��̃m�[�h�����ׂĉ������ꍇ�� true)
    /// </summary>
    /// <param name="node"></param>
    /// <param name="hierarchy"></param>
    //public virtual void RemoveNode(LNTreeNode node, bool hierarchy = false)
    //{
    //    if (hierarchy)
    //    {
    //        foreach (LNTreeNode n in node.Children)
    //        {
    //            RemoveNode(n, hierarchy);
    //        }
    //    }

    //    Children.Remove(node);
    //    node.Parent = null;
    //}

private:

    /// (�f�X�g���N�^����Ăԗp�B�Q�ƃJ�E���g�̓f�X�g���N�^���ő���)
    virtual void _removeChild( NodeType* child_ )
    {
        if ( child_->mParent == this )
        {
            mChildren.erase( std::remove( mChildren.begin(), mChildren.end(), child_ ) );
            child_->mParent = NULL;
        }
    }

protected:

    NodeType*   mParent;
    NodeList    mChildren;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Base
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================