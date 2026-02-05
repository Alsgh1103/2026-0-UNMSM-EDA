#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <iostream>
#include "../general/types.h"
#include "../util.h"
using namespace std;

template <typename T, typename _Func>
struct ListTrait{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTrait : 
    public ListTrait<T, std::greater<T> >{
};

template <typename T>
struct DescendingTrait : 
    public ListTrait<T, std::less<T> >{
};

template <typename Traits>
class NodeLinkedList{

    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedList<Traits>;
private:
    value_type m_data;
    ref_type   m_ref;
    Node *m_pNext = nullptr;


public:
    NodeLinkedList(){}
    NodeLinkedList( value_type _value, ref_type _ref = -1, Node* _next=nullptr)
        : m_data(_value), m_ref(_ref), m_pNext(_next) {   }
    value_type  GetValue   () const { return m_data; }
    value_type &GetValueRef() { return m_data; }

    ref_type    GetRef     () const { return m_ref;   }
    ref_type   &GetRefRef  () { return m_ref;   }

    Node      * GetNext     () const { return m_pNext;   }
    Node      *&GetNextRef  () { return m_pNext;   }

    Node &operator=(const Node &another){
        m_data = another.GetValue();
        m_ref   = another.GetRef();
        return *this;
    }
    bool operator==(const Node &another) const
    { return m_data == another.GetValue();   }
    bool operator<(const Node &another) const
    { return m_data < another.GetValue();   }
};


//iterator
template <typename Traits>
class LinkedListForwardIterator{
    using Node = NodeLinkedList<Traits>;
    using value_type =typename Traits::value_type;

    

};

template <typename Traits>
class CLinkedList {
    using  value_type  = typename Traits::value_type;
    // using  forward_iterator  = LinkedListForwardIterator < CLinkedList<Traits> >;
    // friend forward_iterator;
    // friend GeneralIterator< CLinkedList<Traits> >;
    using  Node = NodeLinkedList<Traits>;

    Node *m_pRoot = nullptr;
    Node *m_pLast = nullptr;
    size_t m_nElements = 0;
public:
    CLinkedList(){}

    void push_back(value_type &val, ref_type ref);
    void Insert(const value_type &val, ref_type ref);
    size_t getSize(){ return m_nElements;  }
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);

    // friend ostream &operator<<(ostream &os, CLinkedList<Traits> &container){
    //     os << "CLinkedList: size = " << container.m_nElements << endl;
    //     os << "[";
    //     for (auto i = 0; i < container.getSize(); ++i){
    //         os << "(" << arr.m_data[i].GetValue() << ":" << arr.m_data[i].GetRef() << "),";
    //     }
    //     os << "]" << endl;
    //     return os;
    // }

    friend ostream &operator<<(ostream &os, CLinkedList<Traits> &container){
        os << "ClinkedList: size = " << container.getSize() << endl;
        Node *currentNode = container.m_pRoot;
        while ( currentNode != nullptr){
            os << "[" << currentNode->GetValue() << "," << currentNode->GetRef() << "]";
            
            if ( currentNode->GetNext() != nullptr)
                os << "->";
            currentNode = currentNode->GetNext();
        }
        return os;
    }
};

template <typename Traits>
void CLinkedList<Traits>::push_back(value_type &val, ref_type ref){
    Node *pNewNode = new Node(val, ref);
    if   ( !m_pRoot )
        m_pRoot = pNewNode;
    else
        m_pLast->m_pNext=pNewNode;
    m_pLast = pNewNode;
    ++m_nElements;
}

template <typename Traits>
void CLinkedList<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
    if( !rParent || rParent->GetValue() > val ){
        Node *pNew = new Node(val, ref, rParent);
        rParent = pNew;
        ++m_nElements;
        return;
    }
    InternalInsert(rParent->GetNextRef(), val, ref);
}

template <typename Traits>
void CLinkedList<Traits>::Insert(const value_type &val, ref_type ref){
    InternalInsert(m_pRoot, val, ref);
}

#endif // __LINKEDLIST_H__
