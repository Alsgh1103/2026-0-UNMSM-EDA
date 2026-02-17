#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <iostream>
#include <utility>
#include <mutex>
#include "../general/types.h"
#include "../util.h"


template <typename T>
struct TreeTraitAscending {
    using value_type  = T;
    using CompareFunc = std::greater<T>;
};

template <typename T>
struct TreeTraitDescending {
    using value_type  = T;
    using CompareFunc = std::less<T>;
};

template <typename Traits>
class NodeBinaryTree{
    using  value_type  = typename Traits::value_type;
    using  Node        = typename NodeBinaryTree<Traits>;
    using  CompareFunc = typename Traits::CompareFunc;
private:
    value_type m_data;
    ref_type   m_ref;
    Node *m_pParent    = nullptr; 
    Node *m_pChild[2]  = { nullptr} ;
public:
    NodeBinaryTree(){}
    NodeBinaryTree( value_type _value, ref_type _ref = -1)
        : m_data(_value), m_ref(_ref){   }
    value_type  GetValue   () const { return m_data; }
    value_type &GetValueRef()       { return m_data; }
    ref_type    GetRef     () const { return m_ref;  }
    ref_type   &GetRefRef  ()       { return m_ref;  }
};

template <typename Traits>
class CBinaryTree{
public:
    using  value_type  = typename Traits::value_type;
    using  Node        = typename NodeBinaryTree<Traits>;
    using  CompareFunc = typename Traits::CompareFunc;
private:
    Node *m_pRoot = nullptr;
    CompareFunc comp;
    mutable std::recursive_mutex m_mtx;

public:
    CBinaryTree(){}
    // TODO: Copy constructor
    CBinaryTree(const CBinaryTree &another);
    // TODO: Move constructor
    CBinaryTree(CBinaryTree &&another) noexcept
        : m_pRoot(std::exchange(another.m_pRoot,nullptr))
    {}
    virtual ~CBinaryTree(){

    }
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
        if( !rParent ){
            rParent = new Node(val, ref);
            return;
        }
        auto path = comp(val, rParent->GetValue());
        InternalInsert(rParent->m_pChild[path], val, ref);
    }

    Node* Clone(Node *pOriginal){
        if (!pOriginal)
            return nullptr;
        Node *pNewNode = new Node(pOriginal->GetValue(),pOriginal->GetRef());
        if (pOriginal->m_pChild[0]){
            pNewNode->m_pChild[0]            = Clone(pOriginal->m_pChild[0]);
            pNewNode->m_pChild[0]->m_pParent = pNewNode;
        }
        if (pOriginal->m_pChild[1]){
            pNewNode->m_pChild[1]            = Clone(pOriginal->m_pChild[1]);
            pNewNode->m_pChild[1]->m_pParent = pNewNode;
        }
        return pNewNode;
    }
    Destroy(pNode->Left); 
Destroy(pNode->Right); 
delete pNode;   
public:
    void Insert(const value_type &val, ref_type ref){
        InternalInsert(m_pRoot, val, ref);
    }
};

//Copy Constructor
template <typename Traits>
CBinaryTree<Traits>::CBinaryTree(const CBinaryTree<Traits> &another){
    std::lock_guard<std::recursive_mutex> lock(another.m_mtx);
    m_pRoot = Clone(another.m_pRoot);
}
#endif // __BINARYTREE_H__