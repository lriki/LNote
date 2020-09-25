//=============================================================================
//【 HierarchyNode 】
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
// ■ HierarchyNode
//-----------------------------------------------------------------------------
///**
//  @brief		階層構造ノード
//
//  @note
//              addChild()、removeChild() で参照カウントを操作している点に注意。
//              参照は親→子。
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

    /// コンストラクタ
    HierarchyNode()
        : mParent   ( NULL )
    {
    }

    /// デストラクタ
    virtual ~HierarchyNode()
    {
        // 削除リストの作成
	    //  イテレート中の解放処理になるので、普通にやると子ノードのデストラクタから
        //  removeChild() が呼ばれ、イテレータが崩れる。
	    //  そのため、削除するべきコントロールのポインタを一度別の配列に入れてから処理をする。
	    NodeList delete_list;
	    delete_list.reserve( mChildren.size() );
        ln_foreach( NodeType* n, mChildren )
        {
            delete_list.push_back( n );
        }

        // 実際の削除
        ln_foreach( NodeType* n, delete_list )
        {
            n->release();
        }

        // このノードも親から外す
        if ( mParent )
        {
            mParent->_removeChild( (NodeType*)this );
        }
    }

public:

    /// 子ノードを追加する
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

    /// 子ノードを外す
    virtual void removeChild( NodeType* child_ )
    {
        if ( child_->mParent == this )
        {
            mChildren.erase(std::remove( mChildren.begin(), mChildren.end(), child_ ));
            child_->mParent = NULL;
            LN_SAFE_RELEASE( child_ );
        }
    }

    /// 親要素の取得
    NodeType* getParent() { return mParent; }

    /// 子配列の取得
    NodeList& getChildren()
    {
        return mChildren;
    }

public:

    /// <summary>
    /// node をこのノードのひとつ前に追加する (親ノード必須)
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
    /// node をこのノードのひとつ後ろに追加する
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
    /// node をこのノードの子として追加する
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
    /// ノードを移動
    /// </summary>
    void moveNode(MoveDirection dir)
    {
        if (!canMoveNode(dir)) { return; }
        switch (dir)
        {
            case MOVE_DIR_Front:
            {
                // 移動可能かは canMoveNode() でチェックされているので範囲チェックはしなくてOK
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
    /// ノードが移動可能かをチェックする
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
    /// node を外す (node以下のノードをすべて解放する場合は true)
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

    /// (デストラクタから呼ぶ用。参照カウントはデストラクタ側で操作)
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